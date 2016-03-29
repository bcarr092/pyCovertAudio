cmake_minimum_required( VERSION 3.0 )

set( PROJECT_NAME "pyCovertAudio" )

project( ${PROJECT_NAME} )

if( ${PLATFORM} STREQUAL "Generic" AND ${TARGET} MATCHES "^android" )
  set_property(GLOBAL PROPERTY TARGET_SUPPORTS_SHARED_LIBS TRUE)
endif()

message( STATUS "Project: ${PROJECT_NAME}" )
message( STATUS "Project directory: ${PROJECT_SOURCE_DIR}" )

message( STATUS "Building for platform ${PLATFORM}" )

add_subdirectory( cpcommon )

if( ${CMAKE_SYSTEM_NAME} STREQUAL "Darwin" )
  add_subdirectory( darwinhelper )
endif()

add_subdirectory( cahal )
add_subdirectory( csignal )
add_subdirectory( pyCovertAudio )

if( ${CMAKE_SYSTEM_NAME} STREQUAL "Darwin" )
  add_custom_target (
    doc DEPENDS
    doc_csignal
    doc_cpcommon
    doc_darwinhelper
    doc_cahal
                    )
else()
  add_custom_target( doc DEPENDS doc_csignal doc_cpcommon doc_cahal )
endif()

enable_testing()
