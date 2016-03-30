pyCovertAudio
#############

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
#####

## Checkout

Note that this project uses Git submodules

```
git clone --recursive https://github.com/bcarr092/pyCovertAudio.git
```

## Build

```
cd pyCovertAudio
mkdir build bin

```

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
