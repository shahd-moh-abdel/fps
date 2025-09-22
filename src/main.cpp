#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include <cstddef>
#include <iostream>

#include "../include/glm/glm.hpp"
#include "../include/glm/gtc/matrix_transform.hpp"
#include "../include/glm/gtc/type_ptr.hpp"

#include <vector>
#include <cmath>

#define STB_IMAGE_IMPLEMENTATION
#include "../include/stb_image.h"
#include "../include/LoadShaders.h"
#include "../include/initWindow.h"
#include "../include/loadTexture.h"
//#include "../include/Mat4.h"

using namespace std;

#define SCREEN_WIDTH 700
#define SCREEN_HEIGHT 700

//this is for resizing later
int g_width = SCREEN_WIDTH;
int g_height = SCREEN_HEIGHT;

GLuint cubeVAO = 0;
GLuint cubeVBO = 0;
GLuint cubeEBO = 0;
GLuint shaderProgram = 0;

vector<GLuint> textures;
vector<const char *> textureNames = {
  "../res/wall.jpg",
  "../res/floor.jpg",
  "../res/ceiling.jpg"
};

// cube vertices with texture coordinates
const GLuint NumVertices = 36;

// (x, y, z)-position, (u, v)-texture coordinates 
static const GLfloat cubeVertices[NumVertices * 5] = {
  //front
  -0.5f, -0.5f, 0.5f,   0.0f, 0.0f,
   0.5f, -0.5f, 0.5f,   1.0f, 0.0f,
   0.5f,  0.5f, 0.5f,   1.0f, 1.0f,
   0.5f,  0.5f, 0.5f,   1.0f, 1.0f,
  -0.5f,  0.5f, 0.5f,   0.0f, 1.0f,
  -0.5f, -0.5f, 0.5f,   0.0f, 0.0f,

  //back
  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
   0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
   0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
   0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

  //left
  -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
  -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
  -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

  //right
  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
  0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
  0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
  0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

  //bottom
  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
   0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
   0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
   0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

  //top
  
  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
   0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
   0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
   0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
  -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

struct CubeInstance {
  glm::vec3 position;
  glm::vec3 scale;
  glm::vec3 rotation;
  int textureIndex;
};

// define quads
vector<CubeInstance> cubeInstances = {
  //position,           scale,               rotation,           textureIndex
  {{0.0f,  0.5f, 0.0f}, {1.0f, 1.0f, 1.0f},  {0.0f, 0.0f, 0.0f},  0}
};

void setupQuad()
{
  //gen and bind vao and vbo
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &cubeVBO);
  glGenBuffers(1, &cubeEBO);

  // bind vao first
  glBindVertexArray(cubeVAO);

  //bind and fill vbo
  glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

  //setup vertex attrib
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)0);
  glEnableVertexAttribArray(0);

  // texture coord loc=1
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*)(2 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);
  
  glBindVertexArray(0);
}

void setupShaders()
{
  auto shader = parseShader("../res/cube.glsl");
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
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  //use shader program
  glUseProgram(shaderProgram);

  //create view matrix
  glm::mat4 view = glm::translate(glm::mat4(1.0f),
				  glm::vec3(0.0f, -1.0f, -8.0f));

  //create perspective projection matrix
  float aspectRatio = (float)g_width / (float)g_height;
  glm::mat4 projection = glm::perspective(glm::radians(45.0f),
					  aspectRatio,
					  0.1f,
					  100.0f);

  //get uniform locations
  GLint modelLocation = glGetUniformLocation(shaderProgram, "u_model");
  GLint viewLocation = glGetUniformLocation(shaderProgram, "u_view");
  GLint projectionLocation = glGetUniformLocation(shaderProgram, "u_projection");
  GLint textureLocation = glGetUniformLocation(shaderProgram, "u_texture");

  //send view and projection matrices
  glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

  glBindVertexArray(cubeVAO);

  //render cubes
  static float time = 0.0f;
  time += 0.016; //60 FPS

  for (size_t i = 0; i < cubeInstances.size(); i++)
    {
      auto cube = cubeInstances[i];

      if (i == 1)
	{
	  cube.rotation.y += time * 0.5;
	}
      if (i == 4)
	{
	  cube.rotation.x += time * 0.8f;
	  cube.rotation.z += time * 1.2f;
	}

      //create transformation matrix
      glm::mat4 model = glm::mat4(1.0f);

      //apply transformation translate, rotate, scale
      model = glm::translate(model, cube.position);
      model = glm::rotate(model,
			  cube.rotation.y,
			  glm::vec3(0.0f, 1.0f, 0.0f));
      model = glm::rotate(model,
			  cube.rotation.x,
			  glm::vec3(1.0f, 0.0f, 0.0f));
      model = glm::rotate(model,
			  cube.rotation.z,
			  glm::vec3(0.0f, 0.0f, 1.0f));
      model = glm::scale(model, cube.scale);

      //send model to shader
      glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

      //bind texture
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, textures[cube.textureIndex]);
      glUniform1i(textureLocation, 0);

      glDrawArrays(GL_TRIANGLES, 0, NumVertices);
			
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
  if (cubeVBO != 0)
    {
      glDeleteBuffers(1, &cubeVBO);
      cubeVBO = 0;
    }
  if (cubeVAO != 0)
    {
      glDeleteVertexArrays(1, &cubeVAO);
      cubeVAO = 0;
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
