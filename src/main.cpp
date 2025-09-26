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
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

//cam rotation
float yaw = -90.0f;
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

struct CubeInstance {
  glm::vec3 position;
  glm::vec3 scale;
  glm::vec3 rotation;
  int textureIndex;
};

// define quads
vector<CubeInstance> cubeInstances = {
  //position,              scale,               rotation,           textureIndex
  {{-2.0f,  -2.0f, -2.0f}, {2.0f, 0.1f, 2.0f},  {0.0f, 0.0f, 0.0f},  1},
  {{ 0.0f,  -2.0f, -2.0f}, {2.0f, 0.1f, 2.0f},  {0.0f, 0.0f, 0.0f},  1},
  {{ 2.0f,  -2.0f, -2.0f}, {2.0f, 0.1f, 2.0f},  {0.0f, 0.0f, 0.0f},  1},
  {{-2.0f,  -2.0f,  0.0f}, {2.0f, 0.1f, 2.0f},  {0.0f, 0.0f, 0.0f},  1},
  {{ 0.0f,  -2.0f,  0.0f}, {2.0f, 0.1f, 2.0f},  {0.0f, 0.0f, 0.0f},  1},
  {{ 2.0f,  -2.0f,  0.0f}, {2.0f, 0.1f, 2.0f},  {0.0f, 0.0f, 0.0f},  1},
  {{-2.0f,  -2.0f,  2.0f}, {2.0f, 0.1f, 2.0f},  {0.0f, 0.0f, 0.0f},  1},
  {{ 0.0f,  -2.0f,  2.0f}, {2.0f, 0.1f, 2.0f},  {0.0f, 0.0f, 0.0f},  1},
  {{ 2.0f,  -2.0f,  2.0f}, {2.0f, 0.1f, 2.0f},  {0.0f, 0.0f, 0.0f},  1},


  //walls
  {{-3.0f,   0.0f,  0.0f}, {0.1f, 6.0f, 4.0f},  {glm::radians(90.0f), 0.0f, 0.0f},  0}, 
  {{ 3.0f,   0.0f,  0.0f}, {0.1f, 6.0f, 4.0f},  {glm::radians(90.0f), 0.0f, 0.0f},  0},
  {{ 0.0f,   0.0f, -3.0f}, {6.0f, 4.0f, 0.1f},  {0.0f , 0.0f, 0.0f},  0},
  {{ 0.0f,   0.0f,  3.0f}, {6.0f, 4.0f, 0.1f},  {0.0f , 0.0f, 0.0f},  0},

  {{0.0f,    0.0f,  0.0f}, {1.0f, 1.0f, 1.0f},  {0.0f, 0.0f, 0.0f}, 2},
  
  //ceiling
  //{{ 0.0f,   2.0f,  0.0f}, {6.0f, 0.1f, 6.0f},  {0.0f , 0.0f, 0.0f},  2},
  {{-2.0f,   2.0f, -2.0f}, {2.0f, 0.1f, 2.0f},  {0.0f, 0.0f, 0.0f},  2},
  {{ 0.0f,   2.0f, -2.0f}, {2.0f, 0.1f, 2.0f},  {0.0f, 0.0f, 0.0f},  2},
  {{ 2.0f,   2.0f, -2.0f}, {2.0f, 0.1f, 2.0f},  {0.0f, 0.0f, 0.0f},  2},
  {{-2.0f,   2.0f,  0.0f}, {2.0f, 0.1f, 2.0f},  {0.0f, 0.0f, 0.0f},  2},
  {{ 0.0f,   2.0f,  0.0f}, {2.0f, 0.1f, 2.0f},  {0.0f, 0.0f, 0.0f},  2},
  {{ 2.0f,   2.0f,  0.0f}, {2.0f, 0.1f, 2.0f},  {0.0f, 0.0f, 0.0f},  2},
  {{-2.0f,   2.0f,  2.0f}, {2.0f, 0.1f, 2.0f},  {0.0f, 0.0f, 0.0f},  2},
  {{ 0.0f,   2.0f,  2.0f}, {2.0f, 0.1f, 2.0f},  {0.0f, 0.0f, 0.0f},  2},
  {{ 2.0f,   2.0f,  2.0f}, {2.0f, 0.1f, 2.0f},  {0.0f, 0.0f, 0.0f},  2},

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
  glm::vec3 lightPos = glm::vec3(
				 sin(lightTime * 0.5f) * 2.0f,
				 1.0f + sin(lightTime * 0.7f) * 1.0f,
				 cos(lightTime * 0.5f) * 2.0f
				 );

  glm::vec3 lightColor = glm::vec3(1.0f, 0.95f, 0.8f);

  glUniform3fv(lightPosLocation, 1, glm::value_ptr(lightPos));
  glUniform3fv(lightColorLocation, 1, glm::value_ptr(lightColor));
  glUniform3fv(viewPosLocation, 1, glm::value_ptr(cameraPos));

  glBindVertexArray(cubeVAO);

  //render cubes
  for (size_t i = 0; i < cubeInstances.size(); i++)
    {
      auto& cube = cubeInstances[i];

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
    glfwSetWindowShouldClose(window, true);

  float cameraSpeed = 1.5f * deltaTime;

  //wasd movement
  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    cameraPos += cameraSpeed * cameraFront;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    cameraPos -= cameraSpeed * cameraFront;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

  //q/e up and down
  if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    cameraPos -= cameraUp * cameraSpeed;
  if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    cameraPos += cameraUp * cameraSpeed;
  
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
