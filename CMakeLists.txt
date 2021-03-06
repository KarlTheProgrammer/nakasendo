#################################################################
#  Date             12/04/2019                                  #
#  Author           nChain's employees                          #
#                                                               #
#  Copyright (c) 2019 nChain Limited. All rights reserved       #
#################################################################

## How to build 
##
## Tool and dependencies requirements
##
## Run to build/test/install
##
## On Windows
##   cmake -G"Visual Studio 15 2017" -A x64 ..\SDKLibraries -DBUILD_WEBASSEMBLY=ON && cmake --build . --target ALL_BUILD --config Debug && cmake --build . --target ALL_BUILD --config Release && ctest -C Debug && ctest -C Release && cpack -G NSIS -C Release
## Windows alternative with MSBuild parallele:
##   cmake -G"Visual Studio 15 2017" -A x64 ..\SDKLibraries -DBUILD_WEBASSEMBLY=ON && msbuild SDKLibraries.sln -maxcpucount:4 /p:Configuration=Debug && msbuild SDKLibraries.sln -maxcpucount:4 /p:Configuration=Release
##   cmake -G"Visual Studio 15 2017" -A x64 ..\SDKLibraries -DBUILD_WEBASSEMBLY=ON && msbuild SDKLibraries.sln -v:q -maxcpucount:4 /p:Configuration=Debug && msbuild SDKLibraries.sln  -v:q -maxcpucount:4 /p:Configuration=Release
##
## On Linux (Ubuntu)
##   cmake ../SDKLibraries -DCMAKE_BUILD_TYPE=Debug -DBUILD_WEBASSEMBLY=ON -DCUSTOM_SYSTEM_OS_NAME=Ubuntu; time -p make -j8 ; ctest ; make install; cpack -G TGZ
##
## Packaging :
##   Windows : cpack -G NSIS -C Release ## Require to install NSIS
##             cpack --config CPackSourceConfig.cmake -G ZIP
##   Linus   : cpack -G TGZ ## use at cmake time to get the precise os type installer name : -DCUSTOM_SYSTEM_OS_NAME=Ubuntu
##             cpack --config CPackSourceConfig.cmake -G TGZ

cmake_minimum_required(VERSION 3.12)
project(SDKLibraries)

set(SDK_VERSION_MAJOR "0" CACHE INTERNAL "framework major version")
set(SDK_VERSION_MINOR "3" CACHE INTERNAL "framework minor version")
set(SDK_VERSION_PATCH "0" CACHE INTERNAL "framework patch version")

option(BUILD_WEBASSEMBLY "Build the web assembly components" OFF)
option(INCLUDE_CPACK "Include the CPACK components" ON) 
option(INCLUDE_SECP256K1_LIB "Include a build of bitcoin specific scep256k1 library" OFF)
option(BUILD_TESTS "Build the unit tests (needs boost)" ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)## Always include the current directory

if(MSVC)
  add_definitions(-D_SCL_SECURE_NO_WARNINGS -DNOMINMAX)
endif()

## Init CMake, find all external packages and eventually build them if necessary
include(cmake/SDKInit.cmake)
sdkInitCMake()

## Everything in core should be includable from everywhere
include_directories(${CMAKE_SOURCE_DIR}/src/core)
include_directories(${CMAKE_SOURCE_DIR}/depends/json)
include_directories(${CMAKE_SOURCE_DIR}/src/applications/TS_protobuf/cpp)

if(NOT BUILD_MISC_ONLY)## Define -DBUILD_MISC_ONLY=ON to build o only prototypes in misc 
    if(INCLUDE_SECP256K1_LIB)
        add_subdirectory(depends/secp256k1)
    endif(INCLUDE_SECP256K1_LIB)
    add_subdirectory(src)
    if(BUILD_TESTS)
        add_subdirectory(tests)
        add_subdirectory(examples)
    endif(BUILD_TESTS)
else()
    add_subdirectory(misc)
endif()

## On Widows, need to download NSIS https://nsis.sourceforge.io/Download
## Possible to do WIX generator for MSI installer
if(INCLUDE_CPACK)
    set(SDK_CPACK_CONFIG_IN "${CMAKE_SOURCE_DIR}/cmake/SDKLibrariesCPackConfig.cpack.in")
    set(SDK_CPACK_CONFIG "${CMAKE_BINARY_DIR}/SDKLibrariesCPackConfig.cpack")
    configure_file(${SDK_CPACK_CONFIG_IN} ${SDK_CPACK_CONFIG} @ONLY)
    install(FILES "ReleaseNote.md" DESTINATION "." COMPONENT Files)
    install(FILES "LICENCE.txt" DESTINATION "." COMPONENT Files)
    include(${SDK_CPACK_CONFIG})
    include(CPack)
endif(INCLUDE_CPACK)
