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

using namespace std;

#define SCREEN_WIDTH 700
#define SCREEN_HEIGHT 700

//this is for resizing later
int g_width = SCREEN_WIDTH;
int g_height = SCREEN_HEIGHT;

//camera
glm::vec3 cameraPos = glm::vec3(3.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

//cam rotation
float yaw =  90.0f;
float pitch = 0.0f;

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

//world grid consts

const int WORLD_SIZE = 24;
const float CELL_SIZE = 3.0f;
const float WALL_HEIGHT = 6.0f;

enum CellType { EMPTY = 0, WALL = 1, PILLAR = 2 };

int world[WORLD_SIZE][WORLD_SIZE] = {
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
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

struct RenderBatch {
  string name;
  vector<glm::mat4> transforms;
  int textureIndex;
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

  float sensitivity = 0.1f;
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw += xoffset;
  pitch += yoffset;

  //constrain pitch
  if (pitch > 89.0f)
    pitch = 89.0f;
  if (pitch < -89.0)
    pitch = -89.0f;

  glm::vec3 front;
  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

  cameraFront = glm::normalize(front);
}

glm::vec3 gridToWorldPos(int x, int z, float y = 0.0f)
{
  return glm::vec3(x * CELL_SIZE, y, z * CELL_SIZE);
}

bool isSolid(int x, int z)
{
  if (x < 0 || x >= WORLD_SIZE || z < 0 || z >= WORLD_SIZE)
    return true;

  return world[z][x] != EMPTY;
}

CellType getCellType(int x, int z)
{
  if (x < 0 || x >= WORLD_SIZE || z < 0 || z >= WORLD_SIZE)
    return WALL;

  return (CellType)world[z][x];
}

void buildWorldGeometry(vector<RenderBatch>& batches)
{
  batches.clear();
  batches.resize(3);

  batches[0] = {"Walls", {}, 0};
  batches[1] = {"Floors", {}, 1};
  batches[2] = {"Ceilings", {}, 2};

  int wallCount = 0, floorCount = 0, ceilingCount = 0;

  for (int x = 0; x < WORLD_SIZE; x++)
    {
      for (int z = 0; z < WORLD_SIZE; z++)
	{
	  CellType cellType  = getCellType(x, z);
	  glm::vec3 pos = gridToWorldPos(x, z);

	  if (cellType == WALL)
	    {
	      glm::mat4 wallTransform = glm::mat4(1.0f);
	      
	      wallTransform = glm::translate(wallTransform,
					   pos + glm::vec3(0.0f, WALL_HEIGHT/2.0f ,0.0f));
	      wallTransform = glm::scale(wallTransform,
					   glm::vec3(CELL_SIZE, WALL_HEIGHT * 2.0f, CELL_SIZE));
	      
	      batches[0].transforms.push_back(wallTransform);
	      wallCount++;
	    }
	  else if (cellType == PILLAR)
	    {
	      glm::mat4 pillarTransform = glm::mat4(1.0f);
	      pillarTransform = glm::translate(pillarTransform,
					       pos + glm::vec3(0.0f, WALL_HEIGHT/2.0f, 0.0f));
	      pillarTransform = glm::scale(pillarTransform,
					       glm::vec3(CELL_SIZE * 0.6f, WALL_HEIGHT * 2.0f, CELL_SIZE * 0.6f));

	      batches[0].transforms.push_back(pillarTransform);
	      wallCount++;                        		       
	    }

	  if (cellType != WALL)
	    {
	      glm::mat4 floorTransform = glm::mat4(1.0f);
	      floorTransform = glm::translate(floorTransform,
					      pos + glm::vec3(0.0f, -WALL_HEIGHT/2.0f, 0.0f));
	      floorTransform = glm::scale(floorTransform,
					  glm::vec3(CELL_SIZE, 0.2f, CELL_SIZE));

	      batches[1].transforms.push_back(floorTransform);
	      floorCount++;

	      glm::mat4 ceilingTransform = glm::mat4(1.0f);
	      ceilingTransform = glm::translate(ceilingTransform,
						pos + glm::vec3(0.0f, WALL_HEIGHT + WALL_HEIGHT/2.0f, 0.0));
	      ceilingTransform = glm::scale(ceilingTransform,
					    glm::vec3(CELL_SIZE, 0.2f, CELL_SIZE));
	      batches[2].transforms.push_back(ceilingTransform);
	      ceilingCount++;		       		  
	    }
	}
    }

  cout << "world built" << endl;
  
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
  cout << "Loaded " << textures.size() << " textures" << endl;
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
  glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

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

  //moving light source
  glm::vec3 lightPos = cameraPos + glm::vec3(0.0f, 2.0f, 0.0f);
  glm::vec3 lightColor = glm::vec3(1.0f, 0.95f, 0.8f);

  glUniform3fv(lightPosLocation, 1, glm::value_ptr(lightPos));
  glUniform3fv(lightColorLocation, 1, glm::value_ptr(lightColor));
  glUniform3fv(viewPosLocation, 1, glm::value_ptr(cameraPos));

  glBindVertexArray(cubeVAO);

  static vector<RenderBatch> worldBatches;
  static bool worldBuilt = false;

  if (!worldBuilt)
    {
      buildWorldGeometry(worldBatches);
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
  int gridX = (int)floor((newPos.x + CELL_SIZE/2.0f) / CELL_SIZE);
  int gridY = (int)floor((newPos.z + CELL_SIZE/2.0f) / CELL_SIZE);

  return isSolid(gridX, gridY);
}

void processInput(GLFWwindow* window)
{
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  float cameraSpeed =  10.0f * deltaTime;
  glm::vec3 newPos = cameraPos;
  
  //wasd movement
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
      //no flying
      glm::vec3 forward = glm::normalize(glm::vec3(cameraFront.x, 0.0f, cameraFront.z));
      
      newPos = cameraPos + cameraSpeed  * forward;
      if (!checkCollision(newPos))
	{
	  cameraPos.x = newPos.x;
	  cameraPos.z = newPos.z;
	}
    }
  
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
      glm::vec3 forward = glm::normalize(glm::vec3(cameraFront.x, 0.0f, cameraFront.z));
      
      newPos = cameraPos - cameraSpeed * forward;
      if (!checkCollision(newPos))
	{
	  cameraPos.x = newPos.x;
	  cameraPos.z = newPos.z;
	}
    }
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
      glm::vec3 right = glm::normalize(glm::cross(cameraFront, cameraUp));
      glm::vec3 left = glm::normalize(glm::vec3(-right.x, 0.0f, -right.z));
      newPos = cameraPos + cameraSpeed * left;
      if (!checkCollision(newPos))
	{
	  cameraPos.x = newPos.x;
	  cameraPos.z = newPos.z;
	}
    }
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
      glm::vec3 right = glm::normalize(glm::cross(cameraFront, cameraUp));
      glm::vec3 rightMove = glm::normalize(glm::vec3(right.x, 0.0f, right.z));
      newPos = cameraPos + cameraSpeed * rightMove;

      if (!checkCollision(newPos))
	{
	  cameraPos.x = newPos.x;
	  cameraPos.z = newPos.z;
	}
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
