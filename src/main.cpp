#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
#include "../include/LoadShaders.h"
#include "../include/initWindow.h"

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
GLuint texture = 0;

// quad vertices
const GLuint NumVertices = 4;
// (x, y)-position, (u, v)-texture coords 
static const GLfloat vertices[NumVertices * 4] = {
  -0.5f, -0.5f,      0.0f, 0.0f,
  +0.5f, -0.5f,      2.0f, 0.0f,
  +0.5f, +0.5f,      2.0f, 2.0f,
  -0.5f, +0.5f,      0.0f, 2.0f
};

static const GLuint indices[] = {
  0, 1, 2,//first tri
  2, 3, 0 //second tri
};

GLuint loadTexture(const char* path)
{
  GLuint textureID;
  glGenTextures(1, &textureID);

  int width, height, nrComponents;
  unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
  if (data)
    {
      GLenum format = GL_RGB;
      if (nrComponents == 1)
	format = GL_RED;
      else if (nrComponents == 3)
	format = GL_RGB;
      else if (nrComponents == 4)
	format = GL_RGBA;

      glBindTexture(GL_TEXTURE_2D, textureID);
      glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

      glGenerateMipmap(GL_TEXTURE_2D);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      stbi_image_free(data);

      cout << path << '\n' << width << ":" << height << endl;
    }
  else
    {
      cout << "failed to load texture: " << path << endl;
      stbi_image_free(data);

      //fallback texture
      unsigned char fallbackData[] = {
	255, 0, 0, 255,
	0, 0, 0, 255,
	0, 0, 0, 255,
	255, 0, 0, 255
      };

      glBindTexture(GL_TEXTURE_2D, textureID);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, fallbackData);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

      cout << "using fallback texture" << endl;
    }
  return textureID;
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
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)0);
  glEnableVertexAttribArray(0);

  // texture coord loc=1
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);
  
  glBindVertexArray(0);
}

void setupShaders()
{
  auto shader = parseShader("../res/triangles.glsl");
  shaderProgram = createShader(shader.vertexShader, shader.fragmentShader);
}

void setupTextures()
{
  texture = loadTexture("../res/wall.jpg");
}

void display()
{
  //clear screen
  static const float darkGray[] = {0.2f, 0.2f, 0.2f, 1.0f};
  glClearColor(darkGray[0], darkGray[1], darkGray[2], darkGray[3]);
  glClear(GL_COLOR_BUFFER_BIT);

  //use shader program
  glUseProgram(shaderProgram);

  //bind texture
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);

  //set texture uniform
  GLint textureLocation = glGetUniformLocation(shaderProgram, "u_wallTexture");
  glUniform1i(textureLocation, 0);

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
  if (quadEBO != 0)
    {
      glDeleteBuffers(1, &quadEBO);
      quadEBO = 0;
    }
  if (quadVAO != 0)
    {
      glDeleteVertexArrays(1, &quadVAO);
      quadVAO = 0;
    }
  if (texture != 0)
    {
      glDeleteTextures(1, &texture);
      texture = 0;
    }
  if (shaderProgram != 0)
    {
      glDeleteProgram(shaderProgram);
      shaderProgram = 0;
    }
}

int main()
{
  GLFWwindow* window = initWindow(SCREEN_WIDTH, SCREEN_HEIGHT);
  if (!window) return -1;

  setupQuad();
  setupShaders();
  setupTextures();
  
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
