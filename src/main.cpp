#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

#include "../include/LoadShaders.h"

using namespace std;

#define SCREEN_WIDTH 700
#define SCREEN_HEIGHT 700

//this is for resizing later
int g_width = SCREEN_WIDTH;
int g_height = SCREEN_HEIGHT;

GLuint triangleVAO = 0;
GLuint triangleVBO = 0;
GLuint shaderProgram = 0;

// tow triangles vertices
const GLuint NumVertices = 6;
static const GLfloat vertices[NumVertices * 2] = {
  -0.90f, -0.90f,
  +0.85f, -0.90f,
  -0.90f,  +0.85f,

  //triangle 2
  +0.90f, -0.85f,
  +0.90f, +0.90f,
  -0.85f, +0.90f
};


GLFWwindow* initWindow()
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

void setupTriangles()
{
  //gen and bind vao and vbo
  glGenVertexArrays(1, &triangleVAO);
  glGenBuffers(1, &triangleVBO);

  // bind vao first
  glBindVertexArray(triangleVAO);

  //bind vbo
  glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  //setup vertex attrib
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void*)0);

  glEnableVertexAttribArray(0);
  
  glBindVertexArray(0);
}

void setupShaders()
{
  auto shader = parseShader("../res/triangles.glsl");
  shaderProgram = createShader(shader.vertexShader, shader.fragmentShader);
}

void display()
{
  //clear screen
  static const float black[] = {0.0f, 0.0f, 0.0f, 1.0f};
  glClearColor(black[0], black[1], black[2], black[3]);
  glClear(GL_COLOR_BUFFER_BIT);

  //use shader program
  glUseProgram(shaderProgram);

  //bind vao and draw
  glBindVertexArray(triangleVAO);
  glDrawArrays(GL_TRIANGLES, 0, NumVertices);
  glBindVertexArray(0);
}

void processInput(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
      glfwSetWindowShouldClose(window, true);
    }
}

void cleanup()
{
  if (triangleVBO != 0)
    {
      glDeleteBuffers(1, &triangleVBO);
      triangleVBO = 0;
    }
  if (triangleVAO != 0)
    {
      glDeleteVertexArrays(1, &triangleVAO);
      triangleVAO = 0;
    }
  if (shaderProgram != 0)
    {
      glDeleteProgram(shaderProgram);
      shaderProgram = 0;
    }
}

int main()
{
  GLFWwindow* window = initWindow();
  if (!window) return -1;

  setupTriangles();
  setupShaders();
  
  while (!glfwWindowShouldClose(window))
    {
      processInput(window);

      display();
      
      glfwSwapBuffers(window);
      glfwPollEvents();
    }

  cleanup();
  glfwTerminate();
  return 0;
}
