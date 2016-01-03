set(sources
    Main.cpp
    Line.cpp
    Line.h
    Material.cpp
    Material.h
    Plane.cpp
    Plane.h
    Scene.cpp
    Scene.h
    SceneObject.cpp
    SceneObject.h
    Sphere.cpp
    Sphere.h
    Vector.cpp
    Vector.h
)

add_executable(raytracer ${sources})

target_compile_options(raytracer PRIVATE ${DEFAULT_COMPILE_FLAGS})

add_custom_target(format-raytracer
    COMMAND clang-format -i ${sources}
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    COMMENT "Formatting code"
    VERBATIM)

add_dependencies(raytracer format-raytracer)
