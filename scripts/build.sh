#!/bin/bash

cd ..
conan profile detect --force
conan install . --output-folder=build --build=missing --settings=build_type=Debug -s compiler.cppstd=17
cd build
cmake -S ..  -B . -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE="conan_toolchain.cmake" -DCMAKE_BUILD_TYPE=Debug
cmake --build . -j 8