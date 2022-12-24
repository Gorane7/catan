cmake_minimum_required (VERSION 3.1)
project(Catan)

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL)

include_directories(include)
file(GLOB SOURCES "src/*.cpp")
add_executable(Catan ${SOURCES})

target_link_libraries(Catan glfw)
target_link_libraries(Catan GLEW)
target_link_libraries(Catan ${OPENGL_gl_LIBRARY})
