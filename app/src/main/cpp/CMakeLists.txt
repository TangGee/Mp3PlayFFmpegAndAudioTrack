cmake_minimum_required(VERSION 3.4.1)


include_directories("ffmpeg/include")



add_library( # Sets the name of the library.
             mp3decoder
             # Sets the library as a shared library.
             SHARED
             # Provides a relative path to your source file(s).
             Mp3Decoder.cpp )

