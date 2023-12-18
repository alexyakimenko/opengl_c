#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

// Include callback functions 
#include "callbacks/main.c"
#include "util/main.c"

int main() {
    glfwInit();

    // Optional 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    // Window Creation
    GLFWwindow* window = glfwCreateWindow(960, 540, "Hello Window", NULL, NULL);
    if(window == NULL) {
        printf("Failed to create GLFW window!\n");
        glfwTerminate();
        return -1;
    }

    // Makes context of our window the main context on the current thread
    glfwMakeContextCurrent(window);

    // Handles user input      
    glfwSetKeyCallback(window, key_callback);


    // Glad init
    if(
        !gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)
    ) {
        printf("Failed to initialize GLAD!\n");
        return -1;
    }

    // Changes viewport on window change size 
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    // Render Loop
    while(!glfwWindowShouldClose(window)) {
        // Rendering
        glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Swaps Front Buffer with Back Buffer
        glfwSwapBuffers(window);
        // Checks if any of events are triggered
        glfwPollEvents();
    }

    // free() in glfw world
    glfwTerminate();
    return 0;
}