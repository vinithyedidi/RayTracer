#include "rtweekend.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include "bvh.h"



int main() {

    // World
    hittable_list world;

    auto ground_material = make_shared<lambertian>(color(0.8, 0.8, 0));
    world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

    auto sphere_material = make_shared<lambertian>(color(0.1, 0.2, 0.2));
    auto center1 = point3(0, 0, -1);
    auto center2 = center1 + vec3(0, 0.5, 0);
    world.add(make_shared<sphere>(center1, center2, 0.2, sphere_material));

    world = hittable_list(make_shared<bvh_node>(world));

    // Camera
    camera cam;

    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 800;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0.6;
    cam.focus_dist    = 10.0;

    cam.render(world);
}