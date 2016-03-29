from BaseReceiver     import BaseReceiver
from Debug            import Debug
from SignalFunctions  import SignalFunctions

import wave
import struct

class WAVReceiver( BaseReceiver ):
  def __init__( self, configuration ):
    BaseReceiver.__init__( self, configuration )

    try:
      wavInfo = configuration[ 'WAVInfo' ]

      self.fileName = wavInfo[ 'fileName' ]
    except KeyError as e:
      print "ERROR: Could not read key value %s." %( str( e ) ) 

  def receiveSignal( self ):
    signal = []

    try:
      wav = wave.open( self.fileName, 'r' )

      if( wav ):
        print "Received file:\t\t%s" %( self.fileName )
        print "Channels:\t\t%d" %( wav.getnchannels() )
        print "Sample width:\t\t%d" %( wav.getsampwidth() )
        print "Sample rate:\t\t%d" %( wav.getframerate() )
        print "Number of samples:\t%d" %( wav.getnframes() )

        if( self.sampleRate != wav.getframerate() ):
          print "ERROR: Mismatch sample rate. Got %d, expected %d." \
            %( wav.getframerate(), self.sampleRate )

          return( None )

        if( wav.getsampwidth() == 1 ):
          signal = \
            list  (
              struct.unpack (
                "b" * wav.getnframes() * wav.getnchannels(),
                wav.readframes( wav.getnframes() )
                            )
                  )
        elif( wav.getsampwidth() == 2 ):
          signal = \
            list  (
              struct.unpack (
                "h" * wav.getnframes() * wav.getnchannels(),
                wav.readframes( wav.getnframes() )
                            )
                  )
        elif( wav.getsampwidth() == 4 ):
          signal = \
            list  (
              struct.unpack (
                "i" * wav.getnframes() * wav.getnchannels(),
                wav.readframes( wav.getnframes() )
                            )
                  )
        else:
          print "ERROR: Could not handle sample width %d."  \
            %( wav.getsampwidth() )

        signal  = signal[ : : wav.getnchannels() ]
        signal  = SignalFunctions.normalizeSignal( signal )

        wav.close()
    except wave.Error as e:
      print "ERROR: Could not open '%s': %s" %( self.fileName, str( e ) )

    return( signal )
