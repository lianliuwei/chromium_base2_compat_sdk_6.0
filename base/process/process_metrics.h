// Copyright (c) 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file contains routines for gathering resource statistics for processes
// running on the system.

#ifndef BASE_PROCESS_PROCESS_METRICS_H_
#define BASE_PROCESS_PROCESS_METRICS_H_

#include <string>

#include "base/base_export.h"
#include "base/basictypes.h"
#include "base/process.h"
#include "base/time.h"

#if defined(OS_MACOSX)
#include <mach/mach.h>
#endif

namespace base {

#if defined(OS_WIN)
struct IoCounters : public IO_COUNTERS {
};
#elif defined(OS_POSIX)
struct IoCounters {
  uint64_t ReadOperationCount;
  uint64_t WriteOperationCount;
  uint64_t OtherOperationCount;
  uint64_t ReadTransferCount;
  uint64_t WriteTransferCount;
  uint64_t OtherTransferCount;
};
#endif

// Working Set (resident) memory usage broken down by
//
// On Windows:
// priv (private): These pages (kbytes) cannot be shared with any other process.
// shareable:      These pages (kbytes) can be shared with other processes under
//                 the right circumstances.
// shared :        These pages (kbytes) are currently shared with at least one
//                 other process.
//
// On Linux:
// priv:           Pages mapped only by this process
// shared:         PSS or 0 if the kernel doesn't support this
// shareable:      0
//
// On OS X: TODO(thakis): Revise.
// priv:           Memory.
// shared:         0
// shareable:      0
struct WorkingSetKBytes {
  WorkingSetKBytes() : priv(0), shareable(0), shared(0) {}
  size_t priv;
  size_t shareable;
  size_t shared;
};

// Committed (resident + paged) memory usage broken down by
// private: These pages cannot be shared with any other process.
// mapped:  These pages are mapped into the view of a section (backed by
//          pagefile.sys)
// image:   These pages are mapped into the view of an image section (backed by
//          file system)
struct CommittedKBytes {
  CommittedKBytes() : priv(0), mapped(0), image(0) {}
  size_t priv;
  size_t mapped;
  size_t image;
};

// Free memory (Megabytes marked as free) in the 2G process address space.
// total : total amount in megabytes marked as free. Maximum value is 2048.
// largest : size of the largest contiguous amount of memory found. It is
//   always smaller or equal to FreeMBytes::total.
// largest_ptr: starting address of the largest memory block.
struct FreeMBytes {
  size_t total;
  size_t largest;
  void* largest_ptr;
};

// Convert a POSIX timeval to microseconds.
BASE_EXPORT int64 TimeValToMicroseconds(const struct timeval& tv);

// Provides performance metrics for a specified process (CPU usage, memory and
// IO counters). To use it, invoke CreateProcessMetrics() to get an instance
// for a specific process, then access the information with the different get
// methods.
class BASE_EXPORT ProcessMetrics {
 public:
  ~ProcessMetrics();

  // Creates a ProcessMetrics for the specified process.
  // The caller owns the returned object.
#if !defined(OS_MACOSX) || defined(OS_IOS)
  static ProcessMetrics* CreateProcessMetrics(ProcessHandle process);
#else
  class PortProvider {
   public:
    virtual ~PortProvider() {}

    // Should return the mach task for |process| if possible, or else
    // |MACH_PORT_NULL|. Only processes that this returns tasks for will have
    // metrics on OS X (except for the current process, which always gets
    // metrics).
    virtual mach_port_t TaskForPid(ProcessHandle process) const = 0;
  };

  // The port provider needs to outlive the ProcessMetrics object returned by
  // this function. If NULL is passed as provider, the returned object
  // only returns valid metrics if |process| is the current process.
  static ProcessMetrics* CreateProcessMetrics(ProcessHandle process,
                                              PortProvider* port_provider);
#endif  // !defined(OS_MACOSX) || defined(OS_IOS)

