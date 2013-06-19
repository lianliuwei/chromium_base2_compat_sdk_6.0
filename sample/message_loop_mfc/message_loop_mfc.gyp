{
  'variables': {
    'chromium_code': 1,
  },
   'includes': [
    'mfc.gypi',
  ],
  'targets': [
    {
      'target_name': 'message_loop_mfc',
      'type': 'executable',
      'variables': {
        'mfc_target': 1,
      },
      'dependencies': [
        '../../base/base.gyp:base',
        '../../base_ex/base_ex.gyp:base_ex',
      ],
      'include_dirs': [
        '.',
      ], 
      'sources': [
        'app/app.h',
        'app/app.cc',
        'app/stdafx.h',
        'app/stdafx.cc',
        'app/target_version.h',
        'ui/main_frame.h',
        'ui/main_frame.cc',
        'ui/test_view.h',
        'ui/test_view.cc',
        'ui/form_view_ex.h',
        'common/mfc_thread.h',
        'common/mfc_thread.cc',
        'resources/resource.h',
        'resources/app.ico',
        'resources/main_frame.rc',
        'resources/test_view.rc',
        'resources/mfc_res.rc',
        'resources/version.rc',
      ], 
    },
  ],
}