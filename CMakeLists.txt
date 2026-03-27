cmake_minimum_required(VERSION 3.15)
project(AlgoToolkit)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
file(GLOB IMGUI_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ImGui/imgui.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ImGui/imgui_draw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ImGui/imgui_tables.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ImGui/imgui_widgets.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ImGui/backends/imgui_impl_glfw.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/src/ImGui/backends/imgui_impl_opengl3.cpp
)


# Your source files
file(GLOB_RECURSE APP_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE APP_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.h)

# Create the executable
add_executable(${PROJECT_NAME} ${APP_SOURCES} ${IMGUI_SOURCES})

# Link dependencies and include directories
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${IMGUI_DIR}
    ${IMGUI_DIR}/backends
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw OpenGL::GL)

# macOS specific linking
if(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework OpenGL" "-framework Cocoa" "-framework IOKit" "-framework CoreVideo")
endif()