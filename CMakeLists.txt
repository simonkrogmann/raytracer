cmake_minimum_required (VERSION 3.0)
set(project raytracer)
project(${project})

set(use_utilgpu_compilation_options ON)
find_package(utilgpu REQUIRED)


find_package(Qt5Gui REQUIRED)

find_package(OpenMP)
if(OPENMP_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

add_subdirectory(source)
add_subdirectory(test)
