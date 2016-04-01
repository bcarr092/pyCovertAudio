from pyCovertAudio_lib import *
from WAVTransmitter import WAVTransmitter
from SymbolTracker import SymbolTracker


class WAVMultiCarrierTransmitter(WAVTransmitter):

    def __init__(self, configuration):
        WAVTransmitter.__init__(self, configuration)

    def toString(self):
        return (
            "WAV (MC) info:\n%s"
            % (
                WAVTransmitter.toString(self)
            )
        )

    def prepareDataToTransmit(self):
        data = self.encodeData()

        self.symbolTracker = SymbolTracker(self.bitsPerSymbol, data)
