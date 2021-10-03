#include<iostream>
#include"GL/glew.h"
#include"GLFW/glfw3.h"

void OnWindowResize(GLFWwindow * pWindow, int width, int height)
{
    glViewport(0, 0, width, height);
    glScissor(0, 0, width, height);
}

void CreateTriangle()
{
    float vertices[] = {
         0.0f,  0.5f, 0.0f      // top middle
        -0.5f, -0.5f, 0.0f,     // bottom left
         0.5f, -0.5f, 0.0f,     // bottom right
    };

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

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
        

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}