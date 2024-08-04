#ifndef RAY_H
#define RAY_H

#include "raytracer.h"

using namespace glm;

class ray {
public:
    ray() {}

    ray(const vec3& origin, const vec3& direction) : orig(origin), dir(direction) {}

    const vec3& origin() const  { return orig; }
    const vec3& direction() const { return dir; }

    vec3 at(float t) const {
        return orig + t*dir;
    }

private:
    vec3 orig;
    vec3 dir;
};

#endif