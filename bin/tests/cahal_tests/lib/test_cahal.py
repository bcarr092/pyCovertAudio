import cahal_tests
import unittest
import string
import types

class TestsCAHAL( unittest.TestCase ):
  def test_get_version_string( self ):
    version = cahal_tests.cahal_get_version_string()
    
    try:
      self.assertIsInstance( version, types.StringType )
    except AttributeError:
      pass

    self.assertNotEqual( version, '' )

  def test_get_version( self ):
    version = cahal_tests.cahal_get_version() 

    try:
      self.assertIsInstance( version, types.LongType )
    except AttributeError:
      pass

    self.assertEqual( version, 256 )

    major = ( version >> 8 ) & 0x00FF
    minor = version & 0x00FF

    self.assertEqual( major, 1 )
    self.assertEqual( minor, 0 )


if __name__ == '__main__':
  try:
    import threading as _threading
  except ImportError:
    import dummy_threading as _threading

  cahal_tests.cpc_log_set_log_level( cahal_tests.CPC_LOG_LEVEL_ERROR )

  cahal_tests.python_cahal_initialize()

  unittest.main()
  
  cahal_tests.cahal_terminate()
