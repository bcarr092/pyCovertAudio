from SymbolTracker import SymbolTracker
from EncoderFactory import EncoderFactory

import random
import string


class Base:

    def __init__(self, configuration):
        try:
            self.sampleRate = configuration['sampleRate']
            self.bitsPerSymbol = configuration['bitsPerSymbol']
            self.samplesPerSymbol = configuration['samplesPerSymbol']
            self.symbolExpansionFactor = configuration['symbolExpansionFactor']
            self.separationIntervals = configuration['separationIntervals']

            self.initializeData(configuration['dataInfo'])

        except KeyError as e:
            print "ERROR: Could not find key %s." % (str(e))

    def decodeData(self, data):
        if(self.dataModifiers is not None and len(self.dataModifiers) != 0):
            errorPositions = []

            for modifier in self.dataModifiers:
                data = modifier.decode(data, errorPositions)

        return(data)

    def encodeData(self):
        data = self.data

        if(self.dataModifiers is not None and len(self.dataModifiers) != 0):
            for modifier in self.dataModifiers:
                data = modifier.encode(data)

        return(data)

    def initializeSentinel(self):
        if(self.sentinel != ""):
            sentinelTracker = SymbolTracker(self.bitsPerSymbol, self.sentinel)
            symbol = sentinelTracker.getNextSymbol()

            while(symbol is not None):
                self.sentinelSymbols.append(symbol)

                symbol = sentinelTracker.getNextSymbol()

    def initializeData(self, modulationInfo):
        self.data = ""
        self.sentinel = ""
        self.dataModifiers = []
        self.sentinelSymbols = []

        try:
            data = ""

            if('byteCount' in modulationInfo):
                data = self.generateRandomData(modulationInfo['byteCount'])
            elif('data' in modulationInfo):
                data = str(modulationInfo['data'])
            else:
                print \
                    "WARNING: Missing required keys 'byteCount' or 'data' in" \
                    " 'dataInfo' (This is normal if running in Receiver mode)."

            if('sentinel' in modulationInfo):
                self.sentinel = str(modulationInfo['sentinel'])

                self.initializeSentinel()

            dataModifiers = modulationInfo['modifiers']

            for modifier in dataModifiers:
                self.dataModifiers.append(EncoderFactory.create(modifier))

        except KeyError as e:
            print "ERROR: Key %s is not present" % (str(e))

        self.data = data

    def generateRandomData(self, numberOfBytes):
        data =  \
            ''.join(
                random.SystemRandom().
                choice(
                    string.printable
                )
                for _ in range(numberOfBytes)
            )

        return(data)
