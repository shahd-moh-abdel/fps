#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>

#include <vector>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
#include "../include/LoadShaders.h"
#include "../include/initWindow.h"
#include "../include/loadTexture.h"

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

vector<GLuint> textures;
vector<const char *> textureNames = {
  "../res/wall.jpg",
  "../res/floor.jpg",
  "../res/ceiling.jpg"
};

// quad vertices
const GLuint NumVertices = 4;
// (x, y)-position, (u, v)-texture coords 
static const GLfloat vertices[NumVertices * 4] = {
  -0.5f, -0.5f,      0.0f, 0.0f,
  +0.5f, -0.5f,      1.0f, 0.0f,
  +0.5f, +0.5f,      1.0f, 1.0f,
  -0.5f, +0.5f,      0.0f, 1.0f
};

static const GLuint indices[] = {
    0, 1, 2, // first tri
    2, 3, 0  // second tri
};

struct Mat4 {
  float m[16];

  Mat4() {
    // init as identity mat
    for (int i = 0; i < 16; i++) m[i] = 0.0f;
    m[0] = m[5] = m[10] = m[15] = 1.0f;
  }

  // translation matrix
  static Mat4 translate(float x, float y, float z) {
    Mat4 result;
    result.m[12] = x;
    result.m[13] = y;
    result.m[14] = z;
    return result;
  }

  static Mat4 scale(float x, float y, float z) {
    Mat4 result;
    result.m[0] = x;
    result.m[5] = y;
    result.m[10] = z;
    return result;
  }

  //rotate around z
  static Mat4 rotateZ(float angleRadians) {
    Mat4 result;
    float c = cos(angleRadians);
    float s = sin(angleRadians);
    result.m[0] = c;
    result.m[1] = s;
    result.m[4] = -s;
    result.m[5] = c;
    return result;
  }

  // matrix multiplication
  Mat4 operator*(const Mat4& other) const {
    Mat4 result;
    for (int i = 0; i < 4; i++)
      {
	for (int j = 0; j < 4; j++)
	  {
	    result.m[i*4 + j] = 0;
	    for (int k = 0; k < 4; k++)
	      result.m[i*4 + j] += m[i*4 + k] * other.m[k*4 + j];
	  }
      }
    return result;
  }
};

struct QuadInstance {
  float x, y, z;
  float scaleX, scaleY, scaleZ;
  float rotation;
  int textureIndex;
};

// define quads
vector<QuadInstance> quadInstances = {
  // x,   y,  z,    scaleX,  scaleY,  scaleZ,  rotation,  textureIndex
  {0.0, 0.5, 0.0f, 1.0f,    1.0f,    1.0f,    0.0f,        0} //left wall
};

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
  auto shader = parseShader("../res/transformed.glsl");
  shaderProgram = createShader(shader.vertexShader, shader.fragmentShader);
}

void setupTextures()
{
  textures.clear();

  for (const char* textureName : textureNames)
    {
      GLuint tex = loadTexture(textureName);
      textures.push_back(tex);
    }
  cout << "Loaded " << textures.size() << " textures" << endl;
}

void display()
{
  //clear screen
  static const float darkGray[] = {0.2f, 0.2f, 0.2f, 1.0f};
  glClearColor(darkGray[0], darkGray[1], darkGray[2], darkGray[3]);
  glClear(GL_COLOR_BUFFER_BIT);

  //use shader program
  glUseProgram(shaderProgram);
  
  //gete uniform locations
  GLint modelLocation = glGetUniformLocation(shaderProgram, "u_model");  
  GLint textureLocation = glGetUniformLocation(shaderProgram, "u_texture");

  //bind vao and draw
  glBindVertexArray(quadVAO);

  //render each quad instance
  for (const auto& quad : quadInstances) {
    Mat4 translation = Mat4::translate(quad.x, quad.y, quad.z);
    Mat4 rotation = Mat4::rotateZ(quad.rotation);
    Mat4 scaling =  Mat4::scale(quad.scaleX, quad.scaleY, quad.scaleZ);

    //combine transformations
    Mat4 model = translation * rotation * scaling;

    //send matrix to shader
    glUniformMatrix4fv(modelLocation, 1, GL_FALSE, model.m);

    //bind texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[quad.textureIndex]);
    glUniform1i(textureLocation, 0);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
  }

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
  textures.clear();
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
