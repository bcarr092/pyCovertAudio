from pyCovertAudio_lib import *

from CAHALAudioFormatDescription import CAHALAudioFormatDescription


class CAHALDeviceStream:

    def __init__(self, device_stream):
        if(type(device_stream) is cahal_device_stream):
            self.handle = device_stream.handle
            self.direction = device_stream.direction
            self.preferredFormat = device_stream.preferred_format

            self.catalogueAudioFormats(device_stream)
        else:
            print "Device is not the correct instance: %s." \
                            % (type(device_stream))

    def catalogueAudioFormats(self, stream):
        self.supportedFormats = []

        if(stream.supported_formats):
            format_description_index = 0
            format_description =\
                cahal_audio_format_description_list_get(
                    stream.supported_formats,
                    format_description_index
                )

            while(format_description):
                format = CAHALAudioFormatDescription(format_description)

                self.supportedFormats.append(format)

                format_description_index += 1
                format_description = \
                    cahal_audio_format_description_list_get(
                        stream.supported_formats,
                        format_description_index
                    )

    def printMe(self, indent):
        print "%sDirection: %s"                               \
            % (
              indent,
              "Input"
                if self.direction == CAHAL_DEVICE_INPUT_STREAM
                else "Output"
            )

        if(self.preferredFormat):
            print "%sPreferred format: %s"                  \
                % (
                  indent,
                  cahal_convert_audio_format_id_to_cstring(
                      self.preferredFormat
                  )
                )

        if(self.supportedFormats):
            print "%sSupported formats:" % (indent)

            for format in self.supportedFormats:
                format.printMe(indent + '\t')
