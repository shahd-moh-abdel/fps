#include "../include/initWindow.h"
#include <iostream>

using namespace std;

GLFWwindow* initWindow(int SCREEN_WIDTH, int SCREEN_HEIGHT)
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  
  GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "FPS", NULL, NULL);

  //error creating the window
  if (window == nullptr)
    {
      cout << "ERROR: Window not created" << endl;
      return nullptr;
    }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
      cout << "ERROR: glad failed" << endl;
      return nullptr;
    }
  
  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

  return window;
}
