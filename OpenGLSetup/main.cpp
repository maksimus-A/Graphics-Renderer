#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <chrono>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

std::string ReadTextFile(const std::string& fileName);

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create the window." << std::endl;
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    int window_size[2];

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    /* Get the width and height to pass into glfl*/
    glfwGetFramebufferSize(window, &window_size[0], &window_size[1]);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to load openGL function pointers." << std::endl;
        glfwTerminate();
        return -1;
    }


    /* The source code for the vertex shader. */
    std::string vertexShaderCode = ReadTextFile("shaders/vertex.glsl");

    std::string fragmentShaderCode = ReadTextFile("shaders/fragment.glsl");

    const char* vertexShaderSrc = vertexShaderCode.c_str();

    const char* fragmentShaderSrc = fragmentShaderCode.c_str();



    // The actual vertex shader that is being created.
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSrc, 0);
    glCompileShader(vertexShader);
    int success;
    char infoLog[512]; // store info for errors
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, 0, infoLog);
        std::cout << "Failed to compile the vertex shader. ERR: " << infoLog << std::endl;
    }

    // The actual fragment shader that is being created.
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSrc, 0);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, 0, infoLog);
        std::cout << "Failed to compile the fragment shader. ERR: " << infoLog << std::endl;
    }

    // Create shader program and link.
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glUseProgram(shaderProgram); // NEW LINE ADDED?!?

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, 0, infoLog);
        std::cout << "Failed to link the shader program. ERR: " << infoLog << std::endl;
    }

    // Set the resolution uniform
    int resolutionLoc = glGetUniformLocation(shaderProgram, "iResolution");
    auto startTime = std::chrono::system_clock::now();
    int timeLoc = glGetUniformLocation(shaderProgram, "iTime");
    glUniform2f(resolutionLoc, window_size[0], window_size[1]);

    // We can delete these because they are now inside of shaderProgram, so to free resources we delete.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Define our vertices of the rectangle
    /*
    float vertices[] = {
    -0.5f, -0.5f, 0.0f, // bottom left
    -0.5f,  0.5f, 0.0f, // top left
     0.5f, -0.5f, 0.0f, // bottom right
     0.5f,  0.5f, 0.0f  // top right
    };*/
    float vertices[] = {
        -1.0f, -1.0f, 0.0f, // bottom left
        -1.0f,  1.0f, 0.0f, // top left
         1.0f, -1.0f, 0.0f, // bottom right
         1.0f,  1.0f, 0.0f  // top right
    };

    unsigned int indices[] = {
        1, 0, 2, // first triangle
        1, 2, 3  // second triangle
    };

    unsigned int VAO, VBO, EBO; // vertex array object, vertex buffer object, element buffer object
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO); // 1 buffer object, and the reference
    glGenBuffers(1, &EBO); // 1 buffer + element object

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // data is set once, but used many times
    // We tell openGL which indices to draw in which order for each triangle.
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); // for drawing a rectangle
    // other examples: gl_stream_draw, gl_dynamic_draw (data is changed a lot, and used a lot)

    // Specifies how to interpret vertices in our array
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0); // is false by default, enable it (0 is the index?)

    // These unbind instead of bind. 0 means to unbind.
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    /* Simple pipeline to convert integer RBG values into floats for openGL rendering.*/
    int intRGB[3] = {
        25, 25, 25
    };
    int RGBmax = 255;
    float fRGB[3];
    for (int i = 0; i < 3; i++) {
        fRGB[i] = (float) intRGB[i] / RGBmax;
        // std::cout << "float i: " << fRGB[i] << std::endl;
    }
    

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        // Calculate elapsed time in seconds
        auto currentTime = std::chrono::system_clock::now();
        std::chrono::duration<float> elapsed = currentTime - startTime;
        float timeValue = elapsed.count();

        processInput(window);

        /* Render here */
        glClearColor(fRGB[0], fRGB[1], fRGB[2], 1.0f); // Set color
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        // Pass the time variable to the fragment shader
        glUniform1f(timeLoc, timeValue);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    /* If escape is pressed, close the window */
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    /*Going to modify this to return the width/height for rendering in glsl*/

    /* Set the new viewport for openGL rendering */
    glViewport(0, 0, width, height);
}

std::string ReadTextFile(const std::string& fileName) 
{
    std::ifstream file(fileName);
    if (!file.is_open())
    {
        std::cout << "File failed to open." << std::endl;
        return "";
    }

    std::stringstream ss;
    ss << file.rdbuf();
    file.close();

    return ss.str();
}