from pyCovertAudio_lib import *

import sys
import math
import scipy.fftpack


class SignalFunctions():

    @staticmethod
    def removeBias(signal):
        mean = float(sum(signal)) / float(len(signal))

        print "Mean: %f" % (mean)

        return([sample - mean for sample in signal])

    @staticmethod
    def interpolateSignal(signal, sampleRate, transitionBW):
        upsampledSignal = [0.0 for i in range(2 * len(signal))]

        for i in range(len(signal)):
            upsampledSignal[i * 2] = signal[i]

        lowPassFilter = \
            python_initialize_kaiser_lowpass_filter(
                sampleRate / 2,
                (sampleRate / 2) + transitionBW,
                0.1,
                80,
                sampleRate * 2
            )

        filterDelay = python_filter_get_group_delay(lowPassFilter)

        print "Interpolator filter delay: %d." % (filterDelay)

        filteredSignal = python_filter_signal(lowPassFilter, upsampledSignal)
        filteredSignal = filteredSignal[filterDelay:]

        csignal_destroy_passband_filter(lowPassFilter)

        return(filteredSignal)

    @staticmethod
    def squareSignal(signal, sampleRate, passband, transitionBW):
        squaredSignal = [x ** 2.0 for x in signal]

        lowPassFilter = \
            python_initialize_kaiser_lowpass_filter(
                passband,
                passband + transitionBW,
                0.1,
                80,
                sampleRate
            )

        filterDelay = python_filter_get_group_delay(lowPassFilter)

        print "Lowpass filter delay: %d." % (filterDelay)

        filteredSignal = python_filter_signal(lowPassFilter, squaredSignal)
        filteredSignal = filteredSignal[filterDelay:]

        return(filteredSignal)

    @staticmethod
    def decimate(signal, decimationFactor):
        decimatedSignal = \
            [signal[i] for i in range(0, len(signal), decimationFactor)]

        return(decimatedSignal)

    @staticmethod
    def movingAverage(signal, numberOfSamples):
        averageSignal = [0.0 for i in range(len(signal))]

        summer = [0.0 for i in range(numberOfSamples)]

        for i in range(len(signal)):
            summer[i % numberOfSamples] = signal[i]

            for x in summer:
                averageSignal[i] += x

            averageSignal[ i ] =  \
                float(averageSignal[i]) / float(numberOfSamples)

        return(averageSignal)

    @staticmethod
    def normalizeSignal(signal):
        average = sum(signal) / (1.0 * len(signal))

        print "Max: %d\tMin: %d\tAverage: %.04f"  \
            % (max(signal), min(signal), average)

        maxValue = max([max(signal), abs(min(signal))])
        signal = map(lambda x: (x * 1.0) / (maxValue * 1.0), signal)

        average = sum(signal) / (1.0 * len(signal))

        print "Max: %+.04f\tMin: %+.04f\tAverage: %+.04f" \
            % (max(signal), min(signal), average)

        return(signal)

    @staticmethod
    def nextPowOf2(value):
        nextPow = math.ceil(math.log(value, 2))

        return(nextPow)

    @staticmethod
    def modulateFSK(signalLength, sampleRate, frequencies):
        N = 2 ** SignalFunctions.nextPowOf2(signalLength)
        deltaF = (1.0 / float(N)) * (float(sampleRate) / 2.0)

        X = [0.0 for i in range(int(N))]

        for frequency in frequencies:
            frequencyBin = int(frequency / deltaF)

            X[frequencyBin] = float(sys.maxint)

        x = scipy.fftpack.irfft(X)

        return(x)

    @staticmethod
    def getCarrierFrequencies(minimumFrequency, maximumFrequency, bandwidth):
        carrierFrequencies = []
        carrierFrequency = minimumFrequency + (bandwidth / 2.0)

        while (
            (carrierFrequency + (bandwidth / 2.0)) <= maximumFrequency
        ):
            carrierFrequencies.append(carrierFrequency)

            print "Added carrier %.02f." % (carrierFrequency)

            carrierFrequency += bandwidth

        return(carrierFrequencies)
