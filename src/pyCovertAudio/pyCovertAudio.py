import os
import re
import socket
import select
import sys
import json
import math
import time
import argparse

# Add the lib directory in the installation directory
sys.path.append('lib')

import pyCovertAudio_lib

from CAHALDevice import CAHALDevice
from WAVRecorder import WAVRecorder
from WAVPlayer import WAVPlayer
from Debug import Debug

parser = None
args = None
configuration = None
devices = []


def handleInputArguments():
    global parser
    global args

    parser = \
        argparse.ArgumentParser(
            description='Interact with the platform\'s audio hardware'
        )

    parser.add_argument(
        '-v',
        '--verbosity',
        choices=['trace', 'debug', 'info', 'warning', 'error', 'none'],
        action='store',
        default='error',
        dest='verbosity',
        help='Verbosity level for the CAHAL and CSignal libraries'
    )

    parser.add_argument(
        '-c',
        '--conf',
        action='store',
        dest='configuration',
        required=True,
        help='Configuration file for command.'
    )

    args = parser.parse_args()

    loadConfigurationFromFile()


def setup():
    global configuration
    global devices

    pyCovertAudio_lib.python_cahal_initialize()
    pyCovertAudio_lib.csignal_initialize()

    initialize_debug_info()

    devices = CAHALDevice.catalogueDevices()


def loadConfigurationFromFile():
    global parser
    global args
    global configuration

    try:
        filePointer = open(args.configuration, 'rb', 0)

        if(filePointer):
            jsonContents = filePointer.read()

            configuration = json.loads(jsonContents)

            filePointer.close()
        else:
            print "ERROR: Could not read from '%s'." % (args.configuration)

            parser.print_help()

            sys.exit(-1)

    except IOError as e:
        print "ERROR: I/O error(%d): %s" % (e.errno, e.strerror)

        parser.print_help()

        sys.exit(-1)
    except ValueError as e:
        print "ERROR: Decode error: %s" % (str(e))

        parser.print_help()

        sys.exit(-1)


def terminate():
    pyCovertAudio_lib.cahal_terminate()
    pyCovertAudio_lib.csignal_terminate()


def initialize_debug_info():
    global args
    global parser
    global configuration

    if(args.verbosity == 'trace'):
        pyCovertAudio_lib.cpc_log_set_log_level(
            pyCovertAudio_lib.CPC_LOG_LEVEL_TRACE)
    elif(args.verbosity == 'debug'):
        pyCovertAudio_lib.cpc_log_set_log_level(
            pyCovertAudio_lib.CPC_LOG_LEVEL_DEBUG)
    elif(args.verbosity == 'info'):
        pyCovertAudio_lib.cpc_log_set_log_level(
            pyCovertAudio_lib.CPC_LOG_LEVEL_INFO)
    elif(args.verbosity == 'warning'):
        pyCovertAudio_lib.cpc_log_set_log_level(
            pyCovertAudio_lib.CPC_LOG_LEVEL_WARN)
    elif(args.verbosity == 'error'):
        pyCovertAudio_lib.cpc_log_set_log_level(
            pyCovertAudio_lib.CPC_LOG_LEVEL_ERROR)
    elif(args.verbosity == 'none'):
        pyCovertAudio_lib.cpc_log_set_log_level(
            pyCovertAudio_lib.CPC_LOG_LEVEL_NO_LOGGING)
    else:
        parser.print_help()

        sys.exit(-1)


def catalogueDevices():
    global devices

    device_list = pyCovertAudio_lib.cahal_get_device_list()
    index = 0
    device = pyCovertAudio_lib.cahal_device_list_get(device_list, index)

    while(device):
        devices.append(CAHALDevice(device))

        index += 1
        device = pyCovertAudio_lib.cahal_device_list_get(device_list, index)


def listDevices():
    global configuration
    global devices
    global parser

    try:
        configuration = configuration['listDevices']

        filter = 'None'

        if('listInput' in configuration):
            filter = 'Input'
        elif('listOutput' in configuration):
            filter = 'Output'
        elif('listDevice' in configuration):
            filter = configuration['listDevice']

        print "List devices:"
        print "\tFilter: '%s'" % (filter)
        print "\n",

        for device in devices:
            if('None' == filter):
                device.printMe()
            elif (
                    'Output' == filter
                    and device.doesSupportPlayback()
            ):
                device.printMe()
            elif (
                    'Input' == filter
                    and device.doesSupportRecording()
            ):
                device.printMe()
            elif(device.name == filter):
                device.printMe()

    except KeyError as e:
        print "ERROR: Could not find key %s." % (str(e))

        parser.print_help()


