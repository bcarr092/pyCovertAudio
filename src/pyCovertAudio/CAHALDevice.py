from pyCovertAudio_lib import *

from CAHALSampleRateRange import CAHALSampleRateRange
from CAHALDeviceStream import CAHALDeviceStream


class CAHALDevice:
    devices = None

    def __init__(self, device):
        if(type(device) is cahal_device):
            self.struct = device
            self.handle = device.handle
            self.name = device.device_name
            self.model = device.model
            self.manufacturer = device.manufacturer
            self.serialNumber = device.serial_number
            self.version = device.version
            self.deviceUID = device.device_uid
            self.modelUID = device.model_uid
            self.isAlive = (True if device.is_alive else False)
            self.isRunning = (True if device.is_running else False)

            self.preferences = {}

            if(device.preferred_number_of_channels):
                self.preferences['numberOfChannels'] =  \
                    device.preferred_number_of_channels

            if(device.preferred_sample_rate):
                self.preferences['sampleRate'] = device.preferred_sample_rate

            self.catalogueSupportedSampleRates(device)

            self.catalogueStreams(device)

        else:
            print "Device is not the correct instance: %s." % (type(device))

    def catalogueSupportedSampleRates(self, device):
        self.supportedSampleRates = []

        if(device.supported_sample_rates):
            sample_rate_index = 0
            sample_rate = \
                cahal_sample_rate_range_list_get(
                    device.supported_sample_rates,
                    sample_rate_index
                )

            while(sample_rate):
                sampleRate =                  \
                    CAHALSampleRateRange(
                        sample_rate.minimum_rate,
                        sample_rate.maximum_rate
                    )     \

                self.supportedSampleRates.append(sampleRate)

                sample_rate_index += 1

                sample_rate =                         \
                    cahal_sample_rate_range_list_get(
                        device.supported_sample_rates,
                        sample_rate_index
                    )

    def catalogueStreams(self, device):
        self.streams = []

        if(device.device_streams):
            stream_index = 0
            stream = \
                cahal_device_stream_list_get(
                    device.device_streams,
                    stream_index
                )

            while(stream):
                self.streams.append(CAHALDeviceStream(stream))

                stream_index += 1
                stream = \
                    cahal_device_stream_list_get(
                        device.device_streams,
                        stream_index
                    )

    def doesSupportRecording(self):
        supportsRecording = False

        for stream in self.streams:
            if(stream.direction == CAHAL_DEVICE_INPUT_STREAM):
                supportsRecording = True

                break

        return(supportsRecording)

    def doesSupportPlayback(self):
        supportsPlayback = False

        for stream in self.streams:
            if(stream.direction == CAHAL_DEVICE_OUTPUT_STREAM):
                supportsPlayback = True

                break

        return(supportsPlayback)

    def printMe(self):
        print "Device '%s'" % (self.name)

        if(self.model):
            print "\tModel: %s" % (self.model)
        if(self.manufacturer):
            print "\tManufacturer: %s" % (self.manufacturer)
        if(self.serialNumber):
            print "\tSN: %s" % (self.serialNumber)
        if(self.version):
            print "\tVersion: %s" % (self.version)
        if(self.deviceUID):
            print "\tUID: %s" % (self.deviceUID)
        if(self.modelUID):
            print "\tModel UID: %s" % (self.modelUID)

        print "\tAlive?: %s" % ("Yes" if self.isAlive else "No")
        print "\tRunning?: %s" % ("Yes" if self.isRunning else "No")

        if (
            'numberOfChannels' in self.preferences and
            'sampleRate' in self.preferences
        ):

            print "\tPreferences:"
            print "\t\t%d channel(s), %.02f Hz"         \
                % (
                  self.preferences['numberOfChannels'],
                    self.preferences['sampleRate']
                )

        if(self.supportedSampleRates):
            print "\tSupported sample rates:"

            for sampleRate in self.supportedSampleRates:
                sampleRate.printMe("\t\t")

        if(self.streams):
            print "\tStreams:"

            for stream in self.streams:
                stream.printMe("\t\t")

    def doesSupportFormat(
        self, stream, numberOfChannels, bitDepth, sampleRate
                         ):
        for format in stream.supportedFormats:
            if(
                format.numberOfChannels == numberOfChannels and
                format.bitDepth == bitDepth and
                (
                    format.sampleRate.minimum <= sampleRate and
                    format.sampleRate.maximum >= sampleRate
                )
            ):
                return(True)

        return(False)

    def hasAppropriateStream(
        self, direction, numberOfChannels, bitDepth, sampleRate
                            ):
        foundStream = False

        for stream in self.streams:
            if(
                stream.direction == direction and
                self.doesSupportFormat(
                    stream,
                    numberOfChannels,
                    bitDepth,
                    sampleRate
                )
            ):
                foundStream = True

        return(foundStream)

    @staticmethod
    def catalogueDevices():
        if(CAHALDevice.devices is None):
            device_list = cahal_get_device_list()
            index = 0
            device = cahal_device_list_get(device_list, index)

            CAHALDevice.devices = []

            while(device):
                CAHALDevice.devices.append(CAHALDevice(device))

                index += 1
                device = cahal_device_list_get(device_list, index)

        return(CAHALDevice.devices)

    @staticmethod
    def findDevice(deviceName):
        if(CAHALDevice.devices is None):
            CAHALDevice.catalogueDevices()

        for device in CAHALDevice.devices:
            if(device.name == deviceName):
                return(device)

        return(None)
