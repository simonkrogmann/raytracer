#include <iostream>
#include <fstream>
#include <cstdlib>
#include <memory>

#include <utilgpu/cpp/cfl.h>
#include <QImage>

#include "Scene.h"
#include "Sphere.h"
#include "Plane.h"
#include "SceneObject.h"
#include "Material.h"
#include "Vector.h"

#define COLORS 3

void save(const std::vector<vec3<float>>& map, const int& width,
          const int& height, const std::string& filename);

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cout << "Usage: " << argv[0] << " <scene> <output>" << std::endl;
        return 1;
    }

    auto document = util::parseCFL(argv[1]);
    if (!document->valid())
    {
        std::cout << document->message() << std::endl;
        return 2;
    }
    auto& root = *document;
    auto& resolution = *root["resolution"];
    auto width = static_cast<unsigned int>(resolution["width"]->value(1920));
    auto height = static_cast<unsigned int>(resolution["height"]->value(1080));
    auto supersampling =
        static_cast<unsigned int>(resolution["supersampling"]->value(1));
    auto light = root["light"]->valueVector<3>({0});
    auto camera = root["camera"]->valueVector<3>({0});
    auto background = root["background"]->valueVector<3>({0});
    auto bounces = static_cast<unsigned int>(root["bounces"]->value(7));

    std::vector<vec3<float>> pixels(width * height);
    auto samplesX = width * supersampling;
    auto samplesY = height * supersampling;

    std::map<std::string, Material> materials;
    for (const auto& nodePtr : root["materials"]->children())
    {
        auto& node = *nodePtr;
        materials[node.name()] =
            Material{node["ambient"]->valueVector<3>({0}),
                     node["diffuse"]->valueVector<3>({0}),
                     node["specular"]->valueVector<3>({0})};
    }
    std::vector<std::unique_ptr<SceneObject>> objects;
    for (const auto& nodePtr : root["objects"]->children())
    {
        auto& node = *nodePtr;
        if (node.name() == "sphere")
        {
            objects.push_back(
                std::make_unique<Sphere>(node["position"]->valueVector<3>({0}),
                                         node["radius"]->value(0.0f),
                                         materials[node["material"]->value()]));
        }
        else if (node.name() == "plane")
        {
            objects.push_back(
                std::make_unique<Plane>(node["position"]->valueVector<3>({0}),
                                        node["normal"]->valueVector<3>({0}),
                                        materials[node["material"]->value()]));
        }
    }

    Scene s{light, background, bounces};
    const vec3<float> cameraPosition{camera};
    for (auto& object : objects)
    {
        s.add(std::move(object));
    }

#pragma omp parallel for collapse(2)
    for (unsigned int x = 0; x < width; ++x)
    {
        for (unsigned int y = 0; y < height; ++y)
        {
            vec3<float> sample;
            for (unsigned int x_s = 0; x_s < supersampling; ++x_s)
            {
                for (unsigned int y_s = 0; y_s < supersampling; ++y_s)
                {
                    const vec3<float> through{
                        ((x * supersampling + x_s) - samplesX / 2.f) / samplesY,
                        ((y * supersampling + y_s) - samplesY / 2.f) / samplesY,
                        1.f};
                    const auto ray = Line::throughPoints(
                        cameraPosition, cameraPosition + through);

                    sample += s.findColorFor(ray);
                }
            }
            pixels[x + y * width] = sample / (supersampling * supersampling);
        }
    }

    save(pixels, width, height, argv[2]);
}

void save(const std::vector<vec3<float>>& map, const int& width,
          const int& height, const std::string& filename)
{
    std::vector<unsigned char> data;
    for (const auto& vec : map)
    {
        for (auto i = 0u; i < COLORS; i++)
        {
            data.push_back(static_cast<unsigned char>(vec[i]));
        }
        data.push_back(255);
    }
    const QImage image{data.data(), width, height, QImage::Format_ARGB32};
    const auto status = image.save(QString::fromStdString(filename));
    assert(status);
}
