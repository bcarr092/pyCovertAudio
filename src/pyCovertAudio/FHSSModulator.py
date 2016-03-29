from pyCovertAudio_lib import *
from BaseModulator import BaseModulator
from BFSKModulator import BFSKModulator
from SignalFunctions import SignalFunctions

import random


class FHSSModulator(BaseModulator):

    def __init__(
            self, bitsPerSymbol, sampleRate, samplesPerSymbol, symbolExpansionFactor,
            separationIntervals, configuration
    ):

        BaseModulator.__init__(
            self,
            bitsPerSymbol,
            sampleRate,
            samplesPerSymbol,
            symbolExpansionFactor,
            separationIntervals,
            {"carrierFrequency": 0}
        )

        (
            self.symbol0Frequency,
            self.symbol1Frequency,
            self.deltaFrequency,
            self.bandwidth
        ) = \
            python_BFSK_determine_frequencies(
            self.samplesPerSymbol,
            self.sampleRate,
            0,
            self.separationIntervals
        )

        try:
            self.minimumFrequency = configuration["minimumFrequency"]
            self.maximumFrequency = configuration["maximumFrequency"]
            self.bandwidthDivisor = configuration["bandwidthDivisor"]

            self.bandwidth /= self.bandwidthDivisor

            self.carrierFrequencies   = \
                SignalFunctions.getCarrierFrequencies(
                    self.minimumFrequency,
                    self.maximumFrequency,
                    self.bandwidth
                )
            self.numberOfSubChannels = len(self.carrierFrequencies)

        except KeyError as e:
            print "ERROR: Could not find key %s" % (str(e))

    def modulate(self, symbolSequence, signal, sentinel):
        symbolSignalLength = self.samplesPerSymbol * self.symbolExpansionFactor

        for symbol in symbolSequence:
            carrierFrequencyIndex = \
                random.SystemRandom().randint(0, len(self.carrierFrequencies) - 1)

            carrierFrequency = self.carrierFrequencies[carrierFrequencyIndex]
            symbolFrequency = carrierFrequency

            if(symbol == 1):
                symbolFrequency += self.symbol1Frequency
            else:
                symbolFrequency += self.symbol0Frequency

            x = \
                SignalFunctions.modulateFSK(
                    symbolSignalLength, self.sampleRate, [symbolFrequency]
                )

            signal.extend(x[: self.samplesPerSymbol])

            signal.extend(
                [0.0 for i in range(
                    (self.symbolExpansionFactor - 1) * self.samplesPerSymbol)]
            )

    def toString(self):
        return (
            "Modulator:\n\tAlgorithm:\t\t\tFHSS\n\tMinimum frequency:\t\t%.02f Hz\n\t"
            "Maximum frequency:\t\t%.02f Hz\n\t"
            "Symbol 0 frequency (rel):\t%.02f Hz\n\t"
            "Symbol 1 frequency (rel):\t%.02f Hz\n\tBandwidth:\t\t\t%.02f Hz\n%s"
            % (
                self.minimumFrequency,
                self.maximumFrequency,
                self.symbol0Frequency,
                self.symbol1Frequency,
                self.bandwidth,
                BaseModulator.toString(self)
            )
        )
