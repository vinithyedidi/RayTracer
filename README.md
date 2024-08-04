# Raytracing Game Engine

(See "Images" to check out what I managed to render)

This project was quite the learning experience — I went from no C++ experience to making a full raytracing engine in OpenGL.

To start, I followed Peter Shirley's "Ray Tracing in One Weekend" tutorial and made a raytracer for images. Then I made another one with my own adjustments, including using GLM for matrix calculations, implementing multithreading, and otherwise attempting to improve performance for instant rendering.

Then I downloaded OpenGL and GLFW libraries and made a window. To render the texture, I rasterized a quad shape using two triangles and then made it upload an array of pixels (freshly calculated by the raytracer) and display it. After that, I implemented keyboard and mouse input for flying around using the camera, making it a full first person game engine.

This project was a lot of fun, because I got to use a lot of math to implement the raytracer (who knew it's just the quadratic formula) and to move the player around in 3D space and render it to a 2D pixel grid. If I were to work on this in the future, I would try and move calculations for all of this to the GPU as much as possible — I attempted this but as it turns out, Apple doesn't support OpenGL compute shaders so I'd have to learn how to use Metal (grrr). 

Overall, I learned a lot about C++, graphics, linear algebra, and generally how computations work on a computer. I hope to do more in graphics and C++.


*note: you will need to install GLM, OpenGL, GLFW, GLAD, and KHR to run this program.

# Sources
https://raytracing.github.io/
https://learnopengl.com

