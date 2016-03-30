from pyCovertAudio_lib import *
from BaseModulator import BaseModulator
from SignalFunctions import SignalFunctions


class BFSKModulator(BaseModulator):

    def __init__(
            self, bitsPerSymbol, sampleRate, samplesPerSymbol,
            symbolExpansionFactor, separationIntervals, configuration
    ):

        BaseModulator.__init__(
            self,
            bitsPerSymbol,
            sampleRate,
            samplesPerSymbol,
            symbolExpansionFactor,
            separationIntervals,
            configuration
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
            self.carrierFrequency,
            self.separationIntervals
        )

    def modulate(self, symbolSequence, signal, sentinel=None):
        symbolSignalLength = self.samplesPerSymbol * self.symbolExpansionFactor

        for symbol in symbolSequence:
            symbolFrequency = self.carrierFrequency

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
            "Modulator:\n\tAlgorithm:\t\t\tBFSK\n\tSymbol 0 frequency:\t\t"
            "%.02f\n\tSymbol 1 frequency:\t\t%.02f\n\tMin frequency"
            " separation:\t%.02f\n\tBandwidth:\t\t\t%.02f\n%s"
            % (
                self.symbol0Frequency,
                self.symbol1Frequency,
                self.deltaFrequency,
                self.bandwidth,
                BaseModulator.toString(self)
            )
        )
