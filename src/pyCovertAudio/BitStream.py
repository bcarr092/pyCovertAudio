from pyCovertAudio_lib import *

import struct


class BitStream:

    def __init__(self, circular, buffer):
        if(type(buffer) is str):
            self.stream = python_bit_stream_initialize(circular, buffer)
        else:
            self.stream = \
                python_bit_stream_initialize_from_bit_packer(
                    circular, buffer.packer)

            self.bitPacker = buffer

    def __del__(self):
        if(self.stream):
            bit_stream_destroy(self.stream)

        self.bitPacker = None

    def readBytes(self, numberOfBits):
        (numBitsRead, buffer) = \
            python_bit_stream_get_bits(self.stream, numberOfBits)

        return(buffer)

    def readRaw(self, numberOfBits):
        returnValue = None

        if(32 < numberOfBits):
            returnValue = self.readBytes(numberOfBits)
        else:
            (numBitsRead, buffer) = \
                python_bit_stream_get_bits(self.stream, numberOfBits)

            if(numberOfBits == 8):
                returnValue = struct.unpack("b", buffer)[0]
            elif(numberOfBits == 16):
                returnValue = struct.unpack("h", buffer)[0]
            elif(numberOfBits == 32):
                returnValue = struct.unpack("i", buffer)[0]
            else:
                print "ERROR: Could not handle number of bits (%d)."  \
                    % (numberOfBits)

        return(returnValue)

    def read(self, numberOfBits):
        returnValue = None

        if(32 < numberOfBits):
            returnValue = self.readBytes(numberOfBits)
        else:
            (numBitsRead, buffer) = \
                python_bit_stream_get_bits(self.stream, numberOfBits)

            if(0 < numBitsRead and buffer and 4 >= len(buffer)):
                signBit = ord(buffer[0]) & 0x80
                shiftByte = '\xFF' if(signBit) else '\x00'
                shiftBytes = 4 - len(buffer)
                string = (shiftByte * shiftBytes) + buffer

                returnValue = struct.unpack("!I", string)[0]
                returnValue >>= (len(buffer) * 8) - numBitsRead
            else:
                print "ERROR: Could not read 0x%x bits." % (numberOfBits)

        return(returnValue)

    def getSize(self):
        return(bit_stream_get_number_of_remaining_bits(self.stream))

    def getRawBytes(self):
        data = ''

        if(self.stream.packer):
            data = python_bit_packer_get_bytes(self.stream.packer)

            if(not data):
                print "ERROR: Could not read data from packer."

                data = ''
        else:
            print "ERROR: Packer member of stream is not accessible."

        return(data)
