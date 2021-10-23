#include<iostream>
#include"GL/glew.h"
#include"GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/string_cast.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"

std::string GetEnv(std::string strName)
{
    char* libvar;
    size_t requiredSize;
    getenv_s(&requiredSize, nullptr, 0, strName.c_str());
    if (requiredSize == 0)
    {
        //   LogMessage(fmt::format("Environment variable {0} doesn't exist!", varName).c_str());
        return nullptr;
    }

    libvar = (char*)malloc(requiredSize * sizeof(char));
    if (!libvar)
    {
        //   LogMessage(fmt::format("Failed to allocate memory {0} bytes!", requiredSize).c_str());
        return nullptr;
    }

    // Get the value of the LIB environment variable.
    getenv_s(&requiredSize, libvar, requiredSize, strName.c_str());

    return libvar;
}

std::string GetMediaPath()
{
    return GetEnv("MEDIA_PATH") + "\\";
}

std::string GetTexturePath()
{
    return GetMediaPath() + "textures\\";
}

void OnWindowResize(GLFWwindow * pWindow, int width, int height)
{
    glViewport(0, 0, width, height);
    glScissor(0, 0, width, height);
}

unsigned int program;
float Scale = 1.0f;

void OnMouseScroll(GLFWwindow* pWindow, double x, double y)
{
    Scale -= y * 0.1f;
    glUniform1f(glGetUniformLocation(program, "uScale"), Scale);
}

unsigned int CreateTriangle(unsigned int & vertexCount)
{
    float vertices[] = {
        //  (x, y, z, w)
        -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f,      // bottom left
         0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f,     // bottom right
         0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 1.0f,     // top right
        -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f     // top left
    };

    float colors[] = {
         //  (r, g, b)
        1.0f, 1.0f, 1.0f,     // bottom left
        1.0f, 1.0f, 1.0f,     // bottom right
        1.0f, 1.0f, 1.0f,     // top right
        1.0f, 1.0f, 1.0f,     // top left
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    vertexCount = 6;

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    {
        unsigned int VBOPos;
        glGenBuffers(1, &VBOPos);
        glBindBuffer(GL_ARRAY_BUFFER, VBOPos);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // at location 0, read 4 floats and jump (stride) 4 floats for next vertex.
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, (6) * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, (6) * sizeof(float), (void*)(4*sizeof(float)));
        glEnableVertexAttribArray(1);

        /*
        unsigned int VBOCol;
        glGenBuffers(1, &VBOCol);
        glBindBuffer(GL_ARRAY_BUFFER, VBOCol);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (3) * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        */

        unsigned int EBO;
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    }
    glBindVertexArray(0);

    return VAO;
}

