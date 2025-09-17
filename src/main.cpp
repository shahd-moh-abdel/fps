#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;

#define SCREEN_WIDTH 700
#define SCREEN_HEIGHT 700

//this is for resizing later
int g_width = SCREEN_WIDTH;
int g_height = SCREEN_HEIGHT;

GLFWwindow* initWindow()
{
  glfwInit();
  GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "FPS", NULL, NULL);

  //error creating the window
  if (window == GL_FALSE)
    {
      cout << "ERROR: Window not created" << endl;
      return nullptr;
    }

  glfwMakeContextCurrent(window);
  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

  return window;
}

void processInput(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
      glfwSetWindowShouldClose(window, true);
    }
}

int main()
{
  GLFWwindow* window = initWindow();

  while (!glfwWindowShouldClose(window))
    {
      processInput(window);

      glClearColor(0.7f, 0.8f, 0.7f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      glfwSwapBuffers(window);
      glfwPollEvents();
    }

  glfwTerminate();
  return 0;
}