  // Returns the current space allocated for the pagefile, in bytes (these pages
  // may or may not be in memory).  On Linux, this returns the total virtual
  // memory size.
  size_t GetPagefileUsage() const;
  // Returns the peak space allocated for the pagefile, in bytes.
  size_t GetPeakPagefileUsage() const;
  // Returns the current working set size, in bytes.  On Linux, this returns
  // the resident set size.
  size_t GetWorkingSetSize() const;
  // Returns the peak working set size, in bytes.
  size_t GetPeakWorkingSetSize() const;
  // Returns private and sharedusage, in bytes. Private bytes is the amount of
  // memory currently allocated to a process that cannot be shared. Returns
  // false on platform specific error conditions.  Note: |private_bytes|
  // returns 0 on unsupported OSes: prior to XP SP2.
  bool GetMemoryBytes(size_t* private_bytes,
                      size_t* shared_bytes);
  // Fills a CommittedKBytes with both resident and paged
  // memory usage as per definition of CommittedBytes.
  void GetCommittedKBytes(CommittedKBytes* usage) const;
  // Fills a WorkingSetKBytes containing resident private and shared memory
  // usage in bytes, as per definition of WorkingSetBytes.
  bool GetWorkingSetKBytes(WorkingSetKBytes* ws_usage) const;

  // Computes the current process available memory for allocation.
  // It does a linear scan of the address space querying each memory region
  // for its free (unallocated) status. It is useful for estimating the memory
  // load and fragmentation.
  bool CalculateFreeMemory(FreeMBytes* free) const;

  // Returns the CPU usage in percent since the last time this method was
  // called. The first time this method is called it returns 0 and will return
  // the actual CPU info on subsequent calls.
  // On Windows, the CPU usage value is for all CPUs. So if you have 2 CPUs and
  // your process is using all the cycles of 1 CPU and not the other CPU, this
  // method returns 50.
  double GetCPUUsage();

  // Retrieves accounting information for all I/O operations performed by the
  // process.
  // If IO information is retrieved successfully, the function returns true
  // and fills in the IO_COUNTERS passed in. The function returns false
  // otherwise.
  bool GetIOCounters(IoCounters* io_counters) const;

 private:
#if !defined(OS_MACOSX) || defined(OS_IOS)
  explicit ProcessMetrics(ProcessHandle process);
#else
  ProcessMetrics(ProcessHandle process, PortProvider* port_provider);
#endif  // !defined(OS_MACOSX) || defined(OS_IOS)

  ProcessHandle process_;

  int processor_count_;

  // Used to store the previous times and CPU usage counts so we can
  // compute the CPU usage between calls.
  int64 last_time_;
  int64 last_system_time_;

#if !defined(OS_IOS)
#if defined(OS_MACOSX)
  // Queries the port provider if it's set.
  mach_port_t TaskForPid(ProcessHandle process) const;

  PortProvider* port_provider_;
#elif defined(OS_POSIX)
  // Jiffie count at the last_time_ we updated.
  int last_cpu_;
#endif  // defined(OS_POSIX)
#endif  // !defined(OS_IOS)

  DISALLOW_COPY_AND_ASSIGN(ProcessMetrics);
};

// Returns the memory committed by the system in KBytes.
// Returns 0 if it can't compute the commit charge.
BASE_EXPORT size_t GetSystemCommitCharge();

#if defined(OS_LINUX) || defined(OS_ANDROID)
// Parse the data found in /proc/<pid>/stat and return the sum of the
// CPU-related ticks.  Returns -1 on parse error.
// Exposed for testing.
BASE_EXPORT int ParseProcStatCPU(const std::string& input);

// Data from /proc/meminfo about system-wide memory consumption.
// Values are in KB.
struct BASE_EXPORT SystemMemoryInfoKB {
  SystemMemoryInfoKB();

  int total;
  int free;
  int buffers;
  int cached;
  int active_anon;
  int inactive_anon;
  int active_file;
  int inactive_file;
  int shmem;

  // Gem data will be -1 if not supported.
  int gem_objects;
  long long gem_size;
};
// Retrieves data from /proc/meminfo about system-wide memory consumption.
// Fills in the provided |meminfo| structure. Returns true on success.
// Exposed for memory debugging widget.
BASE_EXPORT bool GetSystemMemoryInfo(SystemMemoryInfoKB* meminfo);
#endif  // defined(OS_LINUX) || defined(OS_ANDROID)

}  // namespace base

#endif  // BASE_PROCESS_PROCESS_METRICS_H_
