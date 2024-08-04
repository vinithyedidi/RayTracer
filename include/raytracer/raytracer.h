

#ifndef RAYTRACER_H
#define RAYTRACER_H
#define GLM_FORCE_SWIZZLE

#include <glm/glm.hpp>
#include <cmath>
#include <limits>
#include <memory>

#include "camera.h"
#include "color.h"
#include "hittable.h"
#include "hittable_list.h"
#include "ray.h"
#include "sphere.h"

// C++ Std Usings

using std::make_shared;
using std::shared_ptr;
using namespace glm;

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

float degrees_to_radians(float degrees) {
    return degrees * pi / 180.0f;
}

#endif //RAYTRACER_H
