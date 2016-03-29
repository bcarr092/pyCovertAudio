from BaseEncoder import BaseEncoder
from SymbolTracker import SymbolTracker
from ReedSolomon import ReedSolomon
from BitPacker import BitPacker
from BitStream import BitStream

import math


class ReedSolomonEncoder(BaseEncoder):
    SYMBOL_SIZE_IN_BITS = 8

    def __init__(self, configuration):
        BaseEncoder.__init__(self, configuration)

        if (
            self.messageLength % ReedSolomonEncoder.SYMBOL_SIZE_IN_BITS == 0
            and self.blockLength % ReedSolomonEncoder.SYMBOL_SIZE_IN_BITS == 0
        ):

            self.codec = ReedSolomon()

            self.numberOfMessageSymbols = \
                self.messageLength / ReedSolomonEncoder.SYMBOL_SIZE_IN_BITS
            self.numberOfBlockSymbols = \
                self.blockLength / ReedSolomonEncoder.SYMBOL_SIZE_IN_BITS

        else:
            print \
                "ERROR: Block (%d) or message (%d) length"  \
                " are not divisible by 8 bits." \
                % (self.blockLength, self.messageLength)

            self.codec = None

    def toString(self):
        string =  \
            "ReedSolomonEncoder\n\tBlock symbols:\t\t%d\n\tMessage symbols:\t%d\n%s"  \
            % (
                self.numberOfBlockSymbols,
                self.numberOfMessageSymbols,
                BaseEncoder.toString(self)
            )

        return(string)

    def encode(self, data):
        if(self.codec and self.checkParamterIsString(data)):
            bitPacker = BitPacker()
            bitStream = BitStream(False, bitPacker)
            symbolTracker = BitStream(False, data)
            numberOfBlocks  = \
                int(
                    math.ceil(
                        float(symbolTracker.getSize()) /
                        float(self.messageLength)
                    )
                )

            for i in range(numberOfBlocks):
                blockValues = [chr(0)
                               for i in range(self.numberOfMessageSymbols)]

                buffer = symbolTracker.readBytes(self.messageLength)

                for i in range(len(buffer)):
                    blockValues[i] = buffer[i]

                codeword =  \
                    self.codec.RSEncode(
                        blockValues,
                        (self.numberOfBlockSymbols - self.numberOfMessageSymbols)
                    )

                map(
                    lambda x:
                    bitPacker.writeByte(
                        x,
                        ReedSolomonEncoder.SYMBOL_SIZE_IN_BITS
                    ),
                    codeword
                )

            return(bitStream.getRawBytes())
        else:
            print "ERROR: Codec was not initialized."

            return(None)

    def decode(self, data, errorPositions=None):
        if(self.codec and self.checkParamterIsString(data)):
            bitPacker = BitPacker()
            bitStream = BitStream(False, bitPacker)
            symbolTracker = BitStream(False, data)
            numberOfBlocks  = \
                int(
                    math.ceil(
                        float(symbolTracker.getSize()) /
                        float(self.blockLength)
                    )
                )

            for i in range(numberOfBlocks):
                blockValues = [0 for j in range(self.numberOfBlockSymbols)]

                buffer = symbolTracker.readBytes(self.blockLength)

                for j in range(len(buffer)):
                    index = (i * self.numberOfBlockSymbols) + j
                    blockValues[j] = ord(buffer[j])

                    if (
                        errorPositions is not None
                        and index < len(errorPositions)
                        and errorPositions[index]
                    ):
                        blockValues[j] = -1

                message =  \
                    self.codec.RSDecode(
                        blockValues,
                        (self.numberOfBlockSymbols - self.numberOfMessageSymbols)
                    )

                if(message is not None):
                    map(
                        lambda x:
                        bitPacker.writeByte(
                            x,
                            ReedSolomonEncoder.SYMBOL_SIZE_IN_BITS
                        ),
                        message[0: self.numberOfMessageSymbols]
                    )
                else:
                    for i in range(self.numberOfMessageSymbols):
                        bitPacker.writeByte(
                            0,
                            ReedSolomonEncoder.SYMBOL_SIZE_IN_BITS
                        )

            return(bitStream.getRawBytes())
        else:
            print "ERROR: Codec was not initialized."

            return(None)
