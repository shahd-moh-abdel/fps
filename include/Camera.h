#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"


enum Camera_Movement {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT
};

class Camera {
 public:
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up;
  glm::vec3 right;
  glm::vec3 worldUp;

  float yaw;
  float pitch;

  float movementSpeed;
  float mouseSensitivity;
  
  Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
	 glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
	 float yaw = -90.0f,
	 float pitch = 0.0f);

  glm::mat4 getViewMatrix();

  //process keyboard
  void processKeyboard(int direction, float deltaTime);

  //process mouse
  void processMouseMovement(float xoffset,
			    float yoffset,
			    bool constrainPitch = true);
  //gets
  glm::vec3 getPosition() const { return position; }
  void  setPosition(const glm::vec3& pos)
  { position = pos;}

  
 private:
  void updateCameraVectors();
};

#endif