def playback():
    global args
    global parser
    global configuration

    try:
        playbackConfiguration = configuration['playback']

        deviceName = playbackConfiguration['deviceName']
        inputFile = playbackConfiguration['inputFile']
        duration = playbackConfiguration['duration']
        volume = playbackConfiguration['volume']
        delay = playbackConfiguration['delay']

        print "Playback:"
        print "\tDevice name:\t\t'%s'" % (deviceName)
        print "\tInput file:\t\t'%s'" % (inputFile)
        print "\tDuration:\t\t%d secs" % (duration)
        print "\tVolume:\t\t\t%.2f" % (volume)
        print "\tDelay:\t\t\t%.2f secs" % (delay)

        device = CAHALDevice.findDevice(deviceName)
        player = WAVPlayer(inputFile, volume, delay)

        if(device and player):
            if(device.doesSupportPlayback() and player.initPlayback(device)):
                player.play(device, duration)
            else:
                print "ERROR: Device '%s' does not support playback." \
                    % (deviceName)
        else:
            print "ERROR: Could not find device %s." % (deviceName)

    except KeyError as e:
        print "ERROR: Could not find key %s." % (str(e))

        parser.print_help()


def transmit():
    global configuration
    global parser

    try:
        transmitterInfo = configuration['transmitter']
        classInfo = transmitterInfo['class']

        (moduleName, className) = classInfo.split('.')

        module = __import__(moduleName)
        classObject = getattr(module, className)
        instance = classObject(transmitterInfo['attributes'])

        print instance.toString()

        instance.transmit()

    except KeyError as e:
        print "ERROR: Could not find key %s." % (str(e))

        parser.print_help()


def record():
    global args
    global parser
    global configuration

    try:
        recordConfiguration = configuration['record']

        deviceName = recordConfiguration['deviceName']
        outputFile = recordConfiguration['outputFile']
        duration = recordConfiguration['duration']
        bitDepth = recordConfiguration['bitDepth']
        numberOfChannels = recordConfiguration['numberOfChannels']
        sampleRate = recordConfiguration['sampleRate']

        print "Record:"
        print "\tDevice name:\t\t'%s'" % (deviceName)
        print "\tOutput file:\t\t'%s'" % (outputFile)
        print "\tDuration:\t\t%d" % (duration)
        print "Audio info:"
        print "\tBit depth:\t\t%d bits" % (bitDepth)
        print "\tNumber of channels:\t%d" % (numberOfChannels)
        print "\tSample rate:\t\t%d Hz" % (sampleRate)

        device = CAHALDevice.findDevice(deviceName)
        recorder  =                 \
            WAVRecorder(
                outputFile,
                numberOfChannels,
                bitDepth,
                sampleRate
            )

        if(device and recorder):
            if(device.doesSupportRecording() and recorder.initRecord(device)):
                recorder.record(device, duration)
            else:
                print "ERROR: Device '%s' does not support recording."  \
                    % (deviceName)
        else:
            print "ERROR: Could not find device %s." % (deviceName)

    except KeyError as e:
        print "ERROR: Could not find key %s." % (str(e))

        parser.print_help()


def receive():
    global configuration
    global parser

    try:
        receiverInfo = configuration['receiver']
        classInfo = receiverInfo['class']

        (moduleName, className) = classInfo.split('.')

        module = __import__(moduleName)
        classObject = getattr(module, className)
        instance = classObject(receiverInfo['attributes'])

        print instance.toString()

        instance.receive()

    except KeyError as e:
        print "ERROR: Could not find key %s." % (str(e))

        parser.print_help()


def main():
    global devices
    global args
    global debug

    handleInputArguments()

    setup()

    startTime = time.time()

    debug = Debug.getInstance(configuration)

    if(not debug):
        parser.print_help()

        sys.exit(-1)
    else:
        print debug.toString()

    if(configuration.has_key('listDevices')):
        listDevices()
    elif(configuration.has_key('record')):
        record()
    elif(configuration.has_key('playback')):
        playback()
    elif(configuration.has_key('transmitter')):
        transmit()
    elif(configuration.has_key('receiver')):
        receive()
    else:
        parser.print_help()

    duration = time.time() - startTime

    print "Ran for %.04f seconds." % (duration)

    terminate()

main()
