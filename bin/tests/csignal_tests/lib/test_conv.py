from csignal_tests import *

import unittest
import random

class TestsConv( unittest.TestCase ):
  def test_triangle( self ):
    rect = [ 1.0, 1.0, 1.0, 1.0 ]

    output = python_convolve( rect, rect )

    for index in range( len( output ) ):
      if( index < int( len( output ) / 2 ) ):
        self.assertEquals( output[ index ], ( index + 1 ) )
      else:
        self.assertEquals( output[ index ], 7 - index )
  
  def test_length_random( self ):
    for _ in range( 1000 ):
      nImpulse = random.randint( 1, 10 )
      nInput = random.randint( 1, 200 )

      input = [ random.normalvariate( 0, 1 ) for i in range( nInput ) ]
      impulse = [ random.normalvariate( 0, 1 ) for i in range( nImpulse ) ]

      output = python_convolve( input, impulse )

      self.assertNotEquals( output, None )

      self.assertEquals( len( output ), nImpulse + nInput )

  def test_conv_basic( self ):
    input   = [ 1.0, 2.0, 3.0 ]
    impulse = [ 1.0 ]

    output = python_convolve( input, impulse )

    self.assertNotEquals( output, None )

    self.assertEquals( len( input ) + len( impulse ), len( output ) )

    for index in range( len( input ) ):
      self.assertEquals( input[ index ], output[ index ] )

  def test_conv_negative( self ):
    test = [ 1.0 ]

    output = python_convolve( None, None )

    self.assertEquals( output, None )

    output = python_convolve( test, None )

    self.assertEquals( output, None )

    output = python_convolve( None, test )

    self.assertEquals( output, None )

if __name__ == '__main__':
  cpc_log_set_log_level( CPC_LOG_LEVEL_ERROR )

  csignal_initialize()

  unittest.main()

  csignal_terminate()
