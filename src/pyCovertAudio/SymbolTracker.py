from pyCovertAudio_lib import *

import struct

class SymbolTracker:
  def __init__( self, bitsPerSymbol, data ):
    self.data           = data
    self.bitsPerSymbol  = bitsPerSymbol

    self.dataStream = python_bit_stream_initialize( False, self.data )

  def __del__( self ):
    bit_stream_destroy( self.dataStream )

  def getSize( self ):
    return( bit_stream_get_number_of_remaining_bits( self.dataStream ) )

  def getNextSymbol( self ):
    symbol = None

    ( numberOfBits, buffer ) = \
      python_bit_stream_get_bits( self.dataStream, self.bitsPerSymbol ) 

    if( 0 < numberOfBits ):
      symbol = struct.unpack( "B", buffer )[ 0 ] >> ( 8 - self.bitsPerSymbol )   

    return( symbol )

  @staticmethod
  def toList( bitsPerSymbol, data ):
    returnList  = []
    dataStream  = SymbolTracker( bitsPerSymbol, data )
    symbol      = dataStream.getNextSymbol() 

    while( symbol != None ):
      returnList.append( symbol ) 

      symbol = dataStream.getNextSymbol()

    return( returnList )
