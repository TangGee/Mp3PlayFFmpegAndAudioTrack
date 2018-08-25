cmake_minimum_required(VERSION 3.4.1)


set(PREBUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/prebuild)


message(${PREBUILD_DIR}--------------------------------)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/ffmpeg/include")


add_library( # Sets the name of the library.
             mp3decoder
             # Sets the library as a shared library.
             SHARED
             # Provides a relative path to your source file(s).
             native-lib.cpp
             Mp3Decoder.cpp )

target_link_libraries( # Specifies the target library.
                       mp3decoder
                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib}
                       android
                       z
                       ${PREBUILD_DIR}/libavformat.a
                       ${PREBUILD_DIR}/libavcodec.a
                       ${PREBUILD_DIR}/libavfilter.a
                       ${PREBUILD_DIR}/libavutil.a
                       ${PREBUILD_DIR}/libpostproc.a
                       ${PREBUILD_DIR}/libswresample.a
                       ${PREBUILD_DIR}/libswscale.a
                       )