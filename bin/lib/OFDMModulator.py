from pyCovertAudio_lib import *

from BaseModulator import BaseModulator
from BFSKModulator import BFSKModulator
from BandpassFilter import BandpassFilter
from SignalFunctions import SignalFunctions
from SymbolTracker import SymbolTracker


class OFDMModulator(BaseModulator):

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

            self.carrierFrequencies = \
                SignalFunctions.getCarrierFrequencies(
                    self.minimumFrequency,
                    self.maximumFrequency,
                    self.bandwidth
                )
            self.numberOfSubChannels = len(self.carrierFrequencies)

            self.modulators = []
            self.filters = []

            for carrierFrequency in self.carrierFrequencies:
                self.modulators.append(
                    BFSKModulator(
                        self.bitsPerSymbol,
                        self.sampleRate,
                        self.samplesPerSymbol,
                        self.symbolExpansionFactor,
                        self.separationIntervals,
                        {"carrierFrequency": carrierFrequency}
                    )
                )

                firstPassband = carrierFrequency - (self.bandwidth / 2.0)
                secondPassband = carrierFrequency + (self.bandwidth / 2.0)

                firstStopband = firstPassband - (self.bandwidth / 2.0)
                secondStopband = secondPassband + (self.bandwidth / 2.0)

                self.filters.append(
                    BandpassFilter(
                        self.sampleRate,
                        {
                            "firstStopband": firstStopband,
                            "firstPassband": firstPassband,
                            "secondPassband": secondPassband,
                            "secondStopband": secondStopband,
                            "passbandAttenuation": 1.0,
                            "stopbandAttenuation": 80
                        }
                    )
                )

            self.singleCarrierModulator = \
                BFSKModulator(
                    self.bitsPerSymbol,
                    self.sampleRate,
                    self.samplesPerSymbol,
                    self.symbolExpansionFactor,
                    self.separationIntervals,
                    {"carrierFrequency": self.carrierFrequencies[0]}
                )

        except KeyError as e:
            print "ERROR: Could not find key %s" % (str(e))

    def modulate(self, symbolSequence, signal, sentinel):
        if(sentinel is not None):
            sentinelSequence = []
            symbolTracker = SymbolTracker(self.bitsPerSymbol, sentinel)
            symbol = symbolTracker.getNextSymbol()

            while(symbol is not None):
                for i in range(self.numberOfSubChannels):
                    sentinelSequence.append(symbol)

                symbol = symbolTracker.getNextSymbol()

        print sentinelSequence + symbolSequence

        self.multiCarrierModulate(sentinelSequence + symbolSequence, signal)

    def multiCarrierModulate(self, symbolSequence, signal):
        subChannelSignals = []
        symbolSequences = []

        for i in range(self.numberOfSubChannels):
            symbolSequences.append(
                symbolSequence[i:: self.numberOfSubChannels]
            )
            subChannelSignals.append([])

        for i in range(self.numberOfSubChannels):
            self.modulators[i].modulate(
                symbolSequences[i],
                subChannelSignals[i]
            )

            subChannelSignals[i] = \
                self.filters[i].modify(subChannelSignals[i])

        maxLength = max([len(x) for x in subChannelSignals])

        for i in range(maxLength):
            sampleValue = 0.0

            for j in range(self.numberOfSubChannels):
                if(i < len(subChannelSignals[j])):
                    sampleValue += float(subChannelSignals[j][i])

            signal.append(sampleValue)

    def toString(self):
        string =  \
            "Modulator:\n\tAlgorithm:\t\t\tOFDM\n\tMinimum frequency:\t\t" \
            "%.02f Hz\n\tMaximum frequency:\t\t%.02f Hz\n\tSymbol 0 frequency"\
            " (rel):\t%.02f Hz\n\tSymbol 1 frequency (rel):\t%.02f Hz\n\t"\
            "Bandwidth:\t\t\t%.02f Hz\n" \
            % (
                self.minimumFrequency,
                self.maximumFrequency,
                self.symbol0Frequency,
                self.symbol1Frequency,
                self.bandwidth
            )

        for i in range(len(self.modulators)):
            string += "Sub-Channel Modulator %d\n" % ((i + 1))
            string += self.modulators[i].toString()
            string += self.filters[i].toString()

        return(string)
