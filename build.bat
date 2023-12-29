cd out/build
cmake ../.. -G "Visual Studio 17 2022" -DCMAKE_TOOLCHAIN_FILE="deps\conan\conan_toolchain.cmake" -DCMAKE_BUILD_TYPE=Debug
cmake --build . --config Debug