from pyCovertAudio_lib import *
from BaseDemodulator import BaseDemodulator
from Debug import Debug
from SignalFunctions import SignalFunctions

import time
import math


class FHSSDemodulator(BaseDemodulator):

    def __init__(
            self, bitsPerSymbol, sampleRate, samplesPerSymbol,
            symbolExpansionFactor, separationIntervals, configuration
    ):

        configuration["carrierFrequency"] = 0.0

        BaseDemodulator.__init__(
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
            0,
            self.separationIntervals
        )

        try:
            self.minimumFrequency = configuration["minimumFrequency"]
            self.maximumFrequency = configuration["maximumFrequency"]
            self.symbolFrequencyBandwidth = configuration[
                'symbolFrequencyBandwidth']
            self.bandwidthDivisor = configuration["bandwidthDivisor"]

            self.bandwidth /= self.bandwidthDivisor

            self.carrierFrequencies = \
                SignalFunctions.getCarrierFrequencies(
                    self.minimumFrequency,
                    self.maximumFrequency,
                    self.bandwidth
                )
            self.numberOfSubChannels = len(self.carrierFrequencies)

        except KeyError as e:
            print "ERROR: Could not find key %s" % (str(e))

        self.initializeSignals()

        self.initializeFilters()

    def initializeSignals(self):
        self.interpolationGap =  \
            int(
                2.0 *
                (
                    (float(self.sampleRate) / 2.0) -
                    (self.carrierFrequencies[-1] + (self.bandwidth / 2.0))
                )
            )

        print "Gap: %d" % (self.interpolationGap)

        self.decimationFactor =  \
            int(
                math.floor(
                    (2.0 * self.samplesPerSymbol) /
                    float(self.decimatedSamplesPerSymbol)
                )
            )
        self.decimatedSampleRate = \
            int(
                math.ceil(
                    (2.0 * self.sampleRate) /
                    float(self.decimationFactor)
                )
            )

    def initializeFilters(self):
        self.filters = []

        for carrierFrequency in self.carrierFrequencies:
            (filter0, filter1) = self.initializeFilter(carrierFrequency)

            self.filters.append(filter0)
            self.filters.append(filter1)

    def initializeFilter(self, carrierFrequency):
        passbandSymbol0Frequency = carrierFrequency + self.symbol0Frequency
        passbandSymbol1Frequency = carrierFrequency + self.symbol1Frequency

        frequencySeparation =   \
            passbandSymbol1Frequency - passbandSymbol0Frequency

        filter0 = \
            python_initialize_kaiser_filter(
                passbandSymbol0Frequency - frequencySeparation,
                passbandSymbol0Frequency -
                (float(self.symbolFrequencyBandwidth) / 2.0),
                passbandSymbol0Frequency +
                (float(self.symbolFrequencyBandwidth) / 2.0),
                passbandSymbol0Frequency + frequencySeparation,
                0.1,
                80,
                self.sampleRate
            )

        filter1 = \
            python_initialize_kaiser_filter(
                passbandSymbol1Frequency - frequencySeparation,
                passbandSymbol1Frequency -
                (float(self.symbolFrequencyBandwidth) / 2.0),
                passbandSymbol1Frequency +
                (float(self.symbolFrequencyBandwidth) / 2.0),
                passbandSymbol1Frequency + frequencySeparation,
                0.1,
                80,
                self.sampleRate
            )

        filterDelay0 = python_filter_get_group_delay(filter0)
        filterDelay1 = python_filter_get_group_delay(filter1)

        print "Symbol filter delays:\t1=%d\t2=%d" \
            % (filterDelay0, filterDelay1)

        return(filter0, filter1)

    def combineSignal(self, signal):
        filtered = []
        interpolated = []
        squared = []
        decimated = []
        normalized = []

        filterStartTime = time.time()

        for signalFilter in self.filters:
            filterDelay = python_filter_get_group_delay(signalFilter)

            filteredSignal = python_filter_signal(signalFilter, signal)
            filteredSignal = filteredSignal[filterDelay:]

            filtered.append(filteredSignal)

        filterTime = time.time() - filterStartTime

        print "Filter time: %.04f" % (filterTime)

        for i in range(len(filtered)):
            Debug.instance.debugSignal(
                "filteredSignal%d.WAV" % (i),
                filtered[i],
                self.sampleRate
            )

        interpolatorStartTime = time.time()

        for filteredSignal in filtered:
            interpolatedSignal = \
                SignalFunctions.interpolateSignal(
                    filteredSignal,
                    self.sampleRate,
                    self.interpolationGap
                )

            interpolated.append(interpolatedSignal)

        interpolatorTime = time.time() - interpolatorStartTime

        print "Interpolation time: %.04f" % (interpolatorTime)

        for i in range(len(interpolated)):
            Debug.instance.debugSignal(
                "interpolatedSignal%d.WAV" % (i),
                interpolated[i],
                2 * self.sampleRate
            )

        squareStartTime = time.time()

        for interpolatedSignal in interpolated:
            squaredSignal =  \
                SignalFunctions.squareSignal(
                    interpolatedSignal,
                    2 * self.sampleRate,
                    self.symbolFrequencyBandwidth,
                    self.interpolationGap
                )

            squared.append(squaredSignal)

        squareTime = time.time() - squareStartTime

        print "Square time: %.04f" % (squareTime)

        for i in range(len(squared)):
            Debug.instance.debugSignal(
                "squaredSignal%d.WAV" % (i),
                squared[i],
                2 * self.sampleRate
            )

        decimateStartTime = time.time()

        for squaredSignal in squared:
            decimatedSignal = \
                SignalFunctions.decimate(squaredSignal, self.decimationFactor)

            decimated.append(decimatedSignal)

        decimateTime = time.time() - decimateStartTime

        print "Decimate time: %.04f" % (decimateTime)

        for i in range(len(decimated)):
            Debug.instance.debugSignal(
                "signal%d.WAV" % (i),
                decimated[i],
                self.decimatedSampleRate
            )

        for decimatedSignal in decimated:
            signal = SignalFunctions.normalizeSignal(decimatedSignal)

            normalized.append(signal)

        combinedSignal0 = []
        combinedSignal1 = []

        for i in range(len(normalized[0])):
            maxValue = 0.0

            for j in range(0, len(normalized), 2):
                if(abs(normalized[j][i]) >= maxValue):
                    maxValue = abs(normalized[j][i])

            combinedSignal0.append(maxValue)

            maxValue = 0.0

            for j in range(1, len(normalized), 2):
                if(abs(normalized[j][i]) >= maxValue):
                    maxValue = abs(normalized[j][i])

            combinedSignal1.append(maxValue)

        Debug.instance.debugSignal(
            "combinedSignal0.WAV",
            combinedSignal0,
            self.decimatedSampleRate
        )

        Debug.instance.debugSignal(
            "combinedSignal1.WAV",
            combinedSignal1,
            self.decimatedSampleRate
        )

        signal0 = SignalFunctions.normalizeSignal(combinedSignal0)
        signal1 = SignalFunctions.normalizeSignal(combinedSignal1)

        signal1 = [-1.0 * x for x in signal1]

        signal0Length = len(signal0)
        signal1Length = len(signal1)

        length = min(signal0Length, signal1Length)

        result = [0.0 for i in range(length)]

        for i in range(length):
            if(abs(signal0[i]) >= abs(signal1[i])):
                result[i] = signal0[i]
            else:
                result[i] = signal1[i]

        Debug.instance.debugSignal(
            'combined.WAV',
            result,
            self.decimatedSampleRate
        )

        return(result)

    def demodulate(self, signal):
        combined = self.combineSignal(signal)
        symbols = self.getSymbols(combined)

        return(symbols)

    def toString(self):
        return (
            "Demodulator:\n\tAlgorithm:\t\t\tBFSK\n\tSymbol 0 frequency"
            ":\t\t%.02f\n\tSymbol 1 frequency:\t\t%.02f\n\tMin frequency"
            " separation:\t%.02f\n\tBandwidth:\t\t\t%.02f\n\tFrequency"
            " bandwidth:\t\t%d\n%s"
            % (
                self.symbol0Frequency,
                self.symbol1Frequency,
                self.deltaFrequency,
                self.bandwidth,
                self.symbolFrequencyBandwidth,
                BaseDemodulator.toString(self)
            )
        )

    def getSymbols(self, signal):
        symbols = []

        averaged =  \
            SignalFunctions.movingAverage(
                signal,
                self.symbolExpansionFactor * self.decimatedSamplesPerSymbol
            )

        Debug.instance.debugSignal(
            'averaged.WAV',
            averaged,
            self.decimatedSampleRate
        )

        averaged =  \
            SignalFunctions.interpolateSignal(
                averaged,
                self.decimatedSampleRate,
                1
            )

        Debug.instance.debugSignal(
            'averagedInterpolated.WAV',
            averaged,
            2 * self.decimatedSampleRate
        )

        averaged = SignalFunctions.normalizeSignal(averaged)

        Debug.instance.debugSequence('averaged.dat', averaged)

        samplePoints =  \
            self.runGardnerAlgorithm(
                averaged,
                2 * self.symbolExpansionFactor *
                self.decimatedSamplesPerSymbol,
                2 * self.decimatedSampleRate
            )

        Debug.instance.debugSequence('samplePoints.dat', samplePoints)

        for point in samplePoints:
            symbol = 0 if(averaged[point] >= 0.0) else 1

            symbols.append(symbol)

        return(symbols)

    def runGardnerAlgorithm(self, signal, samplesPerSymbol, sampleRate):
        n = 1
        offset = 0

        samplePoints = []

        print "Samples per symbol: %d\tSample rate: %d." \
            % (samplesPerSymbol, sampleRate)

        while(((n * samplesPerSymbol) + offset) < len(signal)):
            nextPoint = n * samplesPerSymbol + offset
            previousPoint = (n - 1) * samplesPerSymbol + offset
            midPoint = \
                int(nextPoint - int(math.floor(samplesPerSymbol / 2.0)))

            samplePoints.append(nextPoint)

            e = \
                (signal[nextPoint] - signal[previousPoint]) * signal[midPoint]

            if((signal[nextPoint] * signal[previousPoint]) < 0):
                if(e < -0.0001):
                    offset += 1
                elif(e > 0.0001):
                    offset -= 1

            n += 1

        return(samplePoints)
