// This file is used to avoid including common libraries in each file
#pragma once

#define GLEW_STATIC
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#define IMGUI_DEFINE_MATH_OPERATORS


// [Libraries]
// Don't swap order between GL and GLFW includes!
#include "GL/glew.h"								
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "spdlog/spdlog.h"
#include "imgui.h"
#include "implot.h"
#include "interface/imgui-impl/imgui_impl_glfw.h"
#include "interface/imgui-impl/imgui_impl_opengl3.h"


// [Standard libraries: basic]
#include <algorithm>
#include <cmath>
#include <chrono>
#include <cstdint>
#include <execution>
#include <fstream>
#include <functional>
#include <iostream>
#include <memory>
#include <numeric>
#include <random>
#include <sstream>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <thread>
#include <filesystem>
#include <experimental/filesystem>
#include <streambuf>

// [Standard libraries: data structures]
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <vector>