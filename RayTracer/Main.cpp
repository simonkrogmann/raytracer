
#include "Vector.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <cstdlib>

#include "Scene.h"
#include "Sphere.h"
#include "Plane.h"
#include "SceneObject.h"
#include "Material.h"

#define IMAGE_X (1920 * 1)
#define IMAGE_Y (1080 * 1)
#define SAMPLES 2
#define SAMPLES_X (IMAGE_X * SAMPLES)
#define SAMPLES_Y (IMAGE_Y * SAMPLES)


#define COLORS 3
#define COLOR_BYTES 1


char* to_bytes(int number, int num_bytes = 4);

void save(std::vector < vec3<float> > map);

vec3<int> clamp(vec3<int> color);

int main() {

	Material mirror{ vec3 < float > { 2.0f, 7.0f, 1.0f }, vec3 < float > { 0.0f }, vec3 < float > { 240.f } };
	Material diffuseRed{ vec3<float>{ 7, 1, 20 }, vec3<float>{ 72, 12, 203 }, vec3 < float > { 0.0f } };
	Material diffuseYellow{ vec3<float>{ 7, 20, 20 }, vec3<float>{ 72, 200, 200 }, vec3 < float > { 0.0f } };
	Material diffuseGreen{ vec3<float>{ 7, 20, 1 }, vec3<float>{ 72, 200, 12 }, vec3 < float > { 0.0f } };
	Material diffuseWhite{ vec3<float>{ 25, 25, 25 }, vec3<float>{ 255, 255, 255 }, vec3 < float > { 0.0f } };
	Material diffusePurple{ vec3<float>{ 7, 1, 5 }, vec3<float>{ 72, 12, 50 }, vec3 < float > { 0.0f } };
	Material reflectingRed{ vec3<float>{ 7, 1, 20 }, vec3<float>{ 14, 2, 40 }, vec3 < float > { 72, 12, 203 } };
	Material reflectingYellow{ vec3<float>{ 7, 20, 20 }, vec3<float>{ 14, 40, 40 }, vec3 < float > { 72, 200, 200 } };
	Material reflectingGreen{ vec3<float>{ 7, 20, 1 }, vec3<float>{ 14, 40, 2 }, vec3 < float > { 72, 200, 12 } };
	Material reflectingWhite{ vec3<float>{ 25, 25, 25 }, vec3<float>{ 50, 50, 50 }, vec3 < float > { 255, 255, 255 } };
	Material reflectingPurple{ vec3<float>{ 7, 1, 5 }, vec3<float>{ 14, 2, 10 }, vec3 < float > { 72, 12, 50 } };

	Scene s{ vec3<float>(15, -0.6, 5)};
	s.add(new Sphere(vec3<float>(4, 2, 25), 5.f, reflectingRed));
	s.add(new Sphere(vec3<float>(10, 0, 15), 1.f, reflectingGreen));
	s.add(new Sphere(vec3<float>(0, -4, 15), 0.3f, reflectingPurple));
	s.add(new Sphere(vec3<float>(-3, -6, 23), 5.f, reflectingYellow));
	//s.add(new Sphere(vec3<float>(0, 0, 0), 30.f, vec3<float>{ 255, 255, 255 }));
	s.add(new Plane(vec3<float>(0, 0, 30), vec3<float>(0, 0, -1), diffuseWhite));
	s.add(new Plane(vec3<float>(0, 0, -5), vec3<float>(0, 0, 1), diffuseWhite));
	s.add(new Plane(vec3<float>(-15, 0, 0), vec3<float>(1, 0, 0), diffuseWhite));
	s.add(new Plane(vec3<float>(20, 0, 0), vec3<float>(-1, 0, 0), diffuseWhite));

	std::vector < vec3<float> > pixels(IMAGE_X * IMAGE_Y);

	vec3<float> camera{ 0.0f };
	for (int x = 0; x < IMAGE_X; x++) {
		for (int y = 0; y < IMAGE_Y; y++) {
			vec3<float> sample;
			for (int x_s = 0; x_s < SAMPLES; x_s++) {
				for (int y_s = 0; y_s < SAMPLES; y_s++) {
					vec3<float> through{ ((x * SAMPLES + x_s) - SAMPLES_X / 2.f) / SAMPLES_Y, ((y * SAMPLES + y_s) - SAMPLES_Y / 2.f) / SAMPLES_Y, 1.f };
					Line ray = Line::throughPoints(camera, through);

					sample = sample + s.findColorFor(ray);
				}
			}
			pixels[x + y * IMAGE_X] = sample / (SAMPLES * SAMPLES);
		}
	}
	
	save(pixels);
}

char* to_bytes(int number, int num_bytes) {
	char* ret = new char[num_bytes];
	for (int i = 0; i < num_bytes; i++) {
		ret[i] = number % 256;
		number /= 256;
	}
	return ret;
}

void save(std::vector < vec3<float> > map) {
	// writes data in map to a bmp file
	std::cout << "saving\n";
	int offset = 54;
	int image_size = IMAGE_X * IMAGE_Y * COLORS;
	int file_size = offset + image_size;
	std::ofstream myFile;
	std::tr2::sys::path p("..\\output");
	int count = 0;
	std::tr2::sys::directory_iterator dir{ p };
	for (; dir != std::tr2::sys::directory_iterator(); dir++) {
		std::cout << dir->path().directory_string() << std::endl;
		count++;
	}

	char location[100] = "";
	sprintf_s(location, (size_t)100, "..\\output\\img%d.bmp", count);
	myFile.open(location, std::ios::out | std::ios::binary);
	if (!myFile.is_open()) {
		std::cout << "filestram failed" << std::endl;
		std::cin.get();
	}


	myFile.write("BM", 2);
	myFile.write(to_bytes(file_size), 4);
	myFile.write(to_bytes(0), 4);
	myFile.write(to_bytes(offset), 4);

	myFile.write(to_bytes(40), 4);
	myFile.write(to_bytes(IMAGE_X), 4);
	myFile.write(to_bytes(IMAGE_Y), 4);
	myFile.write(to_bytes(1), 2);
	myFile.write(to_bytes(COLORS * 8), 2);
	myFile.write(to_bytes(0), 4);
	myFile.write(to_bytes(image_size), 4);
	myFile.write(to_bytes(0), 4);
	myFile.write(to_bytes(0), 4);
	myFile.write(to_bytes(0), 4);
	myFile.write(to_bytes(0), 4);

	char c[3];
	for (int y = 0; y < IMAGE_Y; y++) {
		for (int x = 0; x < IMAGE_X; x++) {
			for (int i = 0; i < COLORS; i++) {
				if (COLOR_BYTES > 1) {
					int bla = round(map[x + y * IMAGE_X][i] / pow(256, COLOR_BYTES - 1));
					c[i] = (0 && bla > 255) ? 255 : bla;
				} else {
					c[i] = map[x + y * IMAGE_X][i];
				}
			}
			myFile.write(c, COLORS);
		}
	}
	myFile.close();
}


vec3<int> clamp(vec3<int> color) {
	color.x = std::min(std::max(0, color.x), 255);
	color.y = std::min(std::max(0, color.y), 255);
	color.z = std::min(std::max(0, color.y), 255);
	return color;
}