
set( PROJECT_NAME "pyCovertAudio_lib" )

project( ${PROJECT_NAME} )

message( STATUS "Project: ${PROJECT_NAME}" )
message( STATUS "Project directory: ${PROJECT_SOURCE_DIR}" )

set( SOURCE_DIR "${PROJECT_SOURCE_DIR}" )
set( INCLUDE_DIR "${SOURCE_DIR}/include" )
set( INSTALL_DIR "${CMAKE_INSTALL_PREFIX}" )

message( STATUS "Source directory: ${SOURCE_DIR}" )
message( STATUS "Include directory: ${INCLUDE_DIR}" )
message( STATUS "Install directory: ${INSTALL_DIR}" )

set( CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH )
set( CMAKE_FIND_ROOT_PATH_MODE_LIBRARY BOTH )
set( CMAKE_FIND_ROOT_PATH_MODE_INCLUDE BOTH )

find_package( SWIG REQUIRED )

set( CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER )
set( CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY )
set( CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY )

include( ${SWIG_USE_FILE} )

set( CMAKE_SWIG_FLAGS "-threads" )

set( CPCOMMON_INCLUDE_DIRECTORY   "${cpcommon_SOURCE_DIR}/src/include"    )
set( CAHAL_INCLUDE_DIRECTORY      "${cahal_SOURCE_DIR}/src/include"       )
set( CAHAL_TEST_INCLUDE_DIRECTORY "${cahal_tests_SOURCE_DIR}/src/include"  )
set( CSIGNAL_INCLUDE_DIRECTORY      "${csignal_SOURCE_DIR}/src/include"       )
set( CSIGNAL_TEST_INCLUDE_DIRECTORY "${csignal_tests_SOURCE_DIR}/src/include"  )

if(
    DEFINED ENV{PYTHON_INCLUDE}
    AND DEFINED ENV{PYTHON_LIB}
    AND DEFINED ENV{PYTHON_BIN}
  )
  set( PYTHON_INCLUDE $ENV{PYTHON_INCLUDE}  )
  set( PYTHON_LIB     $ENV{PYTHON_LIB}      )
  set( PYTHON_BIN     $ENV{PYTHON_BIN}      )
else()
  message (
    FATAL_ERROR
    "Environment variables PYTHON_INCLUDE, PYTHON_LIB, PYTHON_BIN must be set."
          )
endif()

if( "${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin" )
  set (
        PYTHON_LIBRARIES
        "${PYTHON_LIB}/libpython.dylib"
      )
  set (
        PYTHON_BINARY
        "${PYTHON_BIN}/python"
      )
elseif( ${PLATFORM} STREQUAL "Generic" AND ${TARGET} MATCHES "^android" )
  set (
        PYTHON_LIBRARIES
        "${PYTHON_LIB}/libpython.so"
      )
  set (
        PYTHON_BINARY
        "${PYTHON_BIN}/python"
      )
elseif( ${CMAKE_SYSTEM_NAME} STREQUAL "Windows" )
  string( REPLACE "\\" "/" PYTHON_INCLUDE ${PYTHON_INCLUDE} )
  string( REPLACE "\\" "/" PYTHON_LIB ${PYTHON_LIB} )
  string( REPLACE "\\" "/" PYTHON_BIN ${PYTHON_BIN} )

  set (
        PYTHON_LIBRARIES
        "${PYTHON_LIB}/python27.lib"
      )
  set (
        PYTHON_BINARY
        "${PYTHON_BIN}/python.exe"
      )
else()
  message( FATAL_ERROR "Unsupported system: ${CMAKE_SYSTEM_NAME}" )
endif()

include_directories( ${CPCOMMON_INCLUDE_DIRECTORY} )

include_directories( ${CAHAL_INCLUDE_DIRECTORY} )
include_directories( ${CAHAL_TEST_INCLUDE_DIRECTORY} )

include_directories( ${CSIGNAL_INCLUDE_DIRECTORY} )
include_directories( ${CSIGNAL_TEST_INCLUDE_DIRECTORY} )

include_directories( ${PYTHON_INCLUDE} )

include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )

message( STATUS "Python library: ${PYTHON_LIBRARIES}" )
message( STATUS "Python binary: ${PYTHON_BINARY}" )
message( STATUS "Python includes: ${PYTHON_INCLUDE}" )

set( SOURCES "${cahal_tests_SOURCE_DIR}/src/cahal_wrapper.c" )
list( APPEND SOURCES "${csignal_tests_SOURCE_DIR}/src/csignal_wrapper.c" )

set( HEADERS "${CAHAL_TEST_INCLUDE_DIRECTORY}/cahal_wrapper.h" )
list( APPEND HEADERS "${CSIGNAL_TEST_INCLUDE_DIRECTORY}/csignal_wrapper.h" )

message( STATUS "C source files found: ${SOURCES}" )
message( STATUS "C header files found: ${HEADERS}" )

swig_add_module (
  ${PROJECT_NAME}
  python
  "${PROJECT_NAME}.i"
  ${SOURCES}
  ${HEADERS}
                )

swig_link_libraries( ${PROJECT_NAME} ${PYTHON_LIBRARIES} cpcommon cahal csignal )

set( PY_BIN "${SOURCE_DIR}/pyCovertAudio.py" )

