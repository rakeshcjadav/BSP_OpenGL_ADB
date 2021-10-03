#include<iostream>
#include"GL/glew.h"
#include"GLFW/glfw3.h"

void OnWindowResize(GLFWwindow * pWindow, int width, int height)
{
    glViewport(0, 0, width, height);
    glScissor(0, 0, width, height);
}

unsigned int CreateTriangle(unsigned int & vertexCount)
{
    float vertices[] = {
        //  (x, y, z, w) (r, g, b)
        -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,      // top right
        -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,    // bottom left
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,   // bottom right

         0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,      // top left
        -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,      // top right
         0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,    // bottom right
    };

    vertexCount = 6;

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    {
        unsigned int VBO;
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // at location 0, read 4 floats and jump (stride) 4 floats for next vertex.
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, (4+3) * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (4+3) * sizeof(float), (void*)(4*sizeof(float)));
        glEnableVertexAttribArray(1);
    }
    glBindVertexArray(0);

    return VAO;
}

unsigned int CreateShaders()
{
    const char * VertexShaderSource =
        "#version 330 core\n"
        "layout(location = 0) in vec4 aPos;\n"
        "layout(location = 1) in vec3 Color;\n"
        "out vec3 colorFrag;\n"

        "void main()\n"
        "{\n"
            "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
            "colorFrag = Color;\n"
        "}\n";

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &VertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int  success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    const char* FragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "in vec3 colorFrag;\n"
        "void main()\n"
        "{\n"
            "FragColor = vec4(colorFrag.r, colorFrag.g, colorFrag.b, 1.0f);\n"
        "}\n";

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &FragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

int main()
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    int width = 640;
    int height = 480;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }


    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // Initialize glew
    glewInit();


    glfwSetFramebufferSizeCallback(window, OnWindowResize);

    // TODO: Move to material
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);    // Transparent blend or Alpha blend or Transparent
    //glBlendFunc(GL_ONE, GL_ZERO); // Opaque or Replace blend
    //glBlendFunc(GL_ONE_MINUS_SRC_COLOR, GL_ZERO);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glFrontFace(GL_CCW);

    glEnable(GL_SCISSOR_TEST);

    glViewport(0, 0, width, height);
    glScissor(0, 0, width, height);


    unsigned int vertexCount = 0;
    // Entities
    unsigned int triangle = CreateTriangle(vertexCount);
    unsigned int program = CreateShaders();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        glUseProgram(program);
        glBindVertexArray(triangle);

        glDrawArrays(GL_TRIANGLES, 0, vertexCount);

        glBindVertexArray(0);

        /*
        glBegin(GL_TRIANGLES);
            glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
            glVertex3f(0.5f, 0.5f, 0.0f);       // top right
            glVertex3f(-0.5f, 0.5f, 0.0f);      // top left
            glVertex3f(-0.5f, -0.5f, 0.0f);     // bottom left

            glColor4f(0.0f, 0.5f, 0.0f, 0.5f);
            glVertex3f(-0.5f, 0.0f, 0.0f);     // bottom left
            glVertex3f(0.5f, -0.5f, 0.0f);      // bottom right
            glVertex3f(0.5f, 0.5f, 0.0f);       // top right

            glColor4f(0.0f, 0.0f, 1.0f, 0.1f);
            glVertex3f(0.0f, 0.5f, 0.0f);        // bottom left
            glVertex3f(-0.6f, -0.6f, 0.0f);      // bottom right
            glVertex3f(0.6f, -0.6f, 0.0f);       // top right
        glEnd();
        */

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}