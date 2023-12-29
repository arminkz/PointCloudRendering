cmake_minimum_required(VERSION 3.20)

project(point-cloud-rendering CXX)

add_definitions(-D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS=1 -DCRT_SECURE_NO_WARNINGS=1 -DNOMINMAX)

find_package(fmt REQUIRED)
find_package(spdlog REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glew REQUIRED)
find_package(glm REQUIRED)
find_package(imgui REQUIRED)
find_package(implot REQUIRED)
find_package(tinyply REQUIRED)
find_package(lodepng REQUIRED)

# Handle Libraries from conan
set(LIBRARIES
        fmt::fmt
        spdlog::spdlog
        glfw
        GLEW::GLEW        
        glm::glm
        imgui::imgui
        implot::implot
        tinyply::tinyply
        lodepng::lodepng
)


#-------------------------------------------------------------------------------
# include directories from git submodules
#-------------------------------------------------------------------------------
include_directories(SYSTEM thirdparty/ImGuiFileDialog)

include_directories(src)

# Handle .cpp files (cmake recommends manually inputing each filename here) (i cant do that !)
file(GLOB SOURCES 
            src/*
            src/interface/*
            src/interface/fonts/*
            src/interface/imgui-impl/*
            src/utilities/*
            src/model/*
            src/loaders/*
            thirdparty/ImGuiFileDialog/*
)
# Handle .h files
set(INCLUDES ${INCLUDES} src)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDES})

# Copy shaders
add_custom_command(TARGET ${PROJECT_NAME} 
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders)

# Copy fonts
add_custom_command(TARGET ${PROJECT_NAME} 
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/fonts/ $<TARGET_FILE_DIR:${PROJECT_NAME}>/fonts)