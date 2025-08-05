chcp 65001
cd ..
conan profile detect --force
conan install . --output-folder=build --build=missing --settings=build_type=Debug -s compiler.cppstd=17
cd build
cmake -S ..  -B . -G "Visual Studio 17 2022" -DCMAKE_TOOLCHAIN_FILE="conan_toolchain.cmake"
cmake --build . --config Debug