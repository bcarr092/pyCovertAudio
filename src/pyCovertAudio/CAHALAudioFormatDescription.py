from pyCovertAudio_lib import *

from CAHALSampleRateRange import CAHALSampleRateRange

class CAHALAudioFormatDescription:
  def __init__( self, audioFormat ):
    if( type( audioFormat ) is cahal_audio_format_description ):
      self.bitDepth         = audioFormat.bit_depth
      self.numberOfChannels = audioFormat.number_of_channels
      self.format           = audioFormat.format_id
      self.sampleRate       =                         \
        CAHALSampleRateRange  (                       \
          audioFormat.sample_rate_range.minimum_rate, \
          audioFormat.sample_rate_range.maximum_rate  \
                              )
    else:
      print "Device is not the correct instance: %s." %( type( audioFormat ) )

  
  def printMe( self, indent ):
    print "%s%s, %d channel(s), %d bits, %.02f Hz"    \
      %(                                              \
        indent,                                       \
        cahal_convert_audio_format_id_to_cstring  (   \
          self.format                                 \
                                                  ),  \
        self.numberOfChannels,                        \
        self.bitDepth,                                \
        self.sampleRate.minimum
      )