set( PY_LIB "${SOURCE_DIR}/Debug.py" )
list( APPEND PY_LIB "${SOURCE_DIR}/BFSKDemodulator.py" )
list( APPEND PY_LIB "${SOURCE_DIR}/BFSKModulator.py" )
list( APPEND PY_LIB "${SOURCE_DIR}/BandpassFilter.py" )
list( APPEND PY_LIB "${SOURCE_DIR}/Base.py" )
list( APPEND PY_LIB "${SOURCE_DIR}/BaseDemodulator.py" )
list( APPEND PY_LIB "${SOURCE_DIR}/BaseEncoder.py" )
list( APPEND PY_LIB "${SOURCE_DIR}/BaseModifier.py" )
list( APPEND PY_LIB "${SOURCE_DIR}/BaseModulator.py" )
list( APPEND PY_LIB "${SOURCE_DIR}/BaseReceiver.py" )
list( APPEND PY_LIB "${SOURCE_DIR}/BaseTransmitter.py" )
list( APPEND PY_LIB "${SOURCE_DIR}/BitPacker.py" )
list( APPEND PY_LIB "${SOURCE_DIR}/BitStream.py" )
list( APPEND PY_LIB "${SOURCE_DIR}/CAHALAudioFormatDescription.py" )
list( APPEND PY_LIB "${SOURCE_DIR}/CAHALDevice.py" )
list( APPEND PY_LIB "${SOURCE_DIR}/CAHALDeviceStream.py" )
list( APPEND PY_LIB "${SOURCE_DIR}/CAHALSampleRateRange.py" )
list( APPEND PY_LIB "${SOURCE_DIR}/EncoderFactory.py" )
list( APPEND PY_LIB "${SOURCE_DIR}/FHSSDemodulator.py" )
list( APPEND PY_LIB "${SOURCE_DIR}/FHSSModulator.py" )
list( APPEND PY_LIB "${SOURCE_DIR}/ManchesterEncoder.py" )
list( APPEND PY_LIB "${SOURCE_DIR}/ModifierFactory.py" )
list( APPEND PY_LIB "${SOURCE_DIR}/ModulatorFactory.py" )
list( APPEND PY_LIB "${SOURCE_DIR}/OFDMDemodulator.py" )
list( APPEND PY_LIB "${SOURCE_DIR}/OFDMModulator.py" )
list( APPEND PY_LIB "${SOURCE_DIR}/ReedSolomon.py" )
list( APPEND PY_LIB "${SOURCE_DIR}/ReedSolomonEncoder.py" )
list( APPEND PY_LIB "${SOURCE_DIR}/SignalFunctions.py" )
list( APPEND PY_LIB "${SOURCE_DIR}/SymbolTracker.py" )
list( APPEND PY_LIB "${SOURCE_DIR}/TestFactory.py" )
list( APPEND PY_LIB "${SOURCE_DIR}/WAVMultiCarrierReceiver.py" )
list( APPEND PY_LIB "${SOURCE_DIR}/WAVMultiCarrierTransmitter.py" )
list( APPEND PY_LIB "${SOURCE_DIR}/WAVPlayer.py" )
list( APPEND PY_LIB "${SOURCE_DIR}/WAVReceiver.py" )
list( APPEND PY_LIB "${SOURCE_DIR}/WAVRecorder.py" )
list( APPEND PY_LIB "${SOURCE_DIR}/WAVTransmitter.py" )

set ( WRAPPERS "${PROJECT_BINARY_DIR}/${PROJECT_NAME}.py" )

message( STATUS "Copying scripts: ${SCRIPTS}" )
message( STATUS "Copying wrappers: ${WRAPPERS}" )

foreach( FILE_TO_COPY IN ITEMS ${SCRIPTS} )
    message( STATUS "Copying script ${FILE_TO_COPY} to ${PROJECT_BINARY_DIR}" )
    
    add_custom_command( TARGET "_${PROJECT_NAME}" PRE_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                        "${FILE_TO_COPY}" "${PROJECT_BINARY_DIR}" )
                        
endforeach( FILE_TO_COPY )

install(  TARGETS "_${PROJECT_NAME}" DESTINATION
          "${INSTALL_DIR}/lib" )
install(  PROGRAMS ${PY_BIN} DESTINATION
          "${INSTALL_DIR}" )
install(  PROGRAMS ${PY_LIB} DESTINATION
          "${INSTALL_DIR}/lib" )
install(  FILES ${WRAPPERS} DESTINATION
          "${INSTALL_DIR}/lib" )
install(  DIRECTORY "conf/" DESTINATION
          "${INSTALL_DIR}/conf" )

if( "${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin" )
  find_library( CORE_FOUNDATION_FRAMEWORK CoreFoundation )
  find_library( CORE_AUDIO_FRAMEWORK CoreAudio )
  find_library( AUDIO_TOOLBOX_FRAMEWORK AudioToolbox )

  set ( EXTRA_LIBS
        ${CORE_FOUNDATION_FRAMEWORK} ${CORE_AUDIO_FRAMEWORK}
        ${AUDIO_TOOLBOX_FRAMEWORK}
      )

  message( STATUS "Framework libraries: ${EXTRA_LIBS}" )

  swig_link_libraries( ${PROJECT_NAME} ${EXTRA_LIBS} )
elseif( ${PLATFORM} STREQUAL "Generic" AND ${TARGET} MATCHES "^android" )
  find_library  (
                  OPENSLES_LIB
                  OpenSLES
                  HINTS "${CMAKE_FIND_ROOT_PATH}/usr/lib"
                )
  find_library  (
                  MATH_LIB
                  m  
                  HINTS "${CMAKE_FIND_ROOT_PATH}/usr/lib"
                )

  set (
        EXTRA_LIBS
        ${OPENSLES_LIB}
        ${MATH_LIB}
      )

  message( STATUS "Android libraries: ${EXTRA_LIBS}" )

  swig_link_libraries( ${PROJECT_NAME} ${EXTRA_LIBS} )
elseif( ${CMAKE_SYSTEM_NAME} STREQUAL "Windows" )
else()
  message( FATAL_ERROR "Unsupported system: ${CMAKE_SYSTEM_NAME}" )
endif()
