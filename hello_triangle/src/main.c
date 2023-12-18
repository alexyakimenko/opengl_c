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
    GLFWwindow* window = glfwCreateWindow(960, 540, "Hello Triangle", NULL, NULL);
    if(window == NULL) {
        printf("Failed to create GLFW window!\n");
        glfwTerminate();
        return -1;
    }

    // Makes context of our window the main context on the current thread
    glfwMakeContextCurrent(window);

    // Handles user input      
    glfwSetKeyCallback(window, key_callback);
    // Changes viewport on window change size 
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Glad init
    if(
        !gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)
    ) {
        printf("Failed to initialize GLAD!\n");
        return -1;
    }
    
    // Rendering Triangle

    // Vertex Shader
    // Gets shader string from file 
    const char* vertexShaderSource = getShader(VERTEX);
    // Creates vertex shader and stores its id
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // Attaches shader code to the shader object
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    // Compiles shader ...
    glCompileShader(vertexShader);
    { // Checks compilation success
        int success;
        char info[512];
        // Gets compilation status
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if(!success) {
            // Gets shader logs
            glGetShaderInfoLog(vertexShader, sizeof(info), NULL, info);
            printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", info);
        }
    }

    // Fragment Shader
    // Gets shader string from file 
    const char* fragmentShaderSource = getShader(FRAGMENT);
    // Creates fragment shader and stores its id
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // Attaches shader code to the shader object
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    // Compiles shader ...
    glCompileShader(fragmentShader);
    { // Checks compilation success
        int success;
        char info[512];
        // Gets compilation status
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if(!success) {
            // Gets shader logs
            glGetShaderInfoLog(fragmentShader, sizeof(info), NULL, info);
            printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", info);
        }
    }

    // Triangle Vertices ∆
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f 
    };

    // Unique id of Vertex Buffer Object
    unsigned int VBO;
    // Generate Buffer 
    glGenBuffers(1, &VBO);
    // Binds our buffer to opengl array buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Copies data to buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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