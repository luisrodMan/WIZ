#ifndef __WIZ_DESKTOPAPP__
#define __WIZ_DESKTOPAPP__

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cstdio>
#include <string>
#include "WIZ/wiz.h"

#include <iostream>

static void error_callback(int error, const char* description)
{

    fprintf(stderr, "Error: %s\n", description);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}


void window_size_callback(GLFWwindow* window, int width, int height)
{
	(static_cast<DefaultGraphicsImp*>(&WIZ::getGraphics()))->updateScreenSize(width, height);
	WIZ::getApplication().sizeChange(width, height);
}

class DesktopApplicationRunner : public IApplicationRunner {

private:
	int width, height;
	GLFWwindow* window;
	DefaultGraphicsImp graphics;

public:
	DesktopApplicationRunner(IApplication *application, const char *title, int width, int height) : IApplicationRunner(application) {
		this->width = width;
		this->height = height;
    
	    glfwSetErrorCallback(error_callback);

	    if (!glfwInit())
	        exit(EXIT_FAILURE);
	    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	    window = glfwCreateWindow(width, height, title, NULL, NULL);
	    if (!window)
	    {
	        glfwTerminate();
	        exit(EXIT_FAILURE);
	    }

	    // listeners
	    glfwSetKeyCallback(window, key_callback);
	    glfwSetWindowSizeCallback(window, window_size_callback);

	    glfwMakeContextCurrent(window);
	    
	    // GLEW
	    GLenum err = glewInit();
		if (GLEW_OK != err)
		{
		  /* Problem: glewInit failed, something is seriously wrong. */
		  fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		}

	    glfwSwapInterval(1);
	    glClearColor(0, 0.2f, 0.8f, 1);



	    // setup everything

	    graphics.setGameDimension(width, height);

	    WIZ::setup(application, &graphics);

	    // initialize app
	    application->initialize();
	}

	void run() {

		double startTime = glfwGetTime();
		double lastTime = startTime;
		int fps = 0;

		while (!glfwWindowShouldClose(window))
	    {
	    	
	    	double curTime = glfwGetTime();
	    	double delta = (float) (curTime - lastTime);

	    	graphics.updateOthers(delta, (float)(curTime-startTime), fps);

	    	application->renderer(delta);
	    	lastTime = curTime;

	        glfwSwapBuffers(window);
	        glfwPollEvents();
	    }
	    glfwDestroyWindow(window);
	    glfwTerminate();
	}

};

#endif