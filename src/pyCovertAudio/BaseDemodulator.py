from pyCovertAudio_lib import *

import sys


class BaseDemodulator:

    def __init__(
            self, bitsPerSymbol, sampleRate, samplesPerSymbol,
            symbolExpansionFactor, separationIntervals, configuration
    ):

        self.initializeDemodulation(
            bitsPerSymbol,
            sampleRate,
            samplesPerSymbol,
            symbolExpansionFactor,
            separationIntervals,
            configuration
        )

    def initializeDemodulation(
            self, bitsPerSymbol, sampleRate, samplesPerSymbol,
            symbolExpansionFactor, separationIntervals, modulationInfo
    ):
        try:
            self.carrierFrequency = modulationInfo['carrierFrequency']
            self.decimatedSamplesPerSymbol = \
                modulationInfo['decimatedSamplesPerSymbol']
            self.bitsPerSymbol = bitsPerSymbol
            self.sampleRate = sampleRate
            self.samplesPerSymbol = samplesPerSymbol
            self.symbolExpansionFactor = symbolExpansionFactor
            self.separationIntervals = separationIntervals

            self.constellationSize = 2 ** self.bitsPerSymbol

        except KeyError as e:
            print "ERROR: Key %s is not present" % (str(e))

    def assembleSymbols(self, offset, symbolsList):
        return(symbolsList)

    def demodulate(self, signal, identifier=0):
        print "ERROR: This method must be overwritten."

        sys.exit(-1)

    def toString(self):
        return (
            "Base demodulator:\n\tCarrier frequency:\t\t%d Hz\n\t"
            "Bits per symbol:\t\t%d bits\n\tSample rate:\t\t\t%d Hz\n\t"
            "Samples per symbol:\t\t%d samples\n\tConstellation size:\t\t"
            "%d\n\tDecimated samples per symbol:\t%d\n"
            % (
                self.carrierFrequency,
                self.bitsPerSymbol,
                self.sampleRate,
                self.samplesPerSymbol,
                self.constellationSize,
                self.decimatedSamplesPerSymbol
            )
        )
