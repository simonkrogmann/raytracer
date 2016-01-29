set(sources
    Main.cpp
    Line.cpp
    Material.cpp
    Scene.cpp
    Plane.cpp
    SceneObject.cpp
    Sphere.cpp
    Vector.cpp
)

set(includes
    Line.h
    Material.h
    Plane.h
    Scene.h
    SceneObject.h
    Sphere.h
    Vector.h
)

set(libraries
    utilgpu::cpp utilgpu::qt
)

create_executable(${project} raytracer "${sources}" "${includes}" "${libraries}")
