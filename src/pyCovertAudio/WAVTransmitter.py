import os

from pyCovertAudio_lib      import *
from BaseTransmitter  import BaseTransmitter
from Debug            import Debug

class WAVTransmitter( BaseTransmitter ):
  def __init__( self, configuration ):
    BaseTransmitter.__init__( self, configuration )

    try:
      wavInfo = configuration[ 'WAVInfo' ]

      if( os.path.isabs( wavInfo[ 'fileName' ] ) ):
        self.fileName = wavInfo[ 'fileName' ]
      else:
        self.fileName         = \
          os.path.join  (
            Debug.instance.getOutputDirectory(),
            wavInfo[ 'fileName' ]
                        )

      self.numberOfChannels = wavInfo[ 'numberOfChannels' ]
      self.channelMode      = wavInfo[ 'channelMode' ]
    except KeyError as e:
      print "ERROR: Could not read key value %s." %( str( e ) )

  def toString( self ):
    return  (
      "WAV info:\n\tFile name:\t\t'%s'\n\tNumber of channels:\t%d\n\t"
      "Channel mode:\t\t'%s'\n%s" \
        %(
          self.fileName,
          self.numberOfChannels,
          self.channelMode,
          BaseTransmitter.toString( self )
        )
            )

  def transmit( self ):
    signal    = BaseTransmitter.transmit( self )
    maxValue  = 0

    for sample in signal:
      if( abs( sample ) > maxValue ):
        maxValue = abs( sample )

    signal = map( lambda x: ( x / maxValue ), signal )

    outputSignal = []

    outputSignal.append( signal )

    for i in range( self.numberOfChannels - 1 ):
      if( "copy" == self.channelMode ):
        outputSignal.append( signal )
      elif( "blank" == self.channelMode ):
        blankSignal = [ 0.0 for i in range( len( signal ) ) ]

        outputSignal.append( blankSignal )
      else:
        print "ERROR: Unknown channel mode %s." %( self.channelMode )

    result =  \
      python_write_LPCM_wav (
        self.fileName,
        len( outputSignal ),
        self.sampleRate,
        len( signal ),
        outputSignal
                            )

    if( not result ):
      print "ERROR: Could not write WAV samples to '%s'." \
        %( self.fileName )
