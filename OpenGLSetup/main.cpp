#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

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
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to load openGL function pointers." << std::endl;
        glfwTerminate();
        return -1;
    }

    /* The source code for the vertex shader. */
    const char* vertexShaderSrc = 
        "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main() {\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
        "}\0";

    const char* fragmentShaderSrc = 
        "#version 330 core\n"
        "out vec4 fragColor;\n"
        "void main() {\n"
        "   fragColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);\n" // RGBA values
        "}\0";

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
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, 0, infoLog);
        std::cout << "Failed to link the shader program. ERR: " << infoLog << std::endl;
    }

    // We can delete these because they are now inside of shaderProgram, so to free resources we delete.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Define our vertices of the tringle
    float vertices[] = {
        -0.5f, -0.5f, 0.0f, // 1st
         0.0f,  0.5f, 0.0f, // 2nd
         0.5f, -0.5f, 0.0f, // 3rd
    };

    unsigned int VAO, VBO; // vertex array object, vertex buffer object
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO); // 1 buffer object, and the reference

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // data is set once, but used many times
    // other examples: gl_stream_draw, gl_dynamic_draw (data is changed a lot, and used a lot)
    // Specifies how to interpret vertices in our array
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);
    glEnableVertexAttribArray(0); // is false by default, enable it (0 is the index?)

    // These unbind instead of bind. 0 means to unbind.
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    /* Simple pipeline to convert integer RBG values into floats for openGL rendering.*/
    int intRGB[3] = {
        235, 195, 52
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
        processInput(window);

        /* Render here */
        glClearColor(fRGB[0], fRGB[1], fRGB[2], 1.0f); // Set color
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

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
    /* Set the new viewport for openGL rendering */
    glViewport(0, 0, width, height);
}