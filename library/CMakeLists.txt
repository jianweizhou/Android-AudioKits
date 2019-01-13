# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

set(SRC_FILES_ROOT src/main/cpp)

# locate include file
include_directories(${SRC_FILES_ROOT})
include_directories(${SRC_FILES_ROOT}/audiokits)
include_directories(${SRC_FILES_ROOT}/thirdparty/ffmpeg/include)

SET(SRC_LIST ${SRC_FILES_ROOT}/audiokits/com_jack_audiokits_library_AudioUtils.cpp
        ${SRC_FILES_ROOT}/audiokits/AudioDemuxer.cpp
        ${SRC_FILES_ROOT}/audiokits/AudioDecoder.cpp
        ${SRC_FILES_ROOT}/audiokits/AudioEncoder.cpp
        ${SRC_FILES_ROOT}/common/FFMpegUtils.cpp)

add_library( # Sets the name of the library.
             audiokits

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             ${SRC_LIST} )

# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )

set(distribution_DIR ${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI})
add_library( libavcodec
             SHARED
             IMPORTED )
set_target_properties( libavcodec
                       PROPERTIES IMPORTED_LOCATION
                       ${distribution_DIR}/libavcodec-57.so)
add_library( libavfilter
             SHARED
             IMPORTED )
set_target_properties( libavfilter
                       PROPERTIES IMPORTED_LOCATION
                       ${distribution_DIR}/libavfilter-6.so)
add_library( libavformat
             SHARED
             IMPORTED )
set_target_properties( libavformat
                       PROPERTIES IMPORTED_LOCATION
                       ${distribution_DIR}/libavformat-57.so)
add_library( libavutil
             SHARED
             IMPORTED )
set_target_properties( libavutil
                       PROPERTIES IMPORTED_LOCATION
                       ${distribution_DIR}/libavutil-55.so)
add_library( libpostproc
             SHARED
             IMPORTED )
set_target_properties( libpostproc
                       PROPERTIES IMPORTED_LOCATION
                       ${distribution_DIR}/libpostproc-54.so)
add_library( libswresample
             SHARED
             IMPORTED )
set_target_properties( libswresample
                       PROPERTIES IMPORTED_LOCATION
                       ${distribution_DIR}/libswresample-2.so)
add_library( libswscale
             SHARED
             IMPORTED )
set_target_properties( libswscale
                       PROPERTIES IMPORTED_LOCATION
                       ${distribution_DIR}/libswscale-4.so)

# show more debug info when building
set(CMAKE_VERBOSE_MAKEFILE on)

# specify c++ feature
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       audiokits
                       libavcodec
                       libavfilter
                       libavformat
                       libavutil
                       libpostproc
                       libswresample
                       libswscale
                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib} )