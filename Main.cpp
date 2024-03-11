#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb/stb_image.h"

#include "shaderClass.h"
#include "VBO.h"
#include "IBO.h"
#include "VAO.h"
#include "Texture.h"

#include <cmath>


//Vertices Coordinates
GLfloat vertices[] =
{   //  COORDINATES     /      COLORS       /  texture coordinates
    -0.5f, -0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   0.0f, 0.0f,
    -0.5f,  0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f,
     0.5f,  0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   1.0f, 1.0f,
     0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f
};

// Indices for vertices order
GLuint indices[] =
{
    0, 2, 1,
    0, 3, 2
};

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 800, "OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    //Load GLAD
    gladLoadGL();
    //Specify the viewport of OpenGL in the window
    glViewport(0, 0, 800, 800);


    Shader shaderProgram("default.vert", "default.frag");

    VAO VAO1;
	VAO1.Bind();

    VBO VBO1(vertices, sizeof(vertices));
    IBO IBO1(indices, sizeof(indices));

    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(GL_FLOAT), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(GL_FLOAT), (void*)(6 * sizeof(GL_FLOAT)));
    VAO1.Unbind();
    VBO1.Unbind();
    IBO1.Unbind();

    GLuint uniformID = glGetUniformLocation(shaderProgram.ID, "scale");

    stbi_set_flip_vertically_on_load(true);

    Texture texture("thug_cat.jpg", GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE);
    texture.TexUnit(shaderProgram, "tex0", 0);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shaderProgram.Activate();
        glUniform1f(uniformID, 1.5f);
        texture.Bind();
        VAO1.Bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    VAO1.Delete();
    VBO1.Delete();
    IBO1.Delete();
    shaderProgram.Delete();


    glfwTerminate();
    return 0;
}