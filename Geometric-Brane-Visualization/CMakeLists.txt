cmake_minimum_required(VERSION 3.20)
project(bulkBrane)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

#mac/homebrew Paths
include_directories(/opt/homebrew/include)
link_directories(/opt/homebrew/lib)

add_executable(bulkBrane brane_sim.cpp)

target_link_libraries(bulkBrane
        OpenGL::GL
        glfw
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
)
