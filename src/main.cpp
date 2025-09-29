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
#include "../include/Camera.h"
#include "../include/World.h"

using namespace std;

#define SCREEN_WIDTH 700
#define SCREEN_HEIGHT 700

//this is for resizing later
int g_width = SCREEN_WIDTH;
int g_height = SCREEN_HEIGHT;

//camera
Camera camera(glm::vec3(3.0f, 0.0f, 3.0f));
World world;

//mouse input
bool firstMouse = true;
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

GLuint cubeVAO = 0;
GLuint cubeVBO = 0;
GLuint shaderProgram = 0;

vector<GLuint> textures;
vector<const char *> textureNames = {
  "../res/wall.png",
  "../res/floor.jpg",
  "../res/ceiling_2.png"
};

// cube vertices with texture coordinates
const GLuint NumVertices = 36;

// (x, y, z)-position, (u, v)-texture coordinates, normals 
static const GLfloat cubeVertices[NumVertices * 8] = {
  //front
  -0.5f, -0.5f, 0.5f,   0.0f, 0.0f,         0.0f, 0.0f, 1.0f,
   0.5f, -0.5f, 0.5f,   1.0f, 0.0f,         0.0f, 0.0f, 1.0f,
   0.5f,  0.5f, 0.5f,   1.0f, 1.0f,         0.0f, 0.0f, 1.0f, 
   0.5f,  0.5f, 0.5f,   1.0f, 1.0f,         0.0f, 0.0f, 1.0f,
  -0.5f,  0.5f, 0.5f,   0.0f, 1.0f,         0.0f, 0.0f, 1.0f,
  -0.5f, -0.5f, 0.5f,   0.0f, 0.0f,         0.0f, 0.0f, 1.0f,
 
  //back
  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,         0.0f, 0.0f, -1.0f,
   0.5f, -0.5f, -0.5f,  1.0f, 0.0f,         0.0f, 0.0f, -1.0f,
   0.5f,  0.5f, -0.5f,  1.0f, 1.0f,         0.0f, 0.0f, -1.0f,
   0.5f,  0.5f, -0.5f,  1.0f, 1.0f,         0.0f, 0.0f, -1.0f,
  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,         0.0f, 0.0f, -1.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,         0.0f, 0.0f, -1.0f,

  //left
  -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,         -1.0f, 0.0f, 0.0f,
  -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,         -1.0f, 0.0f, 0.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,         -1.0f, 0.0f, 0.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,         -1.0f, 0.0f, 0.0f,
  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,         -1.0f, 0.0f, 0.0f,
  -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,         -1.0f, 0.0f, 0.0f,

  //right
  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,          1.0f, 0.0f, 0.0f,
  0.5f,  0.5f, -0.5f,  1.0f, 1.0f,          1.0f, 0.0f, 0.0f,  
  0.5f, -0.5f, -0.5f,  0.0f, 1.0f,          1.0f, 0.0f, 0.0f,
  0.5f, -0.5f, -0.5f,  0.0f, 1.0f,          1.0f, 0.0f, 0.0f,
  0.5f, -0.5f,  0.5f,  0.0f, 0.0f,          1.0f, 0.0f, 0.0f,
  0.5f,  0.5f,  0.5f,  1.0f, 0.0f,          1.0f, 0.0f, 0.0f,

  //bottom
  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,         0.0f, -1.0f, 0.0f,
  +0.5f, -0.5f, -0.5f,  1.0f, 1.0f,         0.0f, -1.0f, 0.0f,
  +0.5f, -0.5f,  0.5f,  1.0f, 0.0f,         0.0f, -1.0f, 0.0f,
  +0.5f, -0.5f,  0.5f,  1.0f, 0.0f,         0.0f, -1.0f, 0.0f,
  -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,         0.0f, -1.0f, 0.0f,
  -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,         0.0f, -1.0f, 0.0f,

  //top
  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,         0.0f, 1.0f, 0.0f,
   0.5f,  0.5f, -0.5f,  1.0f, 1.0f,         0.0f, 1.0f, 0.0f,
   0.5f,  0.5f,  0.5f,  1.0f, 0.0f,         0.0f, 1.0f, 0.0f, 
   0.5f,  0.5f,  0.5f,  1.0f, 0.0f,         0.0f, 1.0f, 0.0f,
  -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,         0.0f, 1.0f, 0.0f,
  -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,         0.0f, 1.0f, 0.0f
};

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
  if (firstMouse)
    {
      lastX = xpos;
      lastY = ypos;
      firstMouse = false;
    }

  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;

  lastX = xpos;
  lastY = ypos;

  camera.processMouseMovement(xoffset, yoffset);
}

void setupCube()
{
  //gen and bind vao and vbo
  glGenVertexArrays(1, &cubeVAO);
  glGenBuffers(1, &cubeVBO);

  // bind vao first
  glBindVertexArray(cubeVAO);

  //bind and fill vbo
  glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

  //setup vertex attrib
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
  glEnableVertexAttribArray(0);

  // texture coord loc=1
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
  glEnableVertexAttribArray(1);
  
  //normal attrib for light
  glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(5 * sizeof(GLfloat)));
  glEnableVertexAttribArray(2);
  
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
}

