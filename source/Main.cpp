#include <iostream>
#include <fstream>
#include <cstdlib>
#include <memory>

#include "Scene.h"
#include "Sphere.h"
#include "Plane.h"
#include "SceneObject.h"
#include "Material.h"
#include "Vector.h"

#define IMAGE_X (1920)
#define IMAGE_Y (1080)
#define SAMPLES 2
#define SAMPLES_X (IMAGE_X * SAMPLES)
#define SAMPLES_Y (IMAGE_Y * SAMPLES)

#define COLORS 3

char *to_bytes(int number, int num_bytes = 4);

void save(const std::vector<vec3<float>> &map, const std::string &filename);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " outputfile" << std::endl;
        return 1;
    }

    Material mirror = {{2.0f, 7.0f, 1.0f}, {0.0f}, {240.f}};
    Material diffuseRed = {{7, 1, 20}, {72, 12, 203}, {0.0f}};
    Material diffuseYellow = {{7, 20, 20}, {72, 200, 200}, {0.0f}};
    Material diffuseGreen = {{7, 20, 1}, {72, 200, 12}, {0.0f}};
    Material diffuseWhite = {{25, 25, 25}, {255, 255, 255}, {0.0f}};
    Material diffusePurple = {{7, 1, 5}, {72, 12, 50}, {0.0f}};
    Material reflectingRed = {{7, 1, 20}, {14, 2, 40}, {72, 12, 203}};
    Material reflectingYellow = {{7, 20, 20}, {14, 40, 40}, {72, 200, 200}};
    Material reflectingGreen = {{7, 20, 1}, {14, 40, 2}, {72, 200, 12}};
    Material reflectingWhite = {{25, 25, 25}, {50, 50, 50}, {255, 255, 255}};
    Material reflectingPurple = {{7, 1, 5}, {14, 2, 10}, {72, 12, 50}};

    Scene s{vec3<float>(15, -0.6, 5)};
    s.add(std::make_unique<Sphere>(vec3<float>(4, 2, 25), 5.f, reflectingRed));
    s.add(
        std::make_unique<Sphere>(vec3<float>(10, 0, 15), 1.f, reflectingGreen));
    s.add(std::make_unique<Sphere>(vec3<float>(0, -4, 15), 0.3f,
                                   reflectingPurple));
    s.add(std::make_unique<Sphere>(vec3<float>(-3, -6, 23), 5.f,
                                   reflectingYellow));
    s.add(std::make_unique<Plane>(vec3<float>(0, 0, 30), vec3<float>(0, 0, -1),
                                  diffuseWhite));
    s.add(std::make_unique<Plane>(vec3<float>(0, 0, -5), vec3<float>(0, 0, 1),
                                  diffuseWhite));
    s.add(std::make_unique<Plane>(vec3<float>(-15, 0, 0), vec3<float>(1, 0, 0),
                                  diffuseWhite));
    s.add(std::make_unique<Plane>(vec3<float>(20, 0, 0), vec3<float>(-1, 0, 0),
                                  diffuseWhite));

    std::vector<vec3<float>> pixels(IMAGE_X * IMAGE_Y);

    const vec3<float> camera{0.0f};
#pragma omp parallel for collapse(2)
    for (unsigned int x = 0; x < IMAGE_X; ++x)
    {
        for (unsigned int y = 0; y < IMAGE_Y; ++y)
        {
            vec3<float> sample;
            for (unsigned int x_s = 0; x_s < SAMPLES; ++x_s)
            {
                for (unsigned int y_s = 0; y_s < SAMPLES; ++y_s)
                {
                    const vec3<float> through{
                        ((x * SAMPLES + x_s) - SAMPLES_X / 2.f) / SAMPLES_Y,
                        ((y * SAMPLES + y_s) - SAMPLES_Y / 2.f) / SAMPLES_Y,
                        1.f};
                    const auto ray = Line::throughPoints(camera, through);

                    sample += s.findColorFor(ray);
                }
            }
            pixels[x + y * IMAGE_X] = sample / (SAMPLES * SAMPLES);
        }
    }

    save(pixels, argv[1]);
}

void write(std::ofstream &file, int number, int num_bytes)
{
    std::vector<char> bytes;
    for (int i = 0; i < num_bytes; i++)
    {
        bytes.push_back(number % 256);
        number /= 256;
    }
    file.write(bytes.data(), num_bytes);
}

void save(const std::vector<vec3<float>> &map, const std::string &filename)
{
    // writes data in map to a bmp file
    std::cout << "saving" << std::endl;
    const int offset = 54;
    const int image_size = IMAGE_X * IMAGE_Y * COLORS;
    const int file_size = offset + image_size;
    std::ofstream file{filename, std::ios::out | std::ios::binary};

    file.write("BM", 2);
    write(file, file_size, 4);
    write(file, 0, 4);
    write(file, offset, 4);
    write(file, 40, 4);
    write(file, IMAGE_X, 4);
    write(file, IMAGE_Y, 4);
    write(file, 1, 2);
    write(file, COLORS * 8, 2);
    write(file, 0, 4);
    write(file, image_size, 4);
    write(file, 0, 4);
    write(file, 0, 4);
    write(file, 0, 4);
    write(file, 0, 4);

    unsigned char c[3];
    for (auto y = 0u; y < IMAGE_Y; y++)
    {
        for (auto x = 0u; x < IMAGE_X; x++)
        {
            for (auto i = 0u; i < COLORS; i++)
            {
                c[i] = map[x + y * IMAGE_X][i];
            }
            file.write(reinterpret_cast<char *>(c), COLORS);
        }
    }
}
