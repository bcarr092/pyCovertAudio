pyCovertAudio
=============

pyCovertAudio is the result of [my](http://www.site.uottawa.ca/~bcarr092/) PhD studies at the University of Ottawa. My research focussed on defining and characterizing *air-gap covert channels* (i.e., illict communication between disconnected systems) and *out-of-band covert channels* (i.e., *air-gap covert channels* that do not require hardware modification) as well as on building and testing *covert-acoustic channels* (i.e., *out-of-band covert channels* established using audio signals). This tool was used to generate results for the following publications:

* *On Acoustic Covert Channels Between Air-Gapped Systems* (Foundations and Practice of Security, 2014 - Springer) and
* *On Characterizing and Measuring Out-of-Band Covert Channels* (ACM Workshop on Information Hiding and Multimedia Security, 2015 - ACM).

pyCovertAudio has the ability to modulate and demodulate data using the following digital communication schemes:

* [Frequency-Shift Keying (FSK)](https://en.wikipedia.org/wiki/Frequency-shift_keying),
* [Frequency-Hopping Spread Spectrum (FHSS)](https://en.wikipedia.org/wiki/Frequency-hopping_spread_spectrum), and
* [Orthoganal Frequency-Division Multiplexing (OFDM)](https://en.wikipedia.org/wiki/Orthogonal_frequency-division_multiplexing),

as well as perform the following functions:

* list a system's audio input (i.e., microphones) and audio output (i.e., speakers) devices and their supported formats,
* play a given WAV file using a specified audio output device,
* record audio into a WAV file using a specified audio input device,
* modulate a text message into a WAV file, and
* demodulate a given WAV file to recreate a modulated message.

## Tested Platforms, Targets, and Architectures

pyCovertAudio has been tested on the following platforms and architectures:

| Platform      | Target                        | Architecture  |
|---------------|-------------------------------|---------------|
| Mac OS X      | 10.9, 10.10, 10.11            | x86\_64       |
| Windows       | 7, 10                         | x86           |
| iOS           | 8                             | ARM           |
| Android       | android-19 (KitKat, 4.4.4)    | ARM           |

Configuration
=============

pyCovertAudio is powered by JSON configuration files. Sample configuration files can be found in the `src/pyCovertAudio/conf` directory. They are briefly outlined here:

* **ListDevices.json**: Lists the audio input and output devices on the system
* **&lt;platform&gt;Playback.json**: Plays a WAV audio file. The key parameters in this file are:
    * **deviceName**: The audio output device to use to play the WAV file
    * **inputFile**: Path to the WAV file to play
* **&lt;platform&gt;Record.json**: Records samples into a WAV audio file. The key parameters in this file are:
    * **deviceName**: The audio input device to use to record audio samples
    * **outputFile**: Path to the WAV file that will be created and populated with the recorded audio
    * **duration**: Duration, in seconds, to record audio for
* **Transmitter.json**: Modulates a given text message using FSK and produces a WAV file. The key parameters in this file are:
    * **fileName**: Path to the WAV file that will be created containing the modulated message
    * **carrierFrequency**: The carrier frequency, in Hz, that the modulated symbol will be transmitted on (*Note*: Frequencies above 20 kHz cannot be heard by humans and adults have trouble hearing frequencies above 18 kHz.).
    * **data**: The text message that will be modulated
    * **modifiers**: This section contains filter information. If the carrier frequency is modified this section must be updated accordingly.
* **Receiver.json**: Demodulates a given WAV file using FSK to recreate the text message the audio file contains. The key parameters in this file are:
    * **fileName**: Path to the WAV file to demodulate
    * **carrierFrequency**: The carrier frequency, in Hz, that was used in Transmitter.json.
    * **modifiers**: This section contains filter information. If the carrier frequency is modified this section must be updated accordingly.
* **[FHSS,OFDM]Transmitter.json**: Modulates a given text message using FHSS or OFDM, respectively, and produces a WAV file.
    * **fileName**: Path to the WAV file that will be created containing the modulated message
    * **minimumFrequency** and **maximumFrequency**: Given that both FHSS and OFDM are multicarrier modulation schemes, these parameters define the passband that the schemes can use.
    * **data**: The text message that will be modulated
    * **modifiers**: This section contains filter information. If minimumFrequency or maximumFrequency are modified this section must be updated accordingly.
* **[FHSS,OFDM]Receiver.json**: Demodulates a given WAV file using FHSS or OFDM, respectively, to recreate the text message the audio file contains.
    * **fileName**: Path to the WAV file to demodulate
    * **minimumFrequency** and **maximumFrequency**: Given that both FHSS and OFDM are multicarrier modulation schemes, these parameters define the passband that the schemes can use.
    * **modifiers**: This section contains filter information. If the carrier frequency is modified this section must be updated accordingly.


Building and Running
====================

##Dependencies

This project requires the following software to build/run:

* [CMake](https://cmake.org/)
* [Python](https://www.python.org/downloads/) (requires 2.7.x)
* [Perl](https://www.perl.org/)
* [Doxygen](http://www.stack.nl/~dimitri/doxygen/) (optional, required for generating documentation)

This project includes the following GitHub projects:

* [CPCommon](https://github.com/bcarr092/CPCommon)
* [CSignal](https://github.com/bcarr092/CSignal)
* [CAHAL](https://github.com/bcarr092/CAHAL)
* [DarwinHelper](https://github.com/bcarr092) (If you are building on Mac OS X)

Lastly, this project has been tested with the following toolchains:

* Xcode on Mac OS X (v10.9, v10.10, and v10.11)
* Visual Studio 2013
* Android (API level 19)

##Building for Your Current Platform

### Checkout

Note that this project uses Git submodules. As such, execute the following command to checkout this project:

```
git clone --recursive https://github.com/bcarr092/pyCovertAudio.git
```

### Configuration

The cmake configuration as well as the build process require knowledge of the Python include and lib directories for the Python installation that matches the platform and architecture that pyCovertAudio is being built for. Setting these environment variables correctly is required in order to build all tests as well as the main library that pyCovertAudio.py relies upon.

Locate the appropriate Python include and lib directories and set the following three environment variables:

* **PYTHON_BIN**: Set this to the directory that contains the python binary (e.g., /usr/local/bin, C:\python27)
* **PYTHON_INCLUDE**: Set this to Python's include directory (e.g., C:\python27\include) that includes Python.h 
* **PYTHON_LIB**: Set this to Python's libs directory (e.g., C:\python27\libs) that includes Python's dynamic library (e.g., libpython2.7.dylib, python27.lib)

### Build

```
cd pyCovertAudio
mkdir build bin
cd build
cmake -DPLATFORM=<platform> -DTARGET=<target> -DARCHITECTURE=<arch> -DCMAKE_INSTALL_PREFIX=../bin -G "<generator>" -DCMAKE_BUILD_TYPE=Release ../src
```

Since, at this point, we are building for a specific target the values for `<platform>`, `<target>`, and `<arch>` are not all that important. They become more important when cross-compiling this application, however (see below).

The value for `<generator>` should be set to the build system that will be used to actually build the pyCovertAudio application. For a complete list of the generators that are supported on your platform run the command:

```
cmake --help
```

The generators are listed at the end of cmake's help output. pyCovertAudio has been built using the following generators:

* "Unix Makefiles" on Mac OS X
* "NMake Makefiles" on Windows 7 and 10

If "Unix Makefiles" was used in place of `<generator>` simply run `make` to build the application. If "NMake Makefiles" was used run `nmake`. Otherwise, execute the build command for your generator.

### Documentation and Tests

To build all the documentation for pyCovertAudio and its dependencies run `make doc`. The generated documentation will show up in `../bin/doc`.

To execute tests to ensure the dependant libraries (e.g., CAHAL and CSignal) are running correctly run `make test`.

### Running pyCovertAudio

To launch the pyCovertAudio application first run the command `make install` then add the following paths to the **PYTHONPATH** environment variable:

* lib
* lib/&lt;platform&gt;/&lt;target&gt;/&lt;arch&gt;

Once environment variable has been set execute the following commands:

```
cd ../bin
python pyCovertAudio.py -c conf/ListDevices.json
```

This should list all the input (i.e., recording) and output (i.e., playback) devices on your system.

Contributors
============
Brent Carrara (bcarr092@uottawa.ca)

License
=======

   Copyright 2016 Brent Carrara 

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
