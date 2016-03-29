from BaseEncoder import BaseEncoder
from SymbolTracker  import SymbolTracker
from BitPacker import BitPacker
from BitStream import BitStream

import math

class ManchesterEncoder( BaseEncoder ):
  BLOCK_LENGTH    = 2
  MESSAGE_LENGTH  = 1

  HI_SYMBOL       = 0b10
  LO_SYMBOL       = 0b01

  def __init__( self, configuration ):
    configuration[ "blockLength" ]    = ManchesterEncoder.BLOCK_LENGTH
    configuration[ "messageLength" ]  = ManchesterEncoder.MESSAGE_LENGTH

    BaseEncoder.__init__( self, configuration )

  def toString( self ):
    string = "ManchesterEncoder\n%s"  \
      %(
        BaseEncoder.toString( self )
      )
    
    return( string )

  def encode( self, data ):
    if( self.checkParamterIsString( data ) ):
      bitPacker       = BitPacker()
      bitStream       = BitStream( False, bitPacker )    
      symbolTracker   = SymbolTracker( ManchesterEncoder.MESSAGE_LENGTH, data )
      symbol          = symbolTracker.getNextSymbol()
  
      while( symbol != None ):
        if( symbol == 1 ):
          bitPacker.writeByte (
            ManchesterEncoder.HI_SYMBOL,
            ManchesterEncoder.BLOCK_LENGTH
                              )
        elif( symbol == 0 ):
          bitPacker.writeByte (
            ManchesterEncoder.LO_SYMBOL,
            ManchesterEncoder.BLOCK_LENGTH
                              )
        else:
          print "ERROR: Received unknown symbol (%d)." %( symbol )
  
        symbol = symbolTracker.getNextSymbol()
  
      return( bitStream.getRawBytes() )
    else:
      return( None )
  
  def decode( self, data, errorPositions=None ):
    if( self.checkParamterIsString( data ) ):
      bitPacker         = BitPacker()
      bitStream         = BitStream( False, bitPacker )
      symbolTracker     = SymbolTracker( ManchesterEncoder.BLOCK_LENGTH, data )
      symbol            = symbolTracker.getNextSymbol()
      errorBitPositions = []
  
      while( symbol != None ):
        if( symbol == ManchesterEncoder.HI_SYMBOL ):
          bitPacker.writeByte( 1, 1 )
        elif( symbol == ManchesterEncoder.LO_SYMBOL ):
          bitPacker.writeByte( 0, 1 )
        else:
          print "WARN: Received unknown symbol (0x%x) at position %d." %( symbol, bitStream.getSize() )

          errorBitPositions.append( bitStream.getSize() )

          bitPacker.writeByte( 1, 1 )
  
        symbol = symbolTracker.getNextSymbol()

      buffer = bitStream.getRawBytes()

      if( errorPositions is not None ):
        for i in range( len( buffer ) ):
          errorPositions.append( False )
        for bitPosition in errorBitPositions:
          bytePosition = int( math.floor( bitPosition / 8 ) )

          errorPositions[ bytePosition ] = True
  
      return( buffer )
    else:
      return( None )