void display()
{
  //clear screen
  static const float darkGray[] = {0.05f, 0.05f, 0.1f, 1.0f};
  glClearColor(darkGray[0], darkGray[1], darkGray[2], darkGray[3]);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  //use shader program
  glUseProgram(shaderProgram);

  //create view matrix
  glm::mat4 view = camera.getViewMatrix();

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

  //light
  GLint lightPosLocation = glGetUniformLocation(shaderProgram, "u_lightPos");
  GLint lightColorLocation = glGetUniformLocation(shaderProgram, "u_lightColor");
  GLint viewPosLocation = glGetUniformLocation(shaderProgram, "u_viewPos");

  
  //send view and projection matrices
  glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
  glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

  static float lightTime = 0.0f;
  lightTime += deltaTime;

  glm::vec3 lightPos = camera.getPosition() + glm::vec3(0.0f, 2.0f, 0.0f);
  glm::vec3 lightColor = glm::vec3(1.0f, 0.95f, 0.8f);

  glUniform3fv(lightPosLocation, 1, glm::value_ptr(lightPos));
  glUniform3fv(lightColorLocation, 1, glm::value_ptr(lightColor));
  glUniform3fv(viewPosLocation, 1, glm::value_ptr(camera.getPosition()));

  glBindVertexArray(cubeVAO);

  static vector<RenderBatch> worldBatches;
  static bool worldBuilt = false;

  if (!worldBuilt)
    {
      world.buildGeometry(worldBatches);
      worldBuilt = true;
    }
  
  //render batches
  for (const auto& batch : worldBatches)
    {
      if (batch.textureIndex < textures.size() && !batch.transforms.empty())
	{     
	  //bind texture
	  glActiveTexture(GL_TEXTURE0);
	  glBindTexture(GL_TEXTURE_2D, textures[batch.textureIndex]);
	  glUniform1i(textureLocation, 0);
	}
      for (const auto& transform : batch.transforms)
	{
	  glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(transform));
	  glDrawArrays(GL_TRIANGLES, 0, NumVertices);
	}		
    }
  
  glBindVertexArray(0);
}

bool checkCollision(glm::vec3 newPos)
{
  return world.isPositionSolid(newPos);
}

void processInput(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  
  glm::vec3 oldPos = camera.getPosition();
  
  //wasd movement
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
      //no flying
      camera.processKeyboard(FORWARD, deltaTime);
      glm::vec3 newPos = camera.getPosition();
      if (checkCollision(newPos))
	camera.setPosition(oldPos);
    }
  
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
      camera.processKeyboard(BACKWARD, deltaTime);
      glm::vec3 newPos = camera.getPosition();
      if (checkCollision(newPos))
	{
	  camera.setPosition(oldPos);
	}
    }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
      camera.processKeyboard(LEFT, deltaTime);
      glm::vec3 newPos = camera.getPosition();
      if (checkCollision(newPos))
	{
	  camera.setPosition(oldPos);
	}
    }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
      camera.processKeyboard(RIGHT, deltaTime);
      glm::vec3 newPos = camera.getPosition();
      if (checkCollision(newPos))
	{
	  camera.setPosition(oldPos);
	}
    }

  //level loading controls
  static bool levelKeyPressed = false;

  //reload
  if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && !levelKeyPressed)
    {
      cout << "/nREloading level..." << endl;
      static vector<RenderBatch> worldBatches;
      static bool worldBuilt = false;

      if (world.loadLevel("../levels/level.txt"))
	{
	  const Level& currentLevel = world.getCurrentLevel();
	  camera.setPosition(currentLevel.spawnPoint);
	  camera.yaw = currentLevel.spawnYaw;
	  camera.pitch = 0.0f;
	  //update camera
	  glm::vec3 front;
	  front.x = cos(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
	  front.y = sin(glm::radians(camera.pitch));
	  front.z = sin(glm::radians(camera.yaw)) * cos(glm::radians(camera.pitch));
	  camera.front = glm::normalize(front);
	  camera.right = glm::normalize(glm::cross(camera.front, camera.worldUp));
	  camera.up = glm::normalize(glm::cross(camera.right, camera.front));
	}
      levelKeyPressed = true;
    }

  if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS && !levelKeyPressed)
    {
      world.printCurrentLevel();
      levelKeyPressed = true;
    }

  if (glfwGetKey(window, GLFW_KEY_M) == GLFW_RELEASE &&
      glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE)
    {
      levelKeyPressed = false;
    }

  if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
    {
      glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
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

  glEnable(GL_DEPTH_TEST);

  glfwSetCursorPosCallback(window, mouse_callback);
  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //hide cursor
  
  setupCube();
  setupShaders();
  setupTextures();

  //spawn point
  const Level& currentLevel = world.getCurrentLevel();
  camera.setPosition(currentLevel.spawnPoint);
  camera.yaw = currentLevel.spawnYaw;
  camera.pitch = 0.0f;

  world.printCurrentLevel();
  
  while (!glfwWindowShouldClose(window))
    {
      float currentFrame = glfwGetTime();
      deltaTime = currentFrame - lastFrame;
      lastFrame = currentFrame;
      
      processInput(window);
      display();
      
      glfwSwapBuffers(window);
      glfwPollEvents();
    }

  cleanup();
  glfwTerminate();
  return 0;
}
