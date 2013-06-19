{
  'target_defaults': {
    'variables': {
      'mfc_target': 0,
    },
    'target_conditions': [
      ['mfc_target==1', {
        'include_dirs': [
          # so can ignore first line #include "stdafx.h"
          'app/',
        ],
   
        'msvs_precompiled_header': 'app/stdafx.h',
        'msvs_precompiled_source': 'app/stdafx.cc',
        
        'msvs_configuration_attributes': {
          'conditions': [
            ['component=="shared_library"', {
              'UseOfMFC': '2',  # Shared DLL
            },{
              'UseOfMFC': '1',  # Static
            }],
          ],
        },
        'msvs_settings': {
          'VCLinkerTool': {
            #   2 == /SUBSYSTEM:WINDOWS
            'SubSystem': '2',
          },
        },      
      }],
    ],
  },  
}