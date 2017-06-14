#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "Game.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define WINDOW_TITLE "Result.exe"
#define internal static

internal bool keyState[1024];	// @TODO: Check range.
internal float deltaTime;
internal raw::Game* game;

void glfwKeyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);

	if (action == GLFW_PRESS)
		keyState[key] = true;
	if (action == GLFW_RELEASE)
		keyState[key] = false;
}

void glfwCursorCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (game)
		game->processMouseChange(xPos, yPos);
}

GLFWwindow* initGlfw()
{
	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, 0, 0);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, glfwKeyCallback);
	glfwSetCursorPosCallback(window, glfwCursorCallback);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	return window;
}

void initGlew()
{
	glewExperimental = true;
	glewInit();
}

void refreshKeys()
{
}

int main()
{
	GLFWwindow* mainWindow = initGlfw();
	initGlew();

	game = new raw::Game();
	game->init();

	glEnable(GL_DEPTH_TEST);
	glLineWidth(10);

	double lastFrame = glfwGetTime();
	int frameNumber = (int)lastFrame;
	unsigned int fps = 0;

	while (!glfwWindowShouldClose(mainWindow))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

		game->update(deltaTime);
		game->render();
		game->processInput(keyState, deltaTime);

		refreshKeys();
		glfwPollEvents();
		glfwSwapBuffers(mainWindow);

		double currentFrame = glfwGetTime();
		if ((int)currentFrame > frameNumber)
		{
			std::cout << "FPS: " << fps << std::endl;
			fps = 0;
			frameNumber++;
		}
		else
			++fps;

		deltaTime = (float)(currentFrame - lastFrame);
		lastFrame = currentFrame;
	}

	game->destroy();
	glfwTerminate();
}