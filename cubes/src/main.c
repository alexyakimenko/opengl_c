#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stb_image.h>

// Include callback functions 
#include "callbacks/main.c"
// Include utility functions
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
    // Free memory
    free((void*) vertexShaderSource);
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
    // Free memory
    free((void*) fragmentShaderSource);
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

    // Shader program
    // Creates shader program
    unsigned int shaderProgram = glCreateProgram();
    // Attaches vertex shader to the program
    glAttachShader(shaderProgram, vertexShader);
    // Attaches fragment shader to the program
    glAttachShader(shaderProgram, fragmentShader);
    // Links our shader program
    glLinkProgram(shaderProgram);
    { // Checks linking success
        int success;
        char info[512];
        // Gets linking status
        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if(!success) {
            // Gets program logs
            glGetProgramInfoLog(shaderProgram, sizeof(info), NULL, info);
            printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", info);
        }
    }
    // Deletes shaders after they were attached in shader program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Rectangle Vertices []  
    float vertices[] = {
         0.5f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f,
    };
    // Order to draw
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    // Unique id of Vertex Buffer Object
    unsigned int VBO;
    // Generate Buffer 
    glGenBuffers(1, &VBO);

    // Unique id of Vertex Array Object 
    unsigned int VAO;
    // Generato VAO
    glGenVertexArrays(1, &VAO);

    // Unique id of Element Buffer Object
    unsigned int EBO;
    // Generate EBO
    glGenBuffers(1, &EBO);

    // Binds VAO before other binds
    glBindVertexArray(VAO);

    // Binds our buffer to opengl array buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Copies data to buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Binds our element buffer to opengl element array  buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // Copies data to buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Tells opengl how to interpret vertex data
    // first param: location of this attribute
    // second param: size of vertex attribute (vec3 -> 3)
    // third param: specifies the type of vertex data
    // fourth param: should values be normalized or not 
    // fifth param: stride 
    // sixth param: offset where our data begins in the buffer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    // Texture
    // Unique id of texture
    unsigned int texture;
    // Generates texture 
    glGenTextures(1, &texture);
    // Binds our texture to opengl texture
    glBindTexture(GL_TEXTURE_2D, texture);

    // Sets texture wrapping/filtering options
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Loads image 
    int width, height, nrChannels;
    unsigned char* data = stbi_load("assets/textures/container.jpg", &width, &height, &nrChannels, 0);

    // Generates texture
    // first param: texture target
    // second param: mipmap level
    // third param: format that we want to store the texture
    // fourth & fifth params: width & height ...
    // sixth param: border (for some reasons should always be 0) 
    // seventh & eighth params: format & datatype of source image
    // ninth param: image data (array of pixels)
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    // Generates mipmap for currently bound texture
    glGenerateMipmap(GL_TEXTURE_2D);
    // Free image memory
    stbi_image_free(data);

    // Render Loop
    while(!glfwWindowShouldClose(window)) {
        // Rendering
        glClearColor(0.1f, 0.1f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draws triangle
        // Uses following shader program
        glUseProgram(shaderProgram);
        // Binds VAO 
        glBindVertexArray(VAO);
        // Wireframe mode
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        // Draws elements from EBO
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        
        // Swaps Front Buffer with Back Buffer
        glfwSwapBuffers(window);
        // Checks if any of events are triggered
        glfwPollEvents();
    }

    // free() in glfw world
    glfwTerminate();
    return 0;
}