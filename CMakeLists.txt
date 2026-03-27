cmake_minimum_required(VERSION 3.15)
project(AlgoToolkit)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Suppress macOS OpenGL deprecation warnings
if(APPLE)
    add_definitions(-DGL_SILENCE_DEPRECATION)
endif()

# Fetch GLFW automatically
include(FetchContent)
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.3.8
)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(glfw)

# Find OpenGL (Cross-platform)
find_package(OpenGL REQUIRED)

# Setup ImGui sources
set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/ImGui)
set(IMGUI_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ImGui/imgui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ImGui/imgui_draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ImGui/imgui_tables.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ImGui/imgui_widgets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ImGui/backends/imgui_impl_glfw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ImGui/backends/imgui_impl_opengl3.cpp
)

# App sources
set(APP_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Menu/Menu.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Menu/ImGuiHelper.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Visualizer/Visualizer.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Algorithms/SortingAlgorithm.cpp
)

# Headers
set(APP_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Menu/Menu.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Visualizer/Visualizer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/src/Algorithms/SortingAlgorithm.h
)

# Create the executable
add_executable(${PROJECT_NAME} ${APP_SOURCES} ${IMGUI_SOURCES})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link dependencies
target_link_libraries(${PROJECT_NAME} PRIVATE glfw OpenGL::GL)

# macOS specific linking
if(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
endif()
