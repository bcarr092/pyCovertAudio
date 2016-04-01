try:
  import threading as _threading
except ImportError:
  import dummy_threading as _threading

import cahal_tests                                                              
import unittest                                                                 
import sys
                                                                                
from test_cahal                           import TestsCAHAL
from test_cahal_device                    import TestsCAHALDevice
from test_cahal_device_stream             import TestsCAHALDeviceStream
from test_cahal_audio_format_description  import  \
  TestsCAHALAudioFormatDescription

cahal_tests.cpc_log_set_log_level( cahal_tests.CPC_LOG_LEVEL_NO_LOGGING )

cahal_tests.python_cahal_initialize()

alltests = unittest.TestSuite ( [                                                   \
 unittest.TestLoader().loadTestsFromTestCase( TestsCAHAL ),                         \
 unittest.TestLoader().loadTestsFromTestCase( TestsCAHALDevice ),                   \
 unittest.TestLoader().loadTestsFromTestCase( TestsCAHALDeviceStream ),             \
 unittest.TestLoader().loadTestsFromTestCase  (                                     \
  TestsCAHALAudioFormatDescription                                                  \
                                              )                                     \
                                ] )

result = unittest.TextTestRunner( verbosity=2 ).run( alltests )

cahal_tests.cahal_terminate()

if( len( result.errors ) == 0 and len( result.failures ) == 0 ):
    sys.exit( 0 )
else:
    sys.exit( -1 )
