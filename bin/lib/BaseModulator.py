from pyCovertAudio_lib import *

import sys


class BaseModulator:

    def __init__(
            self, bitsPerSymbol, sampleRate, samplesPerSymbol,
            symbolExpansionFactor, separationIntervals, configuration
    ):

        self.initializeModulation(
            bitsPerSymbol,
            sampleRate,
            samplesPerSymbol,
            symbolExpansionFactor,
            separationIntervals,
            configuration
        )

    def initializeModulation(
            self, bitsPerSymbol, sampleRate, samplesPerSymbol,
            symbolExpansionFactor, separationIntervals, modulationInfo
    ):
        try:
            self.carrierFrequency = modulationInfo['carrierFrequency']
            self.bitsPerSymbol = bitsPerSymbol
            self.sampleRate = sampleRate
            self.samplesPerSymbol = samplesPerSymbol
            self.symbolExpansionFactor = symbolExpansionFactor
            self.separationIntervals = separationIntervals

            self.constellationSize = 2 ** self.bitsPerSymbol

        except KeyError as e:
            print "ERROR: Key %s is not present" % (str(e))

    def modulate(self, symbolSequence, signal, sentinel):
        print "ERROR: This method must be overwritten."

        sys.exit(-1)

    def toString(self):
        return (
            "Base modulator:\n\tCarrier frequency:\t\t%d Hz\n\t"
            "Bits per symbol:\t\t%d bits\n\tSample rate:\t\t\t%d Hz\n\t"
            "Samples per symbol:\t\t%d samples\n\tConstellation size:\t\t%d"
            "\n\tSymbol expansion factor:\t%d\n\tSeparation intervals:\t\t%d\n"
            % (
                self.carrierFrequency,
                self.bitsPerSymbol,
                self.sampleRate,
                self.samplesPerSymbol,
                self.constellationSize,
                self.symbolExpansionFactor,
                self.separationIntervals
            )
        )
