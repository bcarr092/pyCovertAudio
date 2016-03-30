import wave
import math

from pyCovertAudio_lib import *

bufferIndex = 0
bufferedSamples = ""


def playback(in_device, in_buffer_length):
    global bufferIndex

    data = ''

    bufferLength = len(bufferedSamples)
    lastIndex = bufferIndex + in_buffer_length

    if(lastIndex > bufferLength):
        lastIndex = bufferLength + 1

    data = bufferedSamples[bufferIndex: lastIndex]

    bufferIndex += in_buffer_length

    return(data)


class WAVPlayer:

    def __init__(self, fileName, volume, delay):
        self.fileName = fileName
        self.volume = volume
        self.delay = delay
        self.initialized = False

        self.readWAVFile()

    def readWAVFile(self):
        global bufferedSamples

        try:
            self.wavFile = wave.open(self.fileName, "rb")

            frameSize = \
                self.wavFile.getnchannels() * self.wavFile.getsampwidth()

            print "WAV file information:"
            print "\tFile:\t\t\t'%s'" % (self.fileName)
            print "\tNumber of channels:\t%d" % (self.wavFile.getnchannels())
            print "\tBit depth:\t\t%d bits" % (self.wavFile.getsampwidth() * 8)
            print "\tSample rate:\t\t%d Hz" % (self.wavFile.getframerate())
            print "\tFrame size:\t\t%d B" % (frameSize)
            print "\tNumber of frames:\t%d" % (self.wavFile.getnframes())
            print "\tDuration:\t\t%d secs"  \
                % (
                  math.ceil(
                      (self.wavFile.getnframes() * 1.0) /
                      (self.wavFile.getframerate() * 1.0)
                  )
                )

            delayFrames = self.delay * self.wavFile.getframerate()
            delayBytes = \
                delayFrames * self.wavFile.getnchannels() * \
                self.wavFile.getsampwidth()

            bufferedSamples = "\x00" * int(math.ceil(delayBytes))

            for i in range(0, self.wavFile.getnframes(), 10000):
                samples = self.wavFile.readframes(10000)

                bufferedSamples = bufferedSamples + samples

            self.wavFile.close()
        except wave.Error:
            print "ERROR: Could not open %s for read." % (self.fileName)

    def initPlayback(self, device):
        if (
            device.hasAppropriateStream(
                CAHAL_DEVICE_OUTPUT_STREAM,
                self.wavFile.getnchannels(),
                self.wavFile.getsampwidth() * 8,
                self.wavFile.getframerate()
            )
        ):
            flags =                                   \
                CAHAL_AUDIO_FORMAT_FLAGISSIGNEDINTEGER  \
                | CAHAL_AUDIO_FORMAT_FLAGISPACKED

            if (
                    start_playback(
                        device.struct,
                    CAHAL_AUDIO_FORMAT_LINEARPCM,
                    self.wavFile.getnchannels(),
                    self.wavFile.getframerate(),
                    self.wavFile.getsampwidth() * 8,
                    self.volume,
                    playback,
                    flags
                    )
            ):
                print "Playback initialized..."

                self.initialized = True
            else:
                print "ERROR: Could not start playing."
        else:
            print "ERROR: Could not find an appropriate stream."

        return(self.initialized)

    def play(self, device, duration):
        if(self.initialized):
            print "Starting playback..."

            cahal_sleep(int(math.ceil((duration + self.delay) * 1000)))

            print "Stopping playback..."

            cahal_stop_playback()

            print "Stopped playback."
        else:
            print "ERROR: Attempting to play from uninitialized player."
