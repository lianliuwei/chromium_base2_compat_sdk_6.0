{ 
   'variables': {
    'chromium_code': 1,
  },
  
  'targets': [
    {
      'target_name': 'base_unittests',
      'type': '<(gtest_target_type)',
      'sources': [
        # Tests.
        '../../base/android/activity_status_unittest.cc',
        '../../base/android/jni_android_unittest.cc',
        '../../base/android/jni_array_unittest.cc',
        '../../base/android/jni_string_unittest.cc',
        '../../base/android/path_utils_unittest.cc',
        '../../base/android/scoped_java_ref_unittest.cc',
        '../../base/at_exit_unittest.cc',
        '../../base/atomicops_unittest.cc',
        '../../base/base64_unittest.cc',
        '../../base/bind_helpers_unittest.cc',
        '../../base/bind_unittest.cc',
        '../../base/bind_unittest.nc',
        '../../base/bits_unittest.cc',
        '../../base/build_time_unittest.cc',
        '../../base/callback_unittest.cc',
        '../../base/callback_unittest.nc',
        '../../base/cancelable_callback_unittest.cc',
        '../../base/command_line_unittest.cc',
        '../../base/containers/linked_list_unittest.cc',
        '../../base/containers/mru_cache_unittest.cc',
        '../../base/containers/small_map_unittest.cc',
        '../../base/containers/stack_container_unittest.cc',
        '../../base/cpu_unittest.cc',
        '../../base/debug/crash_logging_unittest.cc',
        '../../base/debug/leak_tracker_unittest.cc',
        '../../base/debug/stack_trace_unittest.cc',
        '../../base/debug/trace_event_unittest.cc',
        '../../base/debug/trace_event_unittest.h',
        '../../base/debug/trace_event_win_unittest.cc',
        '../../base/deferred_sequenced_task_runner_unittest.cc',
        '../../base/environment_unittest.cc',
        '../../base/file_util_unittest.cc',
        '../../base/file_version_info_unittest.cc',
        '../../base/files/dir_reader_posix_unittest.cc',
        '../../base/files/file_path_unittest.cc',
        '../../base/files/file_util_proxy_unittest.cc',
        '../../base/files/important_file_writer_unittest.cc',
        '../../base/files/scoped_temp_dir_unittest.cc',
        '../../base/gmock_unittest.cc',
        '../../base/guid_unittest.cc',
        '../../base/hi_res_timer_manager_unittest.cc',
        '../../base/id_map_unittest.cc',
        '../../base/i18n/break_iterator_unittest.cc',
        '../../base/i18n/char_iterator_unittest.cc',
        '../../base/i18n/case_conversion_unittest.cc',
        '../../base/i18n/file_util_icu_unittest.cc',
        '../../base/i18n/icu_string_conversions_unittest.cc',
        '../../base/i18n/number_formatting_unittest.cc',
        '../../base/i18n/rtl_unittest.cc',
        '../../base/i18n/string_search_unittest.cc',
        '../../base/i18n/time_formatting_unittest.cc',
        '../../base/ios/device_util_unittest.mm',
        '../../base/json/json_parser_unittest.cc',
        '../../base/json/json_reader_unittest.cc',
        '../../base/json/json_value_converter_unittest.cc',
        '../../base/json/json_value_serializer_unittest.cc',
        '../../base/json/json_writer_unittest.cc',
        '../../base/json/string_escape_unittest.cc',
        '../../base/lazy_instance_unittest.cc',
        '../../base/logging_unittest.cc',
        '../../base/mac/bind_objc_block_unittest.mm',
        '../../base/mac/foundation_util_unittest.mm',
        '../../base/mac/libdispatch_task_runner_unittest.cc',
        '../../base/mac/mac_util_unittest.mm',
        '../../base/mac/objc_property_releaser_unittest.mm',
        '../../base/mac/scoped_sending_event_unittest.mm',
        '../../base/md5_unittest.cc',
        '../../base/memory/aligned_memory_unittest.cc',
        '../../base/memory/discardable_memory_unittest.cc',
        '../../base/memory/linked_ptr_unittest.cc',
        '../../base/memory/ref_counted_memory_unittest.cc',
        '../../base/memory/ref_counted_unittest.cc',
        '../../base/memory/scoped_nsobject_unittest.mm',
        '../../base/memory/scoped_ptr_unittest.cc',
        '../../base/memory/scoped_ptr_unittest.nc',
        '../../base/memory/scoped_vector_unittest.cc',
        '../../base/memory/shared_memory_unittest.cc',
        '../../base/memory/singleton_unittest.cc',
        '../../base/memory/weak_ptr_unittest.cc',
        '../../base/memory/weak_ptr_unittest.nc',
        '../../base/message_loop/message_loop_proxy_impl_unittest.cc',
        '../../base/message_loop/message_loop_proxy_unittest.cc',
        '../../base/message_loop_unittest.cc',
        '../../base/message_pump_glib_unittest.cc',
        '../../base/message_pump_io_ios_unittest.cc',
        '../../base/message_pump_libevent_unittest.cc',
        '../../base/metrics/sample_map_unittest.cc',
        '../../base/metrics/sample_vector_unittest.cc',
        '../../base/metrics/bucket_ranges_unittest.cc',
        '../../base/metrics/field_trial_unittest.cc',
        '../../base/metrics/histogram_base_unittest.cc',
        '../../base/metrics/histogram_unittest.cc',
        '../../base/metrics/sparse_histogram_unittest.cc',
        '../../base/metrics/stats_table_unittest.cc',
        '../../base/metrics/statistics_recorder_unittest.cc',
        '../../base/observer_list_unittest.cc',
        '../../base/os_compat_android_unittest.cc',
        '../../base/path_service_unittest.cc',
        '../../base/pickle_unittest.cc',
        '../../base/platform_file_unittest.cc',
        '../../base/posix/file_descriptor_shuffle_unittest.cc',
        '../../base/posix/unix_domain_socket_linux_unittest.cc',
        '../../base/power_monitor/power_monitor_unittest.cc',
        '../../base/pr_time_unittest.cc',
        '../../base/prefs/default_pref_store_unittest.cc',
        '../../base/prefs/json_pref_store_unittest.cc',
        '../../base/prefs/mock_pref_change_callback.h',
        '../../base/prefs/overlay_user_pref_store_unittest.cc',
        '../../base/prefs/pref_change_registrar_unittest.cc',
        '../../base/prefs/pref_member_unittest.cc',
        '../../base/prefs/pref_notifier_impl_unittest.cc',
        '../../base/prefs/pref_service_unittest.cc',
        '../../base/prefs/pref_value_map_unittest.cc',
        '../../base/prefs/pref_value_store_unittest.cc',
        '../../base/process_util_unittest.cc',
        '../../base/process_util_unittest_ios.cc',
        '../../base/process_util_unittest_mac.h',
        '../../base/process_util_unittest_mac.mm',
        '../../base/profiler/tracked_time_unittest.cc',
        '../../base/rand_util_unittest.cc',
        '../../base/safe_numerics_unittest.cc',
        '../../base/safe_numerics_unittest.nc',
        '../../base/scoped_clear_errno_unittest.cc',
        '../../base/scoped_native_library_unittest.cc',
        '../../base/scoped_observer.h',
        '../../base/security_unittest.cc',
        '../../base/sequence_checker_unittest.cc',
        '../../base/sequence_checker_impl_unittest.cc',
        '../../base/sha1_unittest.cc',
        '../../base/stl_util_unittest.cc',
        '../../base/string16_unittest.cc',
        '../../base/string_util_unittest.cc',
        '../../base/stringprintf_unittest.cc',
        '../../base/strings/string_number_conversions_unittest.cc',
        '../../base/strings/string_piece_unittest.cc',
        '../../base/strings/string_split_unittest.cc',
        '../../base/strings/string_tokenizer_unittest.cc',
        '../../base/strings/stringize_macros_unittest.cc',
        '../../base/strings/sys_string_conversions_mac_unittest.mm',
        '../../base/strings/sys_string_conversions_unittest.cc',
        '../../base/strings/utf_offset_string_conversions_unittest.cc',
        '../../base/strings/utf_string_conversions_unittest.cc',
        '../../base/synchronization/cancellation_flag_unittest.cc',
        '../../base/synchronization/condition_variable_unittest.cc',
        '../../base/synchronization/lock_unittest.cc',
        '../../base/synchronization/waitable_event_unittest.cc',
        '../../base/synchronization/waitable_event_watcher_unittest.cc',
        '../../base/sys_info_unittest.cc',
        '../../base/system_monitor/system_monitor_unittest.cc',
        '../../base/task_runner_util_unittest.cc',
        '../../base/template_util_unittest.cc',
        '../../base/test/expectations/expectation_unittest.cc',
        '../../base/test/expectations/parser_unittest.cc',
        '../../base/test/trace_event_analyzer_unittest.cc',
        '../../base/threading/non_thread_safe_unittest.cc',
        '../../base/threading/platform_thread_unittest.cc',
        '../../base/threading/sequenced_worker_pool_unittest.cc',
        '../../base/threading/simple_thread_unittest.cc',
        '../../base/threading/thread_checker_unittest.cc',
        '../../base/threading/thread_collision_warner_unittest.cc',
        '../../base/threading/thread_id_name_manager_unittest.cc',
        '../../base/threading/thread_local_storage_unittest.cc',
        '../../base/threading/thread_local_unittest.cc',
        '../../base/threading/thread_unittest.cc',
        '../../base/threading/watchdog_unittest.cc',
        '../../base/threading/worker_pool_posix_unittest.cc',
        '../../base/threading/worker_pool_unittest.cc',
        '../../base/time_unittest.cc',
        '../../base/time_win_unittest.cc',
        '../../base/timer_unittest.cc',
        '../../base/tools_sanity_unittest.cc',
        '../../base/tracked_objects_unittest.cc',
        '../../base/tuple_unittest.cc',
        '../../base/values_unittest.cc',
        '../../base/version_unittest.cc',
        '../../base/vlog_unittest.cc',
        '../../base/win/dllmain.cc',
        '../../base/win/enum_variant_unittest.cc',
        '../../base/win/event_trace_consumer_unittest.cc',
        '../../base/win/event_trace_controller_unittest.cc',
        '../../base/win/event_trace_provider_unittest.cc',
        '../../base/win/i18n_unittest.cc',
        '../../base/win/iunknown_impl_unittest.cc',
        '../../base/win/object_watcher_unittest.cc',
        '../../base/win/pe_image_unittest.cc',
        '../../base/win/registry_unittest.cc',
        '../../base/win/sampling_profiler_unittest.cc',
        '../../base/win/scoped_bstr_unittest.cc',
        '../../base/win/scoped_comptr_unittest.cc',
        '../../base/win/scoped_handle_unittest.cc',
        '../../base/win/scoped_process_information_unittest.cc',
        '../../base/win/shortcut_unittest.cc',
        '../../base/win/startup_information_unittest.cc',
        '../../base/win/scoped_variant_unittest.cc',
        '../../base/win/win_util_unittest.cc',
        '../../base/win/wrapped_window_proc_unittest.cc',
      ],
      'dependencies': [
        '../../base/base_bin.gyp:base_bin',
        '../../base/base_bin.gyp:base_i18n_bin',
        '../../base/base_bin.gyp:base_prefs_bin',
        '../../base/base_bin.gyp:base_prefs_test_support_bin',
        '../../base/base_bin.gyp:base_static_bin',
        '../../base/base_bin.gyp:run_all_unittests_bin',
        '../../base/base_bin.gyp:test_support_base_bin',
        '../../base/third_party/dynamic_annotations/dynamic_annotations.gyp:dynamic_annotations',
        '../../testing/gmock.gyp:gmock',
        '../../testing/gtest.gyp:gtest',
        '../../third_party/icu_bin.gyp:icui18n_bin',
        '../../third_party/icu_bin.gyp:icuuc_bin',
      ],
      'includes': ['../../build/nocompile.gypi'],
      'variables': {
         # TODO(ajwong): Is there a way to autodetect this?
        'module_dir': 'base'
      },
      'conditions': [
        ['OS == "android"', {
          'dependencies': [
            'android/jni_generator/jni_generator.gyp:jni_generator_tests',
          ],
          'conditions': [
            ['gtest_target_type == "shared_library"', {
              'dependencies': [
                '../testing/android/native_test.gyp:native_test_native_code',
              ],
            }],
          ],
          'sources!': [
            # Broken on Android, and already disabled there.
            'debug/stack_trace_unittest.cc',
          ],
        }],
        ['OS == "ios" and _toolset != "host"', {
          'sources/': [
            # Only test the iOS-meaningful portion of process_utils.
            ['exclude', '^process_util_unittest'],
            ['include', '^process_util_unittest_ios\\.cc$'],
            # Requires spawning processes.
            ['exclude', '^metrics/stats_table_unittest\\.cc$'],
            # iOS does not use message_pump_libevent.
            ['exclude', '^message_pump_libevent_unittest\\.cc$'],
          ],
          'conditions': [
            ['coverage != 0', {
              'sources!': [
                # These sources can't be built with coverage due to a toolchain
                # bug: http://openradar.appspot.com/radar?id=1499403
                'json/json_reader_unittest.cc',
                'strings/string_piece_unittest.cc',

                # These tests crash when run with coverage turned on due to an
                # issue with llvm_gcda_increment_indirect_counter:
                # http://crbug.com/156058
                'debug/trace_event_unittest.cc',
                'debug/trace_event_unittest.h',
                'logging_unittest.cc',
                'string_util_unittest.cc',
                'test/trace_event_analyzer_unittest.cc',
                'utf_offset_string_conversions_unittest.cc',
              ],
            }],
          ],
          'actions': [
            {
              'action_name': 'copy_test_data',
              'variables': {
                'test_data_files': [
                  'test/data',
                ],
                'test_data_prefix': 'base',
              },
              'includes': [ '../../build/copy_test_data_ios.gypi' ],
            },
          ],
        }],
        ['use_glib==1', {
          'sources!': [
            'file_version_info_unittest.cc',
          ],
          'conditions': [
            [ 'linux_use_tcmalloc==1', {
                'dependencies': [
                  'allocator/allocator.gyp:allocator',
                ],
              },
            ],
            [ 'toolkit_uses_gtk==1', {
              'sources': [
                'nix/xdg_util_unittest.cc',
              ],
              'dependencies': [
                '../build/linux/system.gyp:gtk',
              ]
            }],
          ],
          'dependencies': [
            '../build/linux/system.gyp:glib',
            '../build/linux/system.gyp:ssl',
            '../tools/xdisplaycheck/xdisplaycheck.gyp:xdisplaycheck',
          ],
        }, {  # use_glib!=1
          'sources!': [
            '../../base/message_pump_glib_unittest.cc',
          ]
        }],
        ['use_ozone == 1', {
          'sources!': [
            'message_pump_glib_unittest.cc',
          ]
        }],
        # This is needed to trigger the dll copy step on windows.
        # TODO(mark): This should not be necessary.
        ['OS == "win"', {
          'dependencies': [
            '../../third_party/icu_bin.gyp:icudata_bin',
          ],
          'sources!': [
            '../../base/file_descriptor_shuffle_unittest.cc',
            '../../base/files/dir_reader_posix_unittest.cc',
            '../../base/threading/worker_pool_posix_unittest.cc',
            '../../base/message_pump_libevent_unittest.cc',
          ],
          # TODO(jschuh): crbug.com/167187 fix size_t to int truncations.
          'msvs_disabled_warnings': [
            4267,
          ],
        }, {  # OS != "win"
          'dependencies': [
            '../third_party/libevent/libevent.gyp:libevent'
          ],
          'sources/': [
            ['exclude', '^win/'],
          ],
          'sources!': [
            'debug/trace_event_win_unittest.cc',
            'time_win_unittest.cc',
            'win/win_util_unittest.cc',
          ],
        }],
        ['use_system_nspr==1', {
          'dependencies': [
            'third_party/nspr/nspr.gyp:nspr',
          ],
        }],
      ],  # conditions
      'target_conditions': [
        ['OS == "ios" and _toolset != "host"', {
          'sources/': [
            # Pull in specific Mac files for iOS (which have been filtered out
            # by file name rules).
            ['include', '^mac/objc_property_releaser_unittest\\.mm$'],
            ['include', '^mac/bind_objc_block_unittest\\.mm$'],
            ['include', '^sys_string_conversions_mac_unittest\\.mm$'],
          ],
        }],
      ],  # target_conditions
    },
  ],
}