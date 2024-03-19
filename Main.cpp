#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "stb/stb_image.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "shaderClass.h"
#include "VBO.h"
#include "IBO.h"
#include "VAO.h"
#include "Texture.h"

#include <cmath>


// Vertices coordinates
GLfloat vertices[] =
{ //     COORDINATES     /        COLORS      /   TexCoord  //
    -0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
    -0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
     0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	0.0f, 0.0f,
     0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	5.0f, 0.0f,
     0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	2.5f, 5.0f
};

// Indices for vertices order
GLuint indices[] =
{
    0, 1, 2,
    0, 2, 3,
    0, 1, 4,
    1, 2, 4,
    2, 3, 4,
    3, 0, 4
};

const unsigned int width = 800;
const unsigned int height = 800;

void ProcessInputs(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
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
    glViewport(0, 0, width, height);


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

    Texture texture("brick.png", GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE);
    texture.TexUnit(shaderProgram, "tex0", 0);

    // Variables that help the rotation of the pyramid
    float rotation = 0.0f;
    double prevTime = glfwGetTime();

    // Enables the Depth Buffer
    glEnable(GL_DEPTH_TEST);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    /* Main Loop */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Simple timer
        double crntTime = glfwGetTime();
        if (crntTime - prevTime >= 1 / 60)
        {
            rotation += 0.05f;
            prevTime = crntTime;
        }

        // Initializes matrices so they are not the null matrix
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 proj = glm::mat4(1.0f);

        // Assigns different transformations to each matrix
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, -0.5f, -2.0f));
        proj = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);

        // Outputs the matrices into the Vertex Shader
        int modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        int projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

        shaderProgram.Activate();
        glUniform1f(uniformID, 1.5f);
        texture.Bind();
        VAO1.Bind();

        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);

        // Process Inputs
        ProcessInputs(window);

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