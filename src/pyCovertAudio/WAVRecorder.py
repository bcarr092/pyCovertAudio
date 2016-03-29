import wave
import md5

from pyCovertAudio_lib  import *
from Debug              import Debug

write           = False
bufferedSamples = ""

def bufferSamples( in_device, in_buffer, in_buffer_length ):                         
  global bufferedSamples

  if( write ):
    bufferedSamples = bufferedSamples + in_buffer

  return( "" )

class WAVRecorder:
  def __init__( self, fileName, numberOfChannels, bitDepth, sampleRate ):
    self.numberOfChannels = numberOfChannels
    self.bitDepth         = bitDepth
    self.sampleRate       = sampleRate
    self.initialized      = False

    self.fileName = Debug.instance.getOutputDirectory()+'/'+fileName

  def initRecord( self, device ):
    if  (                               \
      device.hasAppropriateStream (     \
            CAHAL_DEVICE_INPUT_STREAM,  \
            self.numberOfChannels,      \
            self.bitDepth,              \
            self.sampleRate             \
                                  )     \
        ):

      flags =                                   \
        CAHAL_AUDIO_FORMAT_FLAGISSIGNEDINTEGER  \
        | CAHAL_AUDIO_FORMAT_FLAGISPACKED

      if  (
        start_recording (               \
          device.struct,                \
          CAHAL_AUDIO_FORMAT_LINEARPCM, \
          self.numberOfChannels,        \
          self.sampleRate,              \
          self.bitDepth,                \
          bufferSamples,                \
          flags                         \
                        )               \
          ):
        print "Recording initialized..."

        self.initialized = True
      else:
        print "ERROR: Could not start recording."
    else:
      print "ERROR: Could not find an appropriate stream."

    return( self.initialized )

  def writeWAVFile( self ):
    try:
      wavFile = wave.open( self.fileName, "wb" )
        
      wavFile.setnchannels( self.numberOfChannels )
      wavFile.setsampwidth( self.bitDepth / 8 )
      wavFile.setframerate( self.sampleRate )

      wavFile.writeframes( bufferedSamples )

      wavFile.close()
    except wave.Error:
      print "ERROR: Could not open %s for write." %( self.fileName )

  def record( self, device, duration ):
    global write

    if( self.initialized ):
      print "Starting recording..."

      write = True

      cahal_sleep( duration * 1000 )

      print "Stopping recording..."

      cahal_stop_recording()

      print "Stopped recording."
    else:
      print "ERROR: Attempting to record from uninitialized recorder."

    self.writeWAVFile()
