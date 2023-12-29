# PointCloudRendering
A point cloud renderer using splatting method.

---
### Build Instructions (Windows)

1. Make sure you have Python installed.

2. Install `poetry`
```shell
pip install poetry
```

3. Activate virtual environment in the project folder:
```shell
poetry shell
poetry update
```

4. Install Conan dependencies:
```shell
conan install . -sbuild_type=Debug -of=out/build/deps/conan/ --build=missing
```

5. Build using CMake:
```shell
cd out/build
cmake ../.. -G "Visual Studio 17 2022" -DCMAKE_TOOLCHAIN_FILE="deps\conan\conan_toolchain.cmake" -DCMAKE_BUILD_TYPE=Debug
cmake --build . --config Debug
```
   
