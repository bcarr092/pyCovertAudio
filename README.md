pyCovertAudio
=============

## Background

pyCovertAudio is the result of [my](http://www.site.uottawa.ca/~bcarr092/) PhD studies at the University of Ottawa. My research focused on defining and characterizing *air-gap covert channels* (i.e., illicit communication between disconnected systems) and *out-of-band covert channels* (i.e., *air-gap covert channels* that do not require hardware modification at the transmitter or receiver) as well as on building and testing *covert-acoustic channels* (i.e., *out-of-band covert channels* established using audio signals). The pyCovertAudio tool was also used to generate results for the following publications:

* *On Acoustic Covert Channels Between Air-Gapped Systems* (Foundations and Practice of Security, 2014 - Springer) and
* *On Characterizing and Measuring Out-of-Band Covert Channels* (ACM Workshop on Information Hiding and Multimedia Security, 2015 - ACM).

## Overview

pyCovertAudio is a Python tool used to modulate and demodulate data using the following digital communication schemes:

* [Frequency-Shift Keying (FSK)](https://en.wikipedia.org/wiki/Frequency-shift_keying),
* [Frequency-Hopping Spread Spectrum (FHSS)](https://en.wikipedia.org/wiki/Frequency-hopping_spread_spectrum), and
* [Orthogonal Frequency-Division Multiplexing (OFDM)](https://en.wikipedia.org/wiki/Orthogonal_frequency-division_multiplexing).

Specifically, pyCovertAudio can perform the following functions:

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
| Windows       | 7, 10                         | i386          |
| iOS           | 8                             | ARM           |
| Android       | android-19 (KitKat, 4.4.4)    | ARM           |

Configuration
=============

pyCovertAudio is powered by JSON configuration files. Sample configuration files can be found in the `src/pyCovertAudio/conf` directory. Each JSON file is briefly outlined here:

* **ListDevices.json**: Lists the audio input and output devices on the system
* **&lt;platform&gt;Playback.json**: Plays a WAV audio file on a specific platform. The key parameters in this file are:
    * **deviceName**: The audio output device to use to play the WAV file
    * **inputFile**: Path to the WAV file to play
* **&lt;platform&gt;Record.json**: Records samples into a WAV audio file on a specific platform. The key parameters in this file are:
    * **deviceName**: The audio input device to use to record audio samples
    * **outputFile**: Path to the WAV file that will be created and populated with the recorded audio
    * **duration**: Duration, in seconds, to record audio for
* **Transmitter.json**: Modulates a given text message using FSK and produces a WAV file. The key parameters in this file are:
    * **fileName**: Path to the WAV file that will be created containing the modulated message
    * **carrierFrequency**: The carrier frequency, in Hz, that the modulated symbol will be transmitted on (*Note*: Frequencies above 20 kHz cannot be heard by humans and most adults cannot hear frequencies above 18 kHz.).
    * **data**: The text message that will be modulated
    * **modifiers**: This section contains filter information. If the carrier frequency is modified this section must be updated according to the bandwidth requirements of the transmitted signal.
* **Receiver.json**: Demodulates a given WAV file using FSK to recreate the text message the audio file contains. The key parameters in this file are:
    * **fileName**: Path to the WAV file to demodulate
    * **carrierFrequency**: The carrier frequency, in Hz, that was used in Transmitter.json.
    * **modifiers**: This section contains filter information. If the carrier frequency is modified this section must be updated according to the bandwidth requirements of the transmitted signal.
* **[FHSS,OFDM]Transmitter.json**: Modulates a given text message using FHSS or OFDM, respectively, and produces a WAV file.
    * **fileName**: Path to the WAV file that will be created containing the modulated message
    * **minimumFrequency** and **maximumFrequency**: Given that both FHSS and OFDM are multicarrier modulation schemes, these parameters define the passband that the schemes can operate within.
    * **data**: The text message that will be modulated
    * **modifiers**: This section contains filter information. If minimumFrequency or maximumFrequency are modified this section must be updated accordingly.
* **[FHSS,OFDM]Receiver.json**: Demodulates a given WAV file using FHSS or OFDM, respectively, to recreate the text message the audio file contains.
    * **fileName**: Path to the WAV file to demodulate
    * **minimumFrequency** and **maximumFrequency**: Given that both FHSS and OFDM are multicarrier modulation schemes, these parameters define the passband that the schemes can operate within.
    * **modifiers**: This section contains filter information. If the carrier frequency is modified this section must be updated accordingly.


Building and Running
====================

##Dependencies

This project requires the following software to build/run:

* [CMake](https://cmake.org/)
* [Python](https://www.python.org/downloads/) (requires 2.7.x)
* [Perl](https://www.perl.org/)
* [Doxygen](http://www.stack.nl/~dimitri/doxygen/) (optional, required for generating documentation)

This project also includes the following GitHub projects as *git submodules*:

* [CPCommon](https://github.com/bcarr092/CPCommon)
* [CSignal](https://github.com/bcarr092/CSignal)
* [CAHAL](https://github.com/bcarr092/CAHAL)
* [DarwinHelper](https://github.com/bcarr092) (If you are building on Mac OS X)

Lastly, this project has been tested with the following toolchains:

* Xcode on Mac OS X (v10.9, v10.10, and v10.11)
* Visual Studio 2013
* Android (API level 19)

## Checkout

Since this project uses Git submodules, execute the following command to checkout it out:

```
git clone --recursive https://github.com/bcarr092/pyCovertAudio.git
```

## Running a Pre-Built Instance of pyCovertAudio

In the `bin` directory there are pre-built binary versions of the libraries (e.g., CAHAL, CPCommon, CSignal) that pyCovertAudio depends on covering the following platforms and architectures:

| Platform      | Architecture      |
|---------------|-------------------|
| MacOSX        | x86\_64           |
| MacOSX        | i386              |
| Windows       | i386              |
| iPhoneOS      | armv7             |
| Android       | arm               |

To execute pyCovertAudio on your platform using these pre-built libraries add the following paths to the *PYTHONPATH* environment variable:

* lib
* lib/&lt;platform&gt;/&lt;platform&gt;/&lt;arch&gt;

where `<platform>` and `<arch>` are set to the values corresponding to the line from the table above that matches your current system's platform and architecture. Once the *PYTHONPATH* environment variable has been set, execute the following commands:

```
cd bin
python pyCovertAudio.py -c conf/ListDevices.json
```

This should list all the input (i.e., recording) and output (i.e., playback) devices on your system.

*Note*: In order to execute the Windows version of the pre-built pyCovertAudio script the Visual Studio 2013 redistributable [run-time](https://www.microsoft.com/en-ca/download/details.aspx?id=40784) must be installed on your system.

## Building and Running on Your Current Platform

### Configuration

The initial cmake configuration, as well as the build process, require knowledge of the Python include and lib directories for the Python installation that matches the platform and architecture that pyCovertAudio is being built for. Setting the *PYTHON_** environment variables correctly is required in order to build all tests as well as the main library that pyCovertAudio.py relies upon.

Locate the appropriate Python include and lib directories and set the following three environment variables:

* *PYTHON_BIN*: Set this to the directory that contains the python binary (e.g., /usr/local/bin, C:\python27)
* *PYTHON_INCLUDE*: Set this to Python's include directory (e.g., C:\python27\include) that includes Python.h 
* *PYTHON_LIB*: Set this to Python's libs directory (e.g., C:\python27\libs) that includes Python's dynamic library (e.g., libpython2.7.dylib, python27.lib)

### Build

```
cd pyCovertAudio
mkdir build bin
cd build
cmake -DPLATFORM=<platform> -DTARGET=<target> -DARCHITECTURE=<arch> -DCMAKE_INSTALL_PREFIX=../bin -G "<generator>" -DCMAKE_BUILD_TYPE=Release ../src
```

Since, at this point, we are building for a specific target, the values for `<platform>`, `<target>`, and `<arch>` are not all that important. They become more important when cross-compiling this application, however (see below). As part of convention, `<platform>` should be set to the operating system (OS) that pyCovertAudio is being built for (e.g., Windows, MacOSX), `<target>` should be set to the version of the OS that pyCovertAudio is being built for, and `<arch>` should be set to the architecture that pyCovertAudio is being built for.

The value for `<generator>` should be set to the build system that will be used to actually build the pyCovertAudio application. For a complete list of the generators that are supported on your platform run the command:

```
cmake --help
```

The generators are listed at the end of cmake's help output. For a point of reference, pyCovertAudio has previously been built using the following generators:

* "Unix Makefiles" on Mac OS X
* "NMake Makefiles" on Windows 7 and 10

Assuming "Unix Makefiles" was used in place of `<generator>` simply run `make` to build the application. If "NMake Makefiles" was used run `nmake` (and use `nmake` in the subsequent instructions in place of `make`, as well). Otherwise, execute the build command for your chosen generator.

### Documentation and Tests

To build all the documentation for pyCovertAudio and its dependencies run `make doc`. The generated documentation will be placed in `../bin/doc`.

Automated tests can be executed in order to ensure that the dependant libraries (e.g., CAHAL and CSignal) are running correctly. Simply run `make test` to do so. Be forewarned, however, these tests will attempt to record and playback audio as well as execute a number of the signal processing functions of CSignal. Therefore, these tests can take some time to run!

### Running pyCovertAudio

To launch the pyCovertAudio application first run the command `make install` then add the following paths to the *PYTHONPATH* environment variable:

* lib
* lib/&lt;platform&gt;/&lt;target&gt;/&lt;arch&gt;

Once the *PYTHONPATH* environment variable has been set, execute the following commands:

```
cd ../bin
python pyCovertAudio.py -c conf/ListDevices.json
```

This should list all the input (i.e., recording) and output (i.e., playback) devices on your system.

While it is unconventional to have to set *PYTHONPATH* before executing a python script, this step is required because the build process for pyCovertAudio is designed around cmake's cross-compiling feature. When pyCovertAudio is built and `make install` is run, the platform-specific libraries are installed to:

```
..\bin\lib\<platform>\<target>\<arch>
```

While the platform-independent Python scripts are installed to `..\lib`. To deploy pyCovertAudio to a specific system the contents of the platform-independent `..\lib` directory, the contents of the platform-dependent `..\lib\<platform>\<target>\<arch>` directory, and the pyCovertAudio script, therefore, need to be copied to the destination directory. By copying this content to a single directory pyCovertAudio can be run without having to set the *PYTHONPATH* variable.

## Cross-Compiling

pyCovertAudio was built from the ground up to be run on multiple different platforms. The cmake tool makes this possible through its cross-compiling functionality. The instructions that follow allow pyCovertAudio to be built against Mac OS X, iOS, and Android platforms, targets, and architectures from within the Mac OS X platform. A convenience tool, `generateMakefile.pl`, can be found in the root of the pyCovertAudio repos to facilitate this process (*Note*: A TODO is to port this script to Windows).

### Assumptions

* To build for Mac OS X targets the Mac OS X toolchain must be install. Check that it is by running:

```xcrun --sdk macosx --show-sdk-path```

* To build for iOS targets the iOS toolchain must be installed. Check that it is by running:

```xcrun --sdk iphoneos --show-sdk-path```

* To build for Android targets the [Android NDK](https://developer.android.com/ndk/downloads/index.html) must be downloaded and unpacked

### Configuration

The following environment variables must also be set:

* *APPLE_TOOLCHAIN_DIR*: This must point to the Xcode 'Developer' directory. The 'Platforms' directory must be a sub-directory of the location pointed to by this environment variable. This variable must be set to build for Mac OS X and iOS targets.
* *ANDROID_TOOLCHAIN_DIR*: This must be set to the directory the Android NDK was extracted to. The 'platforms' directory must be a sub-directory of the location pointed to by this environment variable. This variable, along with *DEVELOPER_ROOT*, must be set to build Android targets.
* *DEVELOPER_ROOT*: The `generateMakefile.pl` script will automatically build all the Android toolchains contained in the *ANDROID_TOOLCHAIN_DIR* and store them in the location pointed to by this environment variable. This variable, along with *ANDROID_TOOLCHAIN_DIR*, must be set to build Android targets. 

Note that in addition to these environment variables the *PYTHON_BIN*, *PYTHON_LIB*, and *PYTHON_INCLUDE* environment variables (described above) must also be set.

### Generating Makefiles

To see a list of all the platforms that pyCovertAudio can be built for on your machine, run the following command:

```
perl generateMakefile.pl --mode=Release --source=<source directory> --platform
```

where `<source directory>` is the absolute path of the 'src' sub-directory of the pyCovertAudio repos.

Before generating the build Makefiles for the desired targets, create a build directory (`mkdir build`) and an install directory (`mkdir bin`). Also note that the default generator used by `generateMakefile.pl` is "Unix Makefiles." To change this use the appropriate switch (i.e., `-g <generator>`).

To build for the Mac OS X platform, v10.10 target, and x86_64 architecture, as an example, execute the following command

```
perl generateMakefile.pl --mode=Release --source=<source directory> --build=<build directory> --install=<install directory> --platform=MacOSX --target=MacOSX10.10 --architecture=x86_64
```

where `<source directory>`, `<build directory>`, and `<install directory>` are the absolute paths to the 'src' sub-directory of the pyCovertAudio repos, the build directory you created, and the install directory you created, respectively.

### Building

To build for a specific generated target first `cd <build directory>` then `cd` to the sub-directory of the target you would like to build. Each sub-directory of `<build directory>` has the following structure:

```
build-<generator>-<platform>-<target>-<arch>-<mode>
```

pyCovertAudio can be built and installed for the specific build target by executing:

```
make
make install
```

*Note 1*: In order for the build to successfully complete, *PYTHON_LIB* and *PYTHON_INCLUDE* must point to a location that contains the copy of Python that is built for the same target and architecture that pyCovertAudio is being built for.

*Note 2*: All pyCovertAudio build targets can be installed to the same directory since the installation script will generate the appropriate sub-directories for each platform, target, and architecture combination within `<install directory>`.

Bridging the Air-Gap
====================

To bridge the air-gap between two systems, the **transmitter** (i.e., the system that will transmit a coded message using audio signals) and the **receiver** (i.e., the system that will receive the audio signal then demodulate and decode the messaage) using pyCovertAudio, execute the following:

1. On the **transmitter** run:

..```
..python pyCovertAudio.py -c conf/Transmit.json
..```

2. On the **receiver** run:

..```
..python pyCovertAudio.py -c conf/<platform>Record.json
..```

3. _Within a few seconds of running the previous command_, run the following command on the **transmitter**:

..```
..python pyCovertAudio.py -c conf/<platform>Playback.json
..```

4. Once the **receiver** has stopped recording, run the following command on the **receiver**:

..```
..python pyCovertAudio.py -c conf/Receive.json
..```

5. Once the **receiver** has completed demodulating and decoding the transmitted message, you should see the following [text](https://en.wikipedia.org/wiki/Samuel_Morse) on your screen:

..```
..Decoded string: 'What hath God wrought?'
..```

Ensure that in the commands above you replace `<platform>` with the platform of either the **transmitter** or **receiver** depending on the instruction. Additionally, ensure that the **transmitter** completes step #3 before the **receiver**  completes step #2 to guarantee that the **receiver** received the whole audio message.

*Note*: The transmitted message is about 45 seconds long and has been modulated to increase the probability of successful transmission. As such, the transmission bit rate is low and the receiver will have to perform demodulation and error correction, which could take some time. Depending on the hardware you are using as the receiver step #4 could take upwards of a minute or two (and perhaps even longer if your **receiver** is a mobile device). 

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
