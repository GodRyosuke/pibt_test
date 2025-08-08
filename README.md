# install
```bash
conan profile detect --force
conan install . --output-folder=build --build=missing --settings=build_type=Debug -s compiler.cppstd=17
cd build
cmake -S ..  -B . -G "Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE="conan_toolchain.cmake" -DCMAKE_BUILD_TYPE=Debug
cmake --build . -j 8

# for windows
# cmake -S ..  -B . -G "Visual Studio 17 2022" -DCMAKE_TOOLCHAIN_FILE="conan_toolchain.cmake"
# cmake --build . --config Debug
```

# Note
## Windows11 WSL2
Please replace conanfile.txt to below and remove glfw from "requires".
```
[requires]
assimp/5.4.3
boost/1.88.0

[generators]
CMakeDeps
CMakeToolchain
```

Install dependencies with "apt install"
```bash
sudo apt-get install -y libx11-dev xorg-dev \
                          libglu1-mesa libglu1-mesa-dev \
                          libgl1-mesa-glx libgl1-mesa-dev
sudo apt install -y libglfw3 libglfw3-dev
```