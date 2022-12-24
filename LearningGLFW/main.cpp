#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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

    //The loop
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        //Render here
        glClear(GL_COLOR_BUFFER_BIT);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

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