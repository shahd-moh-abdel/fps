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

GLuint quadVAO = 0;
GLuint quadVBO = 0;
GLuint quadEBO = 0;
GLuint shaderProgram = 0;

// quad vertices
const GLuint NumVertices = 6;
static const GLfloat vertices[NumVertices * 2] = {
  -0.5f, -0.5f,
  +0.5f, -0.5f,
  +0.5f, +0.5f,
  -0.5f, +0.5f
};

static const GLuint indices[] = {
  0, 1, 2,//first tri
  2, 3, 0 //second tri
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

void setupQuad()
{
  //gen and bind vao and vbo
  glGenVertexArrays(1, &quadVAO);
  glGenBuffers(1, &quadVBO);
  glGenBuffers(1, &quadEBO);

  // bind vao first
  glBindVertexArray(quadVAO);

  //bind and fill vbo
  glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  //indices buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadEBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
  
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
  static const float darkGray[] = {0.2f, 0.2f, 0.2f, 1.0f};
  glClearColor(darkGray[0], darkGray[1], darkGray[2], darkGray[3]);
  glClear(GL_COLOR_BUFFER_BIT);

  //use shader program
  glUseProgram(shaderProgram);

  //bind vao and draw
  glBindVertexArray(quadVAO);
  glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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
  if (quadVBO != 0)
    {
      glDeleteBuffers(1, &quadVBO);
      quadVBO = 0;
    }
  if (quadVAO != 0)
    {
      glDeleteVertexArrays(1, &quadVAO);
      quadVAO = 0;
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

  setupQuad();
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
