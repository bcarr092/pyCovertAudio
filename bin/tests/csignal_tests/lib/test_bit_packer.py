from csignal_tests import *
from array import array
import random
import struct
import socket
import ctypes
import unittest

class TestsBitPacker( unittest.TestCase ):
  def test_get_bytes_add_bits( self ):
    bitPacker = python_bit_packer_initialize()

    self.assertNotEquals( bitPacker, None )

    data = "\x12\x34\x56\x78"

    for datum in data:
      byte = ord( datum )

      self.assertEquals( bit_packer_add_bits( ( byte >> 4 ), 4, bitPacker ), CPC_ERROR_CODE_NO_ERROR )
      self.assertEquals( bit_packer_add_bits( byte, 4, bitPacker ), CPC_ERROR_CODE_NO_ERROR )

    string = python_bit_packer_get_bytes( bitPacker )

    self.assertNotEquals( string, None )
    self.assertEquals( len( string ), len( data ) )

    for index in range( len( string ) ):
      self.assertEquals( string[ index ], data[ index ] )

    for datum in data:
      byte = ord( datum )

      self.assertEquals( bit_packer_add_bits( ( byte >> 6 ), 2, bitPacker ), CPC_ERROR_CODE_NO_ERROR )
      self.assertEquals( bit_packer_add_bits( ( byte >> 4 ), 2, bitPacker ), CPC_ERROR_CODE_NO_ERROR )
      self.assertEquals( bit_packer_add_bits( ( byte >> 2 ), 2, bitPacker ), CPC_ERROR_CODE_NO_ERROR )
      self.assertEquals( bit_packer_add_bits( byte, 2, bitPacker ), CPC_ERROR_CODE_NO_ERROR )
      
    string = python_bit_packer_get_bytes( bitPacker )

    self.assertNotEquals( string, None )
    self.assertEquals( len( string ), len( data ) * 2 )

    for index in range( len( data ) ):
      self.assertEquals( string[ index ], data[ index ] )

    for index in range( len( data ) ):
      self.assertEquals( string[ index + len( data ) ], data[ index ] )

    for datum in data:
      byte = ord( datum )

      self.assertEquals( bit_packer_add_bits( ( byte >> 7 ), 1, bitPacker ), CPC_ERROR_CODE_NO_ERROR )
      self.assertEquals( bit_packer_add_bits( ( byte >> 6 ), 1, bitPacker ), CPC_ERROR_CODE_NO_ERROR )
      self.assertEquals( bit_packer_add_bits( ( byte >> 5 ), 1, bitPacker ), CPC_ERROR_CODE_NO_ERROR )
      self.assertEquals( bit_packer_add_bits( ( byte >> 4 ), 1, bitPacker ), CPC_ERROR_CODE_NO_ERROR )
      self.assertEquals( bit_packer_add_bits( ( byte >> 3 ), 1, bitPacker ), CPC_ERROR_CODE_NO_ERROR )
      self.assertEquals( bit_packer_add_bits( ( byte >> 2 ), 1, bitPacker ), CPC_ERROR_CODE_NO_ERROR )
      self.assertEquals( bit_packer_add_bits( ( byte >> 1 ), 1, bitPacker ), CPC_ERROR_CODE_NO_ERROR )
      self.assertEquals( bit_packer_add_bits( byte, 1, bitPacker ), CPC_ERROR_CODE_NO_ERROR )

    string = python_bit_packer_get_bytes( bitPacker )

    self.assertNotEquals( string, None )
    self.assertEquals( len( string ), len( data ) * 3 )

    for index in range( len( data ) ):
      self.assertEquals( string[ index ], data[ index ] )

    for index in range( len( data ) ):
      self.assertEquals( string[ index + len( data ) ], data[ index ] )

    for index in range( len( data ) ):
      self.assertEquals( string[ index + ( 2 * len( data ) ) ], data[ index ] )

    self.assertEquals (
      bit_packer_destroy( bitPacker ),
      CPC_ERROR_CODE_NO_ERROR
                      )

  def test_get_bytes( self ):
    bitPacker = python_bit_packer_initialize()

    self.assertNotEquals( bitPacker, None )

    data = "Hello"

    self.assertEquals (
      python_bit_packer_add_bytes (
        data,
        bitPacker
                                  ),
      CPC_ERROR_CODE_NO_ERROR
                    )

    string = python_bit_packer_get_bytes( bitPacker )

    self.assertNotEquals( string, None )
    self.assertEquals( len( string ), len( data ) )

    for index in range( len( string ) ):
      self.assertEquals( string[ index ], data[ index ] )

    self.assertEquals (
      bit_packer_destroy( bitPacker ),
      CPC_ERROR_CODE_NO_ERROR
                      )

    bitPacker = python_bit_packer_initialize()

    data = struct.pack( "I", 1722 )

    self.assertEquals (
      python_bit_packer_add_bytes (
        data,
        bitPacker
                                  ),
      CPC_ERROR_CODE_NO_ERROR
                    )

    string = python_bit_packer_get_bytes( bitPacker )

    self.assertNotEquals( string, None )
    self.assertEquals( len( string ), len( data ) )

    for index in range( len( string ) ):
      self.assertEquals( string[ index ], data[ index ] )

    self.assertEquals (
      bit_packer_destroy( bitPacker ),
      CPC_ERROR_CODE_NO_ERROR
                      )

    bitPacker = python_bit_packer_initialize()

    data = ""

    for index in range( 100 ):
      value = 32767 * random.normalvariate( 0, 1 )
      value = socket.htonl( struct.unpack( "I", struct.pack( "i", int( value ) ) )[ 0 ] )

      if( 0 > value ):
        value = struct.pack( "i", value )
        value = struct.unpack( "I", value )[ 0 ]

      value = struct.pack( "I", value )
      data  = data + value

    self.assertEquals (
      python_bit_packer_add_bytes (
        data,
        bitPacker
                                  ),
      CPC_ERROR_CODE_NO_ERROR
                    )

    string = python_bit_packer_get_bytes( bitPacker )

    self.assertNotEquals( string, None )
    self.assertEquals( len( string ), len( data ) )

    for index in range( len( string ) ):
      self.assertEquals( string[ index ], data[ index ] )

    self.assertEquals (
      bit_packer_destroy( bitPacker ),
      CPC_ERROR_CODE_NO_ERROR
                      )

  def test_bit_packer_negative( self ):
    self.assertNotEquals( bit_packer_destroy( None ), CPC_ERROR_CODE_NO_ERROR )

    dataBuffer = array( 'B', "1" )

    self.assertNotEquals( bit_packer_add_bits( dataBuffer[ 0 ], 8, None ), CPC_ERROR_CODE_NO_ERROR )

    self.assertNotEquals( python_bit_packer_add_bytes( "1", None ), CPC_ERROR_CODE_NO_ERROR )
    self.assertNotEquals( python_bit_packer_add_bytes( 1, None ), CPC_ERROR_CODE_NO_ERROR )

    bitPacker = python_bit_packer_initialize()

    self.assertNotEquals( bitPacker, None )

    self.assertNotEquals( bit_packer_add_bits( dataBuffer[ 0 ], 12, None ), CPC_ERROR_CODE_NO_ERROR )

    self.assertNotEquals( python_bit_packer_add_bytes( None, bitPacker ), CPC_ERROR_CODE_NO_ERROR )

    self.assertEquals( bit_packer_destroy( bitPacker ), CPC_ERROR_CODE_NO_ERROR )

  def test_add_bits( self ):
    bitPacker = python_bit_packer_initialize()

    self.assertNotEquals( bitPacker, None )

    dataBuffer = array( 'B', "\x12\x34\x56\x78" )

    for byte in dataBuffer:
      self.assertEquals( bit_packer_add_bits( ( byte >> 4 ), 4, bitPacker ), CPC_ERROR_CODE_NO_ERROR )
      self.assertEquals( bit_packer_add_bits( byte, 4, bitPacker ), CPC_ERROR_CODE_NO_ERROR )

    for byte in dataBuffer:
      self.assertEquals( bit_packer_add_bits( ( byte >> 6 ), 2, bitPacker ), CPC_ERROR_CODE_NO_ERROR )
      self.assertEquals( bit_packer_add_bits( ( byte >> 4 ), 2, bitPacker ), CPC_ERROR_CODE_NO_ERROR )
      self.assertEquals( bit_packer_add_bits( ( byte >> 2 ), 2, bitPacker ), CPC_ERROR_CODE_NO_ERROR )
      self.assertEquals( bit_packer_add_bits( byte, 2, bitPacker ), CPC_ERROR_CODE_NO_ERROR )

    for byte in dataBuffer:
      self.assertEquals( bit_packer_add_bits( ( byte >> 7 ), 1, bitPacker ), CPC_ERROR_CODE_NO_ERROR )
      self.assertEquals( bit_packer_add_bits( ( byte >> 6 ), 1, bitPacker ), CPC_ERROR_CODE_NO_ERROR )
      self.assertEquals( bit_packer_add_bits( ( byte >> 5 ), 1, bitPacker ), CPC_ERROR_CODE_NO_ERROR )
      self.assertEquals( bit_packer_add_bits( ( byte >> 4 ), 1, bitPacker ), CPC_ERROR_CODE_NO_ERROR )
      self.assertEquals( bit_packer_add_bits( ( byte >> 3 ), 1, bitPacker ), CPC_ERROR_CODE_NO_ERROR )
      self.assertEquals( bit_packer_add_bits( ( byte >> 2 ), 1, bitPacker ), CPC_ERROR_CODE_NO_ERROR )
      self.assertEquals( bit_packer_add_bits( ( byte >> 1 ), 1, bitPacker ), CPC_ERROR_CODE_NO_ERROR )
      self.assertEquals( bit_packer_add_bits( byte, 1, bitPacker ), CPC_ERROR_CODE_NO_ERROR )

    self.assertEquals (
      bit_packer_destroy( bitPacker ),
      CPC_ERROR_CODE_NO_ERROR
                      )

  def test_add_bytes( self ):
    bitPacker = python_bit_packer_initialize()

    self.assertNotEquals( bitPacker, None )

    data = "Hello"

    self.assertEquals (
      python_bit_packer_add_bytes (
        data,
        bitPacker
                                  ),
      CPC_ERROR_CODE_NO_ERROR
                    )

    data = struct.pack( "I", 1722 )

    self.assertEquals (
      python_bit_packer_add_bytes (
        data,
        bitPacker
                                  ),
      CPC_ERROR_CODE_NO_ERROR
                    )

    data = ""

    for index in range( 100 ):
      value = 32767 * random.normalvariate( 0, 1 )
      value = struct.pack( "i", int( value ) )
      data  = data + value

    self.assertEquals (
      python_bit_packer_add_bytes (
        data,
        bitPacker
                                  ),
      CPC_ERROR_CODE_NO_ERROR
                    )

    self.assertEquals (
      bit_packer_destroy( bitPacker ),
      CPC_ERROR_CODE_NO_ERROR
                      )

  def test_initialize_destroy( self ):
    bitPacker = python_bit_packer_initialize()

    self.assertNotEquals( bitPacker, None )

    self.assertEquals (
      bit_packer_destroy( bitPacker ),
      CPC_ERROR_CODE_NO_ERROR
                      )

if __name__ == '__main__':
  cpc_log_set_log_level( CPC_LOG_LEVEL_ERROR )

  csignal_initialize()

  unittest.main()

  csignal_terminate()
