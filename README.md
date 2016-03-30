pyCovertAudio
=============

Dependencies
============

This project requires the following software to build/run:

* [CMake](https://cmake.org/)
* [Python](https://www.python.org/downloads/) (requires 2.7.x)
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

Build
=====

### Checkout

Note that this project uses Git submodules

```
git clone --recursive https://github.com/bcarr092/pyCovertAudio.git
```

### Configuration

The cmake configuration as well as the build process require knowledge of where the Python include and lib directories are in order to build both the tests as well as the pyCovertAudio libraries. Locate both of these directories and set three environment variables:

* PYTHON_BIN: Set this to the directory that contains the python binary (e.g., /usr/local/python, C:\python27)
* PYTHON_INCLUDE: Set this to Python's include directory (e.g., C:\python27\include) that includes Python.h 
* PYTHON_LIB: Set this to Python's libs directory (e.g., C:\python27\libs) that includes Python's dynamic library (e.g., libpython2.7.dylib, python27.lib)

### Build

```
cd pyCovertAudio
mkdir build bin
cd build
cmake -DPLATFORM=<platform> -DTARGET=<target> -DARCHITECTURE=<arch> -DCMAKE_INSTALL_PREFIX=../bin -G "<generator>" -DCMAKE_BUILD_TYPE=Release ../src
```

Since, at this point, we are building for a specific target the values for <platform>, <target>, and <arch> are not all that important. They become more important when cross-compiling this library, however (see below)

The value for <generator> should be set to the build system that will be used to actually compile this application. For a complete list of the generators that are supported on your platform run the command

```
cmake --help
```

The generators are listed at the end of cmake's help output. Building this application has been tested with the following parameters:

* "Unix Makefiles" on Mac OS X
* "NMake Makefiles" on Windows 7 and 10

If "Unix Makefiles" was used in place of <generator> simply run `make` to build the application (Note: If "NMake Makefiles" was used replace `make` with `nmake` everywhere):

To build all the documentation for this project and its dependencies run `make doc`.

To execute tests to ensure the dependant libraries (e.g., CAHAL and CSignal) are running correctly run `make test`.

To run the pyCovertAudio application run the following commands:

```
make install
cd ../bin
set PYTHONPATH=%PYTHONPATH%;lib;lib/<platform>/<target>/<arch>
python pyCovertAudio.py -c conf\ListDevices.json
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
