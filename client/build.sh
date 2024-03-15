#!/usr/bin/env sh

# MacOS/Linux cmake file

conan install . --build=missing -of=build
cd build
cmake .. -DCMAKE_TOOLCHAIN_FILE=conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build .