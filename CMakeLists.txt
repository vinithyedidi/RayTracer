cmake_minimum_required(VERSION 3.28)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DCMAKE_OSX_ARCHITECTURES=arm64")
project(Renderer)
set(CMAKE_CXX_STANDARD 23)

if (APPLE)
    set(CMAKE_CXX_FLAGS "-framework Cocoa -framework IOKit -framework OpenGL -framework CoreVideo")
    add_compile_definitions(GL_SILENCE_DEPRECATION)
endif ()

include_directories(
        ${PROJECT_SOURCE_DIR}/include)
link_directories(
        ${PROJECT_SOURCE_DIR}/lib)

set(SOURCES
        src/main.cpp
        src/render.cpp
        src/poop.cpp
)
add_executable(Renderer src/main.cpp)

target_link_libraries(Renderer
        libglfw3.a)