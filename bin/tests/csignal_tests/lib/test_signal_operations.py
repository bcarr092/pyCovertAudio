from csignal_tests import *

import math
import unittest
import random
import struct
import copy

try:
  import fractions
except ImportError:
    print "ERROR: Could not import fractions, skipping some tests."

class TestsSignalOperations( unittest.TestCase ):
  def test_signal_sum( self ):
    cs_sum = python_csignal_sum_signal( [ 1.0 for i in range( 100 ) ], 1.0 )

    self.assertNotEquals( None, cs_sum )
    self.assertEquals( 100.0, cs_sum )

    cs_sum = python_csignal_sum_signal( [ 1.0 for i in range( 100 ) ], -1.0 )

    self.assertNotEquals( None, cs_sum )
    self.assertEquals( -100.0, cs_sum )

    cs_sum = python_csignal_sum_signal( [ 1.0 for i in range( 100 ) ], 2.0 )

    self.assertNotEquals( None, cs_sum )
    self.assertEquals( 200.0, cs_sum )

    for i in range( 1000 ):
      signal = [ random.normalvariate( 0, 1 ) for j in range( 100 ) ]

      py_sum = sum( signal )

      cs_sum = python_csignal_sum_signal( signal, 1.0 )

      self.assertNotEquals( None, cs_sum )
      self.assertEquals( py_sum, cs_sum )

      cs_sum = python_csignal_sum_signal( signal, -1.0 )

      self.assertNotEquals( None, cs_sum )
      self.assertEquals( -1.0 * py_sum, cs_sum )

      cs_sum = python_csignal_sum_signal( signal, 2.0 )

      self.assertNotEquals( None, cs_sum )
      self.assertEquals( 2.0 * py_sum, cs_sum )

  def test_signal_sum_negative( self ):
    cs_sum = python_csignal_sum_signal( None, 1.0 )

    self.assertEquals( None, cs_sum )

    cs_sum = python_csignal_sum_signal( [], 1.0 )

    self.assertNotEquals( None, cs_sum )
    self.assertEquals( 0.0, cs_sum )

  def test_energy( self ):
    test = [ 1.0 for i in range( 100 ) ]

    energy = python_csignal_calculate_energy( test )

    self.assertNotEquals( None, test )
    self.assertEquals( 100, energy )

    for _ in range( 1000 ):
      test = [ random.normalvariate( 0, 1 ) for i in range( 100 ) ]

      output = python_csignal_calculate_energy( test )

      self.assertNotEquals( output, None )

  def test_energy_negative( self ):
    energy = python_csignal_calculate_energy( None )

    self.assertEquals( None, energy )

  def test_multiply_signals_negative( self ):
    test = [ 1.0 ]

    output = python_csignal_multiply_signals( None, None )

    self.assertEquals( output, None )

    output = python_csignal_multiply_signals( test, None )

    self.assertEquals( output, None )

    output = python_csignal_multiply_signals( None, test )

    self.assertEquals( None, output )

    test2= [ 1.0, 2.0 ]

    output = python_csignal_multiply_signals( test, test2 )

    self.assertEquals( None, output )

  def test_multiply_signals( self ):
    test = [ 1.0 for i in range( 100 ) ]
    test2 = [ random.randint( 0, 10 ) * 1.0 for i in range( 100 ) ]

    output = python_csignal_multiply_signals( test, test2 )

    self.assertNotEquals( output, None )
    self.assertEquals( len( output ), len( test ) )
    self.assertEquals( len( output ), len( test2 ) )

    [ self.assertEquals( output[ i ], test2[ i ] ) for i in range( len( output ) ) ]

    for _ in range( 1000 ):
      test = [ random.normalvariate( 0, 1 ) for i in range( 100 ) ]
      test2 = [ random.normalvariate( 0, 1 ) for i in range( 100 ) ]

      output = python_csignal_multiply_signals( test, test2 )

      self.assertNotEquals( output, None )
      self.assertEquals( len( output ), len( test ) )
      self.assertEquals( len( output ), len( test2 ) )

if __name__ == '__main__':
  cpc_log_set_log_level( CPC_LOG_LEVEL_ERROR )

  csignal_initialize()

  unittest.main()

  csignal_terminate()
