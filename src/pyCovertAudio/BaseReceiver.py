from ModulatorFactory import ModulatorFactory
from ModifierFactory  import ModifierFactory
from TestFactory      import TestFactory
from Base             import Base
from Debug            import Debug
from BitPacker        import BitPacker
from BitStream        import BitStream
from SymbolTracker    import SymbolTracker

import sys
import dataset
import pickle
import time
import math

from pyCovertAudio_lib  import *

class BaseReceiver( Base ):
  def toString( self ):
    string =  \
      "Base receiver:\n\tSample rate:\t\t%d Hz\n\tBits per symbol:\t" \
      "%d bits\nDemodulator:\n%s"  \
        %(
          self.sampleRate,
          self.bitsPerSymbol,
          self.dataDemodulator.toString()
        )

    string += "Modifiers:\n"

    for modifier in self.modifiers:
      string += modifier.toString()

    return( string )

  def __init__( self, configuration ):
    try:
      Base.__init__( self, configuration )

      self.modifiers   = []

      self.dataDemodulator  =  \
        ModulatorFactory.create (
          self.bitsPerSymbol,
          self.sampleRate,
          self.samplesPerSymbol,
          self.symbolExpansionFactor,
          self.separationIntervals,
          configuration[ 'demodulator' ]
                                )

      ModifierFactory.initializeModifiers (
        self.sampleRate,
        configuration[ 'modifiers' ],
        self.modifiers
                                          )

    except KeyError as e:
      print "ERROR: Could not read key %s." %( str( e ) )

  def handleModifiers( self, signal ):
    for modifier in self.modifiers:
      signal = modifier.modify( signal )

      offset = modifier.getOffset()

      signal = signal[ offset : ]

    Debug.instance.debugSignal( 'modified.WAV', signal, self.sampleRate )

    return( signal )

  def handleDemodulation( self, signal ):
    sampleOffset  = None
    symbols       = self.dataDemodulator.demodulate( signal )
    offsetSymbol  = self.locateData( symbols )

    if( offsetSymbol is not None ):
      print "Signal starts at symbol %d." %( offsetSymbol )
  
      sampleOffset =  \
        ( offsetSymbol + 1 )  \
        * self.samplesPerSymbol \
        * self.symbolExpansionFactor

      print "Sample offset is %d." %( sampleOffset )

      decodedSymbols = self.extractSymbols( symbols, ( offsetSymbol  + 1 ) )

    return( symbols, sampleOffset )

  def receive( self ):
    offsetSignal  = None
    signal        = self.receiveSignal()

    Debug.instance.debugSignal( 'received.WAV', signal, self.sampleRate )   

    signal = self.handleModifiers( signal )

    demodulationStartTime = time.time()

    ( symbols, sampleOffset ) = self.handleDemodulation( signal )

    runningTime = time.time() - demodulationStartTime

    print "Demodulation time: %.04f" %( runningTime )
  
  def locateData( self, symbols ):
    maxIndex    = 0
    convolution = \
      python_convolve (
        [ 1.0 if( x ) else -1.0 for x in symbols ],
        [ 1.0 if( x ) else -1.0 for x in reversed( self.sentinelSymbols ) ]
                      )

    for i in range( len( convolution ) ):
      if( convolution[ i ] > convolution[ maxIndex ] ):
        maxIndex = i

    if( maxIndex >= len( symbols ) ):
      print "WARN: Could no locate sentinel in data."

      return( None )

    return( maxIndex )

  def extractSymbols( self, symbols, offset ):
    bitPacker = BitPacker()
    bitStream = BitStream( False, bitPacker )

    for i in range( offset, len( symbols ) ):
      bitPacker.writeByte( symbols[ i ], 1 )

    data = bitStream.getRawBytes()

    print "Data (%d):" %( len( data ) )
    for i in range( len( data ) ):
      if( i != 0 and i % 8 == 0 ):
        print

      print "0x%02x " %( ord( data[ i ] ) ),
    print

    decodedData = self.decodeData( data )

    print "Decoded data (%d):" %( len( decodedData ) )
    for i in range( len( decodedData ) ):
      if( i != 0 and i % 8 == 0 ):
        print

      print "0x%02x " %( ord( decodedData[ i ] ) ),
    print

    print "Decoded string: '%s'" %( decodedData )

    decodedSymbols      = \
      SymbolTracker.toList( self.bitsPerSymbol, decodedData )

    return( decodedSymbols )

  def receiveSignal( self ):
    print "ERROR: Must be overridden."

    sys.exit( -1 )
