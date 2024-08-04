#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glad/glad.c>
#include <iostream>
#include <chrono>

#include "shader.h"
#include "raytracer/raytracer.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, camera *cam, double* mouse_pos, long long dt);

// settings
const unsigned int SCR_WIDTH = 200;
const float ASPECT_RATIO = 16.0 / 9.0;
const unsigned int SCR_HEIGHT = SCR_WIDTH / ASPECT_RATIO;


void run_window() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif


    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Raytracer v1.0   -Vinith Yedidi, 2024",
        NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    // Use Shader class to compile shaders
    Shader ourShader("/Users/vinithyedidi/CodingProjects/CLion/Renderer/src/vt.glsl",
        "/Users/vinithyedidi/CodingProjects/CLion/Renderer/src/ft.glsl");

    // Generate rectangle across entire screen — this is the canvas for displaying the pixels
    float vertices[] = {
        // positions          // colors           // texture coords
         1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // top right
         1.0f, -1.0f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // bottom right
        -1.0f, -1.0f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -1.0f,  1.0f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // top left
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Initialize pixel array to be rendered onto
    unsigned char pixels[SCR_WIDTH*SCR_HEIGHT*3];
    unsigned int texture;
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // set up camera
    camera cam;
    cam.aspect_ratio = 16.0 / 9.0;
    cam.width  = SCR_WIDTH;
    cam.vfov     = 90;
    cam.lookfrom = vec3(0,0,0);
    cam.lookat   = vec3(0,0,-1);
    cam.vup      = vec3(0,1,0);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // set up world with 2 spheres
    hittable_list world;
    world.add(make_shared<sphere>(vec3(-0.5,0,-1), 0.3));
    world.add(make_shared<sphere>(vec3(0.5,0,-1), 0.3));
    //world.add(make_shared<sphere>(vec3( 0.0, -100.5, -1.0),100.0));

    // set up cursor for mouse input
    double mouse_pos[] = {0, 0};
    std::chrono::time_point<std::chrono::steady_clock> t0, t1;
    long long dt = -1;

    // Game loop
    while (!glfwWindowShouldClose(window)) {

        t0 = std::chrono::high_resolution_clock::now();
        processInput(window, &cam, mouse_pos, dt);
        glClear(GL_COLOR_BUFFER_BIT);

        // Use rendering engine to generate array of pixels to display on screen
        cam.render(pixels, world);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCR_WIDTH, SCR_HEIGHT,
            0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
        glGenerateMipmap(GL_TEXTURE_2D);
        ourShader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();

        t1 = std::chrono::high_resolution_clock::now();
        dt = std::chrono::duration_cast<std::chrono::microseconds>(t1 - t0).count();
        std::cout << '\r' << 1000000.0f/dt << " fps" << std::flush;
    }

    // De-allocate all resources once they've outlived their purpose:
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
}


int main() {
    run_window();
    return 0;
}


// Process all user input
void processInput(GLFWwindow *window, camera *cam, double* mouse_pos, long long dt) {

    // Reset keys: ESC to quit, R to go back to origin,
    // F to make window big, C to make window small.
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        cam->lookfrom = vec3(0, 0, 0);
        cam->lookat   = vec3(0, 0, -1);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        glfwSetWindowSize(window, 800, 800/ASPECT_RATIO);
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        glfwSetWindowSize(window, 400, 400/ASPECT_RATIO);
    }


    // Movement keys: WASD
    vec3 move_vector = vec3(0, 0, 0);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        move_vector -= cam->forward;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        move_vector += cam->forward;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        move_vector -= cam->right;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        move_vector += cam->right;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        move_vector += cam->up;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        move_vector -= cam->up;
    }
    move_vector   *= cam->movement_speed;
    cam->lookfrom += move_vector;
    cam->lookat   += move_vector;

    // Look-at keys: mouse input
    double new_x; double new_y;
    glfwGetCursorPos(window, &new_x, &new_y);
    float x_offset = new_x - mouse_pos[0];
    float y_offset = new_y - mouse_pos[1];

    cam->yaw += x_offset*cam->mouse_sensitivity;
    cam->pitch += y_offset*cam->mouse_sensitivity;
    if (cam->pitch > 89.0f)
        cam->pitch = 89.0f;
    if (cam->pitch < -89.0f)
        cam->pitch = -89.0f;

    mouse_pos[0] = new_x;
    mouse_pos[1] = new_y;
}

// React to change in window size.
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    // make sure the viewport matches the new window dimensions
    glViewport(0, 0, width, height);
}
