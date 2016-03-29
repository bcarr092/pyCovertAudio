from pyCovertAudio_lib import *
from BaseModifier import BaseModifier


class BandpassFilter(BaseModifier):

    def __init__(self, sampleRate, configuration):
        BaseModifier.__init__(self, sampleRate, configuration)

        try:
            self.firstStopband = configuration['firstStopband']
            self.firstPassband = configuration['firstPassband']
            self.secondPassband = configuration['secondPassband']
            self.secondStopband = configuration['secondStopband']

            self.passbandAttenuation = configuration['passbandAttenuation']
            self.stopbandAttenuation = configuration['stopbandAttenuation']
        except KeyError as e:
            print "ERROR: Could not find key %s." % (str(e))

        self.filter = \
            python_initialize_kaiser_filter(
                self.firstStopband,
                self.firstPassband,
                self.secondPassband,
                self.secondStopband,
                self.passbandAttenuation,
                self.stopbandAttenuation,
                self.sampleRate
            )

        self.filterDelay = python_filter_get_group_delay(self.filter)

    def __del__(self):
        csignal_destroy_passband_filter(self.filter)

    def modify(self, signal):
        filteredSignal = python_filter_signal(self.filter, signal)

        return(filteredSignal)

    def toString(self):
        string = "BandpassFilter:\n"
        string = string + "\tSample rate:\t\t%d\n" % (self.sampleRate)
        string = string + "\tFirst stopband:\t\t%d Hz\n" % (self.firstStopband)
        string = string + "\tFirst passband:\t\t%d Hz\n" % (self.firstPassband)
        string = string + "\tSecond passband:\t%d Hz\n" % (self.secondPassband)
        string = string + "\tSecond stopband:\t%d Hz\n" % (self.secondStopband)
        string = string + "\tPassband attenuation:\t%.02f dB\n" \
            % (self.passbandAttenuation)
        string = string + "\tStopband attenuation:\t%d dB\n"  \
            % (self.stopbandAttenuation)
        string = string + "\tGroup delay:\t\t%d samples\n" % (self.filterDelay)

        return(string)

    def getOffset(self):
        return(self.filterDelay)
