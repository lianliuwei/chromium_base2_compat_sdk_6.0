// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "base/process_util.h"

#include <dirent.h>
#include <malloc.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include "base/file_util.h"
#include "base/logging.h"
#include "base/process/internal_linux.h"
#include "base/string_util.h"
#include "base/strings/string_number_conversions.h"
#include "base/strings/string_split.h"
#include "base/sys_info.h"
#include "base/threading/thread_restrictions.h"

namespace base {

namespace {

// Reads the |field_num|th field from |proc_stats|.
// Returns an empty string on failure.
// This version only handles VM_COMM and VM_STATE, which are the only fields
// that are strings.
std::string GetProcStatsFieldAsString(
    const std::vector<std::string>& proc_stats,
    internal::ProcStatsFields field_num) {
  if (field_num < internal::VM_COMM || field_num > internal::VM_STATE) {
    NOTREACHED();
    return std::string();
  }

  if (proc_stats.size() > static_cast<size_t>(field_num))
    return proc_stats[field_num];

  NOTREACHED();
  return 0;
}

// Reads /proc/<pid>/cmdline and populates |proc_cmd_line_args| with the command
// line arguments. Returns true if successful.
// Note: /proc/<pid>/cmdline contains command line arguments separated by single
// null characters. We tokenize it into a vector of strings using '\0' as a
// delimiter.
bool GetProcCmdline(pid_t pid, std::vector<std::string>* proc_cmd_line_args) {
  // Synchronously reading files in /proc is safe.
  ThreadRestrictions::ScopedAllowIO allow_io;

  FilePath cmd_line_file = internal::GetProcPidDir(pid).Append("cmdline");
  std::string cmd_line;
  if (!file_util::ReadFileToString(cmd_line_file, &cmd_line))
    return false;
  std::string delimiters;
  delimiters.push_back('\0');
  Tokenize(cmd_line, delimiters, proc_cmd_line_args);
  return true;
}

}  // namespace

#if defined(USE_LINUX_BREAKPAD)
size_t g_oom_size = 0U;
#endif

const char kProcSelfExe[] = "/proc/self/exe";

ProcessId GetParentProcessId(ProcessHandle process) {
  ProcessId pid =
      internal::ReadProcStatsAndGetFieldAsInt(process, internal::VM_PPID);
  if (pid)
    return pid;
  return -1;
}

FilePath GetProcessExecutablePath(ProcessHandle process) {
  FilePath stat_file = internal::GetProcPidDir(process).Append("exe");
  FilePath exe_name;
  if (!file_util::ReadSymbolicLink(stat_file, &exe_name)) {
    // No such process.  Happens frequently in e.g. TerminateAllChromeProcesses
    return FilePath();
  }
  return exe_name;
}

ProcessIterator::ProcessIterator(const ProcessFilter* filter)
    : filter_(filter) {
  procfs_dir_ = opendir(internal::kProcDir);
}

ProcessIterator::~ProcessIterator() {
  if (procfs_dir_) {
    closedir(procfs_dir_);
    procfs_dir_ = NULL;
  }
}

bool ProcessIterator::CheckForNextProcess() {
  // TODO(port): skip processes owned by different UID

  pid_t pid = kNullProcessId;
  std::vector<std::string> cmd_line_args;
  std::string stats_data;
  std::vector<std::string> proc_stats;

  // Arbitrarily guess that there will never be more than 200 non-process
  // files in /proc.  Hardy has 53 and Lucid has 61.
  int skipped = 0;
  const int kSkipLimit = 200;
  while (skipped < kSkipLimit) {
    dirent* slot = readdir(procfs_dir_);
    // all done looking through /proc?
    if (!slot)
      return false;

    // If not a process, keep looking for one.
    pid = internal::ProcDirSlotToPid(slot->d_name);
    if (!pid) {
      skipped++;
      continue;
    }

    if (!GetProcCmdline(pid, &cmd_line_args))
      continue;

    if (!internal::ReadProcStats(pid, &stats_data))
      continue;
    if (!internal::ParseProcStats(stats_data, &proc_stats))
      continue;

    std::string runstate =
        GetProcStatsFieldAsString(proc_stats, internal::VM_STATE);
    if (runstate.size() != 1) {
      NOTREACHED();
      continue;
    }

    // Is the process in 'Zombie' state, i.e. dead but waiting to be reaped?
    // Allowed values: D R S T Z
    if (runstate[0] != 'Z')
      break;

    // Nope, it's a zombie; somebody isn't cleaning up after their children.
    // (e.g. WaitForProcessesToExit doesn't clean up after dead children yet.)
    // There could be a lot of zombies, can't really decrement i here.
  }
  if (skipped >= kSkipLimit) {
    NOTREACHED();
    return false;
  }

  entry_.pid_ = pid;
  entry_.ppid_ = GetProcStatsFieldAsInt(proc_stats, internal::VM_PPID);
  entry_.gid_ = GetProcStatsFieldAsInt(proc_stats, internal::VM_PGRP);
  entry_.cmd_line_args_.assign(cmd_line_args.begin(), cmd_line_args.end());
  entry_.exe_file_ = GetProcessExecutablePath(pid).BaseName().value();
  return true;
}

bool NamedProcessIterator::IncludeEntry() {
  if (executable_name_ != entry().exe_file())
    return false;
  return ProcessIterator::IncludeEntry();
}


int GetNumberOfThreads(ProcessHandle process) {
  return internal::ReadProcStatsAndGetFieldAsInt(process,
                                                 internal::VM_NUMTHREADS);
}

namespace {

void OnNoMemorySize(size_t size) {
#if defined(USE_LINUX_BREAKPAD)
  g_oom_size = size;
#endif

  if (size != 0)
    LOG(FATAL) << "Out of memory, size = " << size;
  LOG(FATAL) << "Out of memory.";
}

void OnNoMemory() {
  OnNoMemorySize(0);
}

}  // namespace

#if !defined(ADDRESS_SANITIZER) && !defined(MEMORY_SANITIZER) && \
    !defined(THREAD_SANITIZER)

#if defined(LIBC_GLIBC) && !defined(USE_TCMALLOC)

extern "C" {
void* __libc_malloc(size_t size);
void* __libc_realloc(void* ptr, size_t size);
void* __libc_calloc(size_t nmemb, size_t size);
void* __libc_valloc(size_t size);
void* __libc_pvalloc(size_t size);
void* __libc_memalign(size_t alignment, size_t size);

// Overriding the system memory allocation functions:
//
// For security reasons, we want malloc failures to be fatal. Too much code
// doesn't check for a NULL return value from malloc and unconditionally uses
// the resulting pointer. If the first offset that they try to access is
// attacker controlled, then the attacker can direct the code to access any
// part of memory.
//
// Thus, we define all the standard malloc functions here and mark them as
// visibility 'default'. This means that they replace the malloc functions for
// all Chromium code and also for all code in shared libraries. There are tests
// for this in process_util_unittest.cc.
//
// If we are using tcmalloc, then the problem is moot since tcmalloc handles
// this for us. Thus this code is in a !defined(USE_TCMALLOC) block.
//
// If we are testing the binary with AddressSanitizer, we should not
// redefine malloc and let AddressSanitizer do it instead.
//
// We call the real libc functions in this code by using __libc_malloc etc.
// Previously we tried using dlsym(RTLD_NEXT, ...) but that failed depending on
// the link order. Since ld.so needs calloc during symbol resolution, it
// defines its own versions of several of these functions in dl-minimal.c.
// Depending on the runtime library order, dlsym ended up giving us those
// functions and bad things happened. See crbug.com/31809
//
// This means that any code which calls __libc_* gets the raw libc versions of
// these functions.

#define DIE_ON_OOM_1(function_name) \
  void* function_name(size_t) __attribute__ ((visibility("default"))); \
  \
  void* function_name(size_t size) { \
    void* ret = __libc_##function_name(size); \
    if (ret == NULL && size != 0) \
      OnNoMemorySize(size); \
    return ret; \
  }

#define DIE_ON_OOM_2(function_name, arg1_type) \
  void* function_name(arg1_type, size_t) \
      __attribute__ ((visibility("default"))); \
  \
  void* function_name(arg1_type arg1, size_t size) { \
    void* ret = __libc_##function_name(arg1, size); \
    if (ret == NULL && size != 0) \
      OnNoMemorySize(size); \
    return ret; \
  }

DIE_ON_OOM_1(malloc)
DIE_ON_OOM_1(valloc)
DIE_ON_OOM_1(pvalloc)

DIE_ON_OOM_2(calloc, size_t)
DIE_ON_OOM_2(realloc, void*)
DIE_ON_OOM_2(memalign, size_t)

// posix_memalign has a unique signature and doesn't have a __libc_ variant.
int posix_memalign(void** ptr, size_t alignment, size_t size)
    __attribute__ ((visibility("default")));

int posix_memalign(void** ptr, size_t alignment, size_t size) {
  // This will use the safe version of memalign, above.
  *ptr = memalign(alignment, size);
  return 0;
}

}  // extern C

#else

// TODO(mostynb@opera.com): dlsym dance

#endif  // LIBC_GLIBC && !USE_TCMALLOC

#endif  // !*_SANITIZER

void EnableTerminationOnHeapCorruption() {
  // On Linux, there nothing to do AFAIK.
}

void EnableTerminationOnOutOfMemory() {
#if defined(OS_ANDROID)
  // Android doesn't support setting a new handler.
  DLOG(WARNING) << "Not feasible.";
#else
  // Set the new-out of memory handler.
  std::set_new_handler(&OnNoMemory);
  // If we're using glibc's allocator, the above functions will override
  // malloc and friends and make them die on out of memory.
#endif
}

// NOTE: This is not the only version of this function in the source:
// the setuid sandbox (in process_util_linux.c, in the sandbox source)
// also has its own C version.
bool AdjustOOMScore(ProcessId process, int score) {
  if (score < 0 || score > kMaxOomScore)
    return false;

  FilePath oom_path(internal::GetProcPidDir(process));

  // Attempt to write the newer oom_score_adj file first.
  FilePath oom_file = oom_path.AppendASCII("oom_score_adj");
  if (file_util::PathExists(oom_file)) {
    std::string score_str = IntToString(score);
    DVLOG(1) << "Adjusting oom_score_adj of " << process << " to "
             << score_str;
    int score_len = static_cast<int>(score_str.length());
    return (score_len == file_util::WriteFile(oom_file,
                                              score_str.c_str(),
                                              score_len));
  }

  // If the oom_score_adj file doesn't exist, then we write the old
  // style file and translate the oom_adj score to the range 0-15.
  oom_file = oom_path.AppendASCII("oom_adj");
  if (file_util::PathExists(oom_file)) {
    // Max score for the old oom_adj range.  Used for conversion of new
    // values to old values.
    const int kMaxOldOomScore = 15;

    int converted_score = score * kMaxOldOomScore / kMaxOomScore;
    std::string score_str = IntToString(converted_score);
    DVLOG(1) << "Adjusting oom_adj of " << process << " to " << score_str;
    int score_len = static_cast<int>(score_str.length());
    return (score_len == file_util::WriteFile(oom_file,
                                              score_str.c_str(),
                                              score_len));
  }

  return false;
}

}  // namespace base
