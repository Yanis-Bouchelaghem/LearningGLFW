#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
#include "Shader.h"
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif

    //Create a window and set its context to the current thread.
    GLFWwindow* window = glfwCreateWindow(800, 600, "You're the best", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to init GLFW.\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    //Get the openGL functions addresses using GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
       
    //Set the viewport of the window
    glViewport(0, 0, 800, 600);
    //Set the background clear color.
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    //Set the window resize callback function
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    //The coordinates of our triangle in NDC
    float vertices[] = {
        //Positions             //RGB               //Texture coordinates
         0.5f,  0.5f, 0.0f,     1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,     0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,     0.0f, 0.0f, 1.0f,   0.0f, 0.0f,// bottom left
        -0.5f,  0.5f, 0.0f,     1.0f, 1.0f, 1.0f,   0.0f, 1.0f // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    ////////////////Load the shaders/////////////////////////

    Shader shaderProgram{ "..\\Shaders\\VertexShader.vert", "..\\Shaders\\FragmentShader.frag" };

    ///////////////Create VAOs and VBOs//////////////////////

    //Create a VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    //Create a Vertex Buffer Object
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //Copy the vertex data into the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //Tell openGL how to read the VBO into the input of the vertex shader.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(vertices[0]), 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(vertices[0]), (void*)(3 * sizeof(vertices[0])));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(vertices[0]), (void*)(6 * sizeof(vertices[0])));
    glEnableVertexAttribArray(2);
    //Create the element buffer object.
    unsigned int EBO;
    glGenBuffers(1, &EBO);
    //Bind the EBO and fill its data.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    //Unbind the VAO, it contains all of the info about how to read the VBO.
    //Always unbind the VAO first. If we unbind the EBO first, it will also be unbound in the VAO.
    glBindVertexArray(0);
    //Unbind the VBO.
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    //Create the OpenGL texture.
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping/filtering options (on the currently bound texture object)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //Load the texture
    int width, height, nrChannels;
    unsigned char* data = stbi_load("..\\Textures\\container.jpg", &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else

    {
        std::cout << "ERROR::TEXTURE::LOADING_FAILED\n";
    }

    stbi_image_free(data);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    



    //The loop
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        //Render here
        glClear(GL_COLOR_BUFFER_BIT);
        //Bind texture
        glBindTexture(GL_TEXTURE_2D, texture); //Set the uniform for the texture sampler in the fragment shader
        
        //Create the transformation
        glm::mat4 transformation = glm::mat4{ 1.0f }; //Identity matrix
        transformation = glm::translate(transformation, glm::vec3{ 0.5f, 0.5f, 0.0f });
        transformation = glm::rotate(transformation, (float)glfwGetTime() , glm::vec3{ 0.0f, 0.0f, 1.0f });

        //Activate the shader program
        shaderProgram.Use();
        //Send uniforms to shader
        unsigned int transformLocation = glGetUniformLocation(shaderProgram.GetID(), "transform");
        glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(transformation));
        glBindVertexArray(VAO);

        //glDrawArrays(GL_TRIANGLES, 0, 3);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glfwTerminate();
    return 0;
}


void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}