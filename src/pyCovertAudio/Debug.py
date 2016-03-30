import sys
import os

import pyCovertAudio_lib


class Debug:
    instance = None

    def __init__(
        self, debugDirectory, outputDirectory
    ):

        self.debugDirectory = debugDirectory
        self.outputDirectory = outputDirectory

        self.createDirectories()

        self.debug = False

    def createDirectories(self):
        if(not os.path.exists(self.debugDirectory)):
            os.mkdir(self.debugDirectory)
        else:
            if(not os.path.isdir(self.debugDirectory)):
                print "ERROR: %s is not a directory." % (self.debugDirectory)

                sys.exit(-1)

        if(not os.path.exists(self.outputDirectory)):
            os.mkdir(self.outputDirectory)
        else:
            if(not os.path.isdir(self.outputDirectory)):
                print "ERROR: %s is not a directory." % (self.outputDirectory)

                sys.exit(-1)

    def toString(self):
        return (
            "Debug:\n\tDebug directory:\t'%s'\n\tOutput directory:\t'%s'"
            "\n\tEnabled:\t\t%s"
            % (
                self.debugDirectory,
                self.outputDirectory,
                "Yes" if(self.debug) else "No"
            )
        )

    def enableDebug(self):
        self.debug = True

    def disableDebug(self):
        self.debug = False

    def getDebugDirectory(self):
        return(self.debugDirectory)

    def getOutputDirectory(self):
        return(self.outputDirectory)

    @staticmethod
    def getInstance(configuration):
        if(Debug.instance is None):
            try:
                debugDirectory = configuration['debugDirectory']
                debug = True if(configuration['debugEnabled']) else False
                outputDirectory = configuration['outputDirectory']

                Debug.instance =  \
                    Debug(
                        debugDirectory,
                        outputDirectory
                    )

                if(debug):
                    Debug.instance.enableDebug()
                else:
                    Debug.instance.disableDebug()

            except KeyError as e:
                print "ERROR: Could not find key %s." % (str(e))

        return(Debug.instance)

    def debugSignal(self, fileName, signal, sampleRate):
        if(self.debug):
            path = os.path.join(self.debugDirectory, fileName)

            self.saveSignal(path, signal, sampleRate)

    def outputSignal(self, fileName, signal, sampleRate):
        path = os.path.join(self.outputDirectory, fileName)

        self.saveSignal(path, signal, sampleRate)

    def saveSignal(self, fileName, signal, sampleRate):
        maxValue = -1

        for value in signal:
            if(abs(value) > maxValue):
                maxValue = abs(value)

        wavSignal = \
            map(lambda x: (x * 1.0) / (maxValue * 1.0), signal)

        samples = [wavSignal]

        if(os.path.exists(fileName)):
            os.unlink(fileName)

        pyCovertAudio_lib.python_write_FLOAT_wav(
            fileName,
            len(samples),
            sampleRate,
            len(wavSignal),
            samples
        )

    def debugSequence(self, fileName, sequence, variable=None):
        if(self.debug):
            self.saveSequence(
                os.path.join(self.debugDirectory, fileName),
                sequence,
                variable
            )

    def outputSequence(self, fileName, sequence, variable=None):
        self.saveSequence(
            os.path.join(self.outputDirectory, fileName),
            sequence,
            variable
        )

    def saveSequence(self, fileName, sequence, variable):
        if(variable is None):
            variable = [i for i in range(len(sequence))]

        try:
            file = open(fileName, 'w')

            if(file):
                for i in range(len(sequence)):
                    file.write("%f\t%f\n" % (variable[i], sequence[i]))

                file.close()
        except IOError as e:
            print "I/O error(%d): %d" % (e.errno, e.strerror)