unsigned int CreateShaders()
{
    const char* VertexShaderSource =
        "#version 330 core\n"
        "layout(location = 0) in vec4 aPos;\n"
        "layout(location = 1) in vec2 TexCoord;\n"
        "//layout(location = 1) in vec3 Color;\n"
        "uniform vec3 uColor;\n"
        "uniform float uTime;\n"
        "uniform mat4 uScale;\n"
        "uniform mat4 uRotate;\n"
        "uniform mat4 uTranslate;\n"
        "uniform mat4 uCombinedTransform;\n"
        "out vec3 colorFrag;\n"
        "out vec2 UVFrag;\n"
        "const float PI = 3.14;\n"
        "const float Angle = 90.0f * PI/180.0f;"
        "void main()\n"
        "{\n"
            "//mat4 combined = uTranslate * uRotate * uScale;\n"
            "gl_Position = uCombinedTransform * aPos;\n"
            "UVFrag = TexCoord;\n"
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
        "in vec3 colorFrag;\n"
        "in vec2 UVFrag;\n"
        "uniform vec3 uColor[2];\n"
        "out vec4 FragColor;\n"
        "uniform sampler2D Texture;\n"
        "uniform sampler2D Second;\n"
        "void main()\n"
        "{\n"
            "vec2 scaledUV = (UVFrag - vec2(0.5)) * vec2(2.0) + vec2(0.5);\n"
            "vec2 scaledUV1 = UVFrag * vec2(2.0) - vec2(0.5);\n"
            "vec4 colorMinionOne = texture(Texture, UVFrag);\n"
            "//vec4 colorMinionTwo = texture(Second, UVFrag);\n"
            "//vec4 colorMultiply = colorMinionOne* colorMinionTwo;\n"
            "//vec4 finalColor = colorMinionOne*colorMinionOne.a + colorMinionTwo*(1.0 - colorMinionOne.a);\n"
            "FragColor = colorMinionOne;\n"
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

unsigned int LoadTexture(std::string strFileName)
{
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(strFileName.c_str(), &width, &height, &nrChannels, 0);

    if (!data)
    {
        std::cout << "Failed to load the image :" << strFileName << std::endl;
        return 0;
    }

    std::cout << "Image Name : " << strFileName << std::endl;
    std::cout << "Image Width : " << width << std::endl;
    std::cout << "Channels : " << nrChannels << std::endl;

    GLint format = (nrChannels == 4) ? GL_RGBA : GL_RGB;

    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    //float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
    //glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    // Filtering ( Tri-linear filtering )
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    return texture;
}

int main()
{
    /* GLM Introduction
    glm::vec4 point1(-0.5f, -0.5f, 0.0f, 1.0f); // A
    glm::vec4 point2(0.5f, -0.5f, 0.0f, 1.0f);  // B
    glm::vec4 point3(0.5f, 0.5f, 0.0f, 1.0f);   // C
    glm::vec4 point4(-0.5f, 0.5f, 0.0f, 1.0f);

    // BA
    glm::vec3 direction1 = glm::normalize(point2 - point1);

    // CA
    glm::vec3 direction2 = glm::normalize(point3 - point1);

    float consineAngle = glm::dot(direction1, direction2);
    float Angle = glm::acos(consineAngle) * 180.0f / glm::pi<float>();
    std::cout << glm::to_string(direction1) << std::endl;
    std::cout << glm::to_string(direction2) << std::endl;
    std::cout << Angle << std::endl;
    glm::vec3 normal = glm::normalize(glm::cross(direction1, direction2));
    std::cout << glm::to_string(normal) << std::endl;

    glm::mat4 matScale = glm::identity<glm::mat4>();
    matScale = glm::scale(matScale, glm::vec3(2.0f, 1.0f, 1.0f));
    std::cout << glm::to_string(matScale) << std::endl;

    glm::mat4 matRotate = glm::identity<glm::mat4>();
    matRotate = glm::rotate(matRotate, 45.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    std::cout << glm::to_string(matRotate) << std::endl;

    glm::mat4 matTranslate = glm::identity<glm::mat4>();
    matTranslate = glm::translate(matTranslate, glm::vec3(1.0f, 0.0f, 0.0f));
    std::cout << glm::to_string(matTranslate) << std::endl;

    glm::mat4 matCombined = matTranslate * matRotate * matScale;
    std::cout << glm::to_string(matCombined) << std::endl;

    glm::vec4 scaledPoint1 = matCombined * point1;
    std::cout << glm::to_string(scaledPoint1) << std::endl;

    matCombined = glm::identity<glm::mat4>();
    matCombined = glm::translate(matCombined, glm::vec3(1.0f, 0.0f, 0.0f));
    matCombined = glm::rotate(matCombined, 45.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    matCombined = glm::scale(matCombined, glm::vec3(2.0f, 1.0f, 1.0f));
    std::cout << glm::to_string(matCombined) << std::endl;

    scaledPoint1 = matCombined * point1;
    std::cout << glm::to_string(scaledPoint1) << std::endl;

    getchar();
    return 0;
    */

    GLFWwindow* window;
    {
        /* Initialize the library */
        if (!glfwInit())
            return -1;

        int width = 1920;
        int height = 1080;

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

        glfwSetScrollCallback(window, OnMouseScroll);

        // TODO: Move to material
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);    // Transparent blend or Alpha blend or Transparent
        //glBlendFunc(GL_ONE, GL_ZERO); // Opaque or Replace blend
        //glBlendFunc(GL_ONE_MINUS_SRC_COLOR, GL_ZERO);

        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);

        glFrontFace(GL_CCW);

        glEnable(GL_SCISSOR_TEST);

        glViewport(0, 0, width, height);
        glScissor(0, 0, width, height);

    }

    unsigned int vertexCount = 0;
    // Entities
    unsigned int triangle = CreateTriangle(vertexCount);
    program = CreateShaders();

    glUseProgram(program);
    glUniform1f(glGetUniformLocation(program, "uScale"), 1.0f);

    glm::mat4 matScale = glm::identity<glm::mat4>();
    matScale = glm::scale(matScale, glm::vec3(2.0f, 1.0f, 1.0f));
    //std::cout << glm::to_string(matScale) << std::endl;

    glm::mat4 matRotate = glm::identity<glm::mat4>();
    matRotate = glm::rotate(matRotate, 45.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    //std::cout << glm::to_string(matRotate) << std::endl;

    glm::mat4 matTranslate = glm::identity<glm::mat4>();
    matTranslate = glm::translate(matTranslate, glm::vec3(0.2f, 0.0f, 0.0f));
    //std::cout << glm::to_string(matTranslate) << std::endl;

    glUniformMatrix4fv(glGetUniformLocation(program, "uScale"), 1, GL_FALSE, &(matScale[0][0]));
    glUniformMatrix4fv(glGetUniformLocation(program, "uRotate"), 1, GL_FALSE, &(matRotate[0].x));
    glUniformMatrix4fv(glGetUniformLocation(program, "uTranslate"), 1, GL_FALSE, glm::value_ptr(matTranslate));

    glm::mat4 matCombined = glm::identity<glm::mat4>();
    matCombined = glm::translate(matCombined, glm::vec3(0.0f, 0.0f, 0.0f));
    matCombined = glm::rotate(matCombined, 45.0f, glm::vec3(0.0f, 0.0f, 1.0f));
    matCombined = glm::scale(matCombined, glm::vec3(2.0f, 1.0f, 1.0f));

    glUniformMatrix4fv(glGetUniformLocation(program, "uCombinedTransform"), 1, GL_FALSE, glm::value_ptr(matCombined));

    //unsigned int texture = LoadTexture(GetTexturePath() + "small_texture.png");
    unsigned int texture = LoadTexture(GetTexturePath() + "minions\\minion.png");
    unsigned int texture2 = LoadTexture(GetTexturePath() + "minions\\minion.jpg");


    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(0.0f, 0.0f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);


        float time = (float)glfwGetTime();
        float fTime = 1.0f + fabs(sinf(time*0.25f)*2.0f);
        float fCos = cosf(time * 0.2f);
        glUniform1f(glGetUniformLocation(program, "uTime"), 1.0f);
        //glUniform1f(glGetUniformLocation(program, "uScale"), 0.5f);

        float color[] = { 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f };
        glUniform3fv(glGetUniformLocation(program, "uColor"), 2, color);
        /*
        glm::mat4 matTranslate = glm::identity<glm::mat4>();
        matTranslate = glm::translate(matTranslate, glm::vec3(0.2f*fTime, 0.0f, 0.0f));
        glUniformMatrix4fv(glGetUniformLocation(program, "uTranslate"), 1, GL_FALSE, glm::value_ptr(matTranslate));

        glm::mat4 matRotate = glm::identity<glm::mat4>();
        matRotate = glm::rotate(matRotate, 45.0f*fCos, glm::vec3(0.0f, 0.0f, 1.0f));
        glUniformMatrix4fv(glGetUniformLocation(program, "uRotate"), 1, GL_FALSE, &(matRotate[0].x));
        */
        glUseProgram(program);
        glBindVertexArray(triangle);
        glActiveTexture(GL_TEXTURE0); // activate the texture unit first before binding texture
        glBindTexture(GL_TEXTURE_2D, texture);
        glActiveTexture(GL_TEXTURE1); // activate the texture unit first before binding texture
        glBindTexture(GL_TEXTURE_2D, texture2);

        glUniform1i(glGetUniformLocation(program, "Texture"), 0); // set it manually
        glUniform1i(glGetUniformLocation(program, "Second"), 1); // set it manually

        //glDrawArrays(GL_TRIANGLES, 0, vertexCount);
        glDrawElements(GL_TRIANGLES, vertexCount /* index count */, GL_UNSIGNED_INT, 0);

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