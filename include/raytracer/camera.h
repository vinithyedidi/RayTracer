#ifndef CAMERA_H
#define CAMERA_H

#include "hittable.h"
#include "color.h"
#include "raytracer.h"

#include <thread>


class camera {
public:

    float aspect_ratio  = 1.0;                  // Ratio of image width over height
    int    width        = 100;                  // Rendered image width in pixel count
    double vfov         = 90.0;                 // Vertical view angle (field of view)
    vec3 lookfrom       = vec3(0,0,0);   // Point camera is looking from
    vec3 lookat         = vec3(0,0,-1);  // Point camera is looking at
    vec3   vup          = vec3(0,1,0);     // Camera-relative "up" direction

    float pitch = 0.0f;
    float yaw = -90.0f;
    float mouse_sensitivity = 0.12;
    float movement_speed = 0.05;
    vec3   right    = vec3(1, 0, 0);
    vec3   up       = vec3(0, 1, 0);
    vec3   forward  = vec3(0, 0, -1);              // Camera frame basis vectors


    void render(unsigned char* pixels, const hittable& world) {
        initialize();
        int num_threads = 4;
        std::vector <int> partitions;
        partitions.push_back(0);
        std::vector <std::thread> threads;

        for (int j = 0; j < num_threads; j++) {
            partitions.push_back(int((j+1)/num_threads * height));
            threads.push_back(std::thread([this, pixels, &partitions, &world, j](){
            render_loop(pixels, partitions.at(j), partitions.at(j+1), world);}));
        }
        for (int k = 0; k < num_threads; k++) {
            threads.at(k).join();
        }
        // int counter = 0;
        // for (int j = 0; j < height; j++) {
        //     for (int i = 0; i < width; i++) {
        //         auto pixel_center = pixel00_loc + (float(i) * pixel_delta_u) + (float(j) * pixel_delta_v);
        //         auto ray_direction = pixel_center - lookfrom;
        //         ray r(lookfrom, ray_direction);
        //         color pixel_color = ray_color(r, world);
        //
        //         pixels[3*counter]     = (unsigned char)(pixel_color.x * 255.9);
        //         pixels[3*counter+1]   = (unsigned char)(pixel_color.y * 255.9);
        //         pixels[3*counter+2]   = (unsigned char)(pixel_color.z * 255.9);
        //
        //         ++counter;
        //     }
        // }
    }


    void render_loop (unsigned char* pixels, int start, int stop, const hittable& world) {
        int pixel_num = start*width;
        for (int j = start; j < stop; j++) {
            for (int i = 0; i < width; i++) {
                vec3 pixel_center = pixel00_loc + (float(i) * pixel_delta_u) + (float(j) * pixel_delta_v);
                vec3 ray_direction = pixel_center - lookfrom;
                ray r(lookfrom, ray_direction);
                color pixel_color = ray_color(r, world);

                pixels[3*pixel_num]     = (unsigned char)(pixel_color.x * 255.9);
                pixels[3*pixel_num+1]   = (unsigned char)(pixel_color.y * 255.9);
                pixels[3*pixel_num+2]   = (unsigned char)(pixel_color.z * 255.9);

                ++pixel_num;
            }
        }
    }

private:

    int    height;          // Rendered image height
    vec3   pixel00_loc;    // Location of pixel 0, 0
    vec3   pixel_delta_u;  // Offset to pixel to the right
    vec3   pixel_delta_v;  // Offset to pixel below

    void initialize() {
        height = int(width / aspect_ratio);

        // Determine viewport dimensions.
        float focal_length = length(lookfrom - lookat);
        float theta = radians(vfov);
        float h = std::tan(theta/2);
        float viewport_height = 2 * h * focal_length;
        float viewport_width = viewport_height * (float(width)/height);

        // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
        forward.x = cos(radians(yaw)) * cos(radians(pitch));
        forward.y = sin(radians(pitch));
        forward.z = sin(radians(yaw)) * cos(radians(pitch));
        forward = normalize(forward);
        right = normalize(cross(vup, forward));
        up = cross(forward, right);

        lookat = lookfrom + forward;

        // Calculate the vectors across the horizontal and down the vertical viewport edges.
        vec3 viewport_u = viewport_width * right;    // Vector across viewport horizontal edge
        vec3 viewport_v = viewport_height * up;  // Vector down viewport vertical edge

        // Calculate the horizontal and vertical delta vectors from pixel to pixel.
        pixel_delta_u = viewport_u / float(width);
        pixel_delta_v = viewport_v / float(height);

        // Calculate the location of the upper left pixel.
        auto viewport_upper_left = lookfrom - (focal_length * forward) - viewport_u/2.0f - viewport_v/2.0f;
        pixel00_loc = viewport_upper_left + 0.5f * (pixel_delta_u + pixel_delta_v);
    }


    color ray_color(const ray& r, const hittable& world) {
        color col;;
        hit_record rec;
        if (world.hit(r, 0, INFINITY, rec)) {
            return 0.5f * (rec.normal + color(1,1,1));
        } else {
            vec3 unit_direction = normalize(r.direction());
            float a = 0.5f*(unit_direction.y + 1.0f);
            col = a*color(0.0, 0.15, 0.3) + (1.0f-a)*color(0.0, 0.05, 0.1);
        }
        return col;
    }
};

#endif //CAMERA_H
