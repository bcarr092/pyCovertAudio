from ModulatorFactory import ModulatorFactory
from ModifierFactory import ModifierFactory
from SymbolTracker import SymbolTracker
from Base import Base
from Debug import Debug


class BaseTransmitter(Base):

    def __init__(self, configuration):
        try:
            Base.__init__(self, configuration)

            self.dataModulator =  \
                ModulatorFactory.create(
                    self.bitsPerSymbol,
                    self.sampleRate,
                    self.samplesPerSymbol,
                    self.symbolExpansionFactor,
                    self.separationIntervals,
                    configuration['modulator']
                )

            self.modifiers = []

            ModifierFactory.initializeModifiers(
                self.sampleRate,
                configuration['modifiers'],
                self.modifiers
            )

        except KeyError as e:
            print "ERROR: Could not find key %s." % (str(e))

    def toString(self):
        string =  \
            "Base transmitter:\nModulator:\n%sModifiers:\n" \
            % (
                self.dataModulator.toString(),
            )

        for modifier in self.modifiers:
            string += modifier.toString()

        return(string)

    def transmit(self):
        signal = []

        self.prepareDataToTransmit()

        symbolSequence = []
        symbol = self.symbolTracker.getNextSymbol()

        while(symbol is not None):
            symbolSequence.append(symbol)

            symbol = self.symbolTracker.getNextSymbol()

        self.dataModulator.modulate(symbolSequence, signal, self.sentinel)

        Debug.instance.debugSignal(
            'modulatedSignal.WAV', signal, self.sampleRate)

        for modifier in self.modifiers:
            signal = modifier.modify(signal)

        Debug.instance.debugSignal('transmitted.WAV', signal, self.sampleRate)

        return(signal)

    def prepareDataToTransmit(self):
        data = self.sentinel + self.encodeData()

        self.symbolTracker = SymbolTracker(self.bitsPerSymbol, data)
