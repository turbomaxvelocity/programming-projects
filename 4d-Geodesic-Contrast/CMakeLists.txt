cmake_minimum_required(VERSION 3.20)
project(4d_Geodesic_Contrast)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

# Mac/Homebrew Paths
include_directories(/opt/homebrew/include)
link_directories(/opt/homebrew/lib)

add_executable(4d_Geodesic_Contrast sine_wave_geodesic.cpp)

target_link_libraries(4d_Geodesic_Contrast
        OpenGL::GL
        glfw
        # Linking required for gluPerspective:
        "-framework OpenGL"
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
)
