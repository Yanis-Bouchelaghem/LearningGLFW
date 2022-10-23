#include <glad/glad.h>
#include <glfw3.h>
#include <iostream>
int main()
{
	if (!glfwInit()) {
		std::cout << "Could not initialize glfw.\n";
		return -1;
	}

	GLFWwindow * window;
	window = glfwCreateWindow(640, 480, "OpenGL TP 1", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	gladLoadGL();
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
		while (!glfwWindowShouldClose(window))
		{
			glClear(GL_COLOR_BUFFER_BIT);
			glfwSwapBuffers(window);
			glfwPollEvents();
		}
	glfwDestroyWindow(window);
	glfwTerminate();
}