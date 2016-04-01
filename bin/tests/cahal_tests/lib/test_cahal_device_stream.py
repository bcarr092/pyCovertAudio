import cahal_tests
import unittest
import string
import types

class TestsCAHALDeviceStream( unittest.TestCase ):
  def test_print_cahal_device_stream_list( self ):
    cahal_tests.cahal_print_device_stream_list( None )

    device_list = cahal_tests.cahal_get_device_list()
    index       = 0;                                                                    
    device      = cahal_tests.cahal_device_list_get( device_list, index )               
                                                                                
    while( device ):                                                                
      self.assertNotEqual( device, None )

      cahal_tests.cahal_print_device_stream_list( device.device_streams )

      index += 1                                                                    

      device = cahal_tests.cahal_device_list_get( device_list, index )

  def test_print_cahal_device_stream( self ):
    cahal_tests.cahal_print_device_stream( None )

    device_list = cahal_tests.cahal_get_device_list()
    index       = 0;                                                                    
    device      = cahal_tests.cahal_device_list_get( device_list, index )               
                                                                                
    while( device ):                                                                
      self.assertNotEqual( device, None )

      stream_index  = 0                                                             
      stream        =                               \
        cahal_tests.cahal_device_stream_list_get  ( \
          device.device_streams,                    \
          stream_index                              \
                                                  )
                                                                                
      while( stream ):                                                              
        cahal_tests.cahal_print_device_stream( stream )

        stream_index += 1
        stream        =                               \
          cahal_tests.cahal_device_stream_list_get  ( \
            device.device_streams,                    \
            stream_index                              \
                                                    )

      index += 1                                                                    

      device = cahal_tests.cahal_device_list_get( device_list, index )

if __name__ == '__main__':
  try:
    import threading as _threading
  except ImportError:
    import dummy_threading as _threading


  cahal_tests.cpc_log_set_log_level( cahal_tests.CPC_LOG_LEVEL_ERROR )

  cahal_tests.python_cahal_initialize()

  unittest.main()

  cahal_tests.cahal_terminate()
