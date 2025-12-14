cmake_minimum_required(VERSION 3.20)
project(UniverseSimulation)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

include_directories(/opt/homebrew/include)
link_directories(/opt/homebrew/lib)

add_executable(UniverseSimulation main.cpp)

target_link_libraries(UniverseSimulation
        OpenGL::GL
        glfw
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
)
