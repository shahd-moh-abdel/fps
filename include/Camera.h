#ifndef CAMERA_H
#define CAMERA_H

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

class Camera {
 public:
  Camera(glm::vec3 position = glm::vec3(3.0f, 0.0f, 3.0f),
	 glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
	 float yaw = 90.0f,
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
  glm::vec3 getFront() const { return front; }
  glm::vec3 getUp() const { return up; }
  glm::vec3 getRight() const { return right; }

  void  setPosition(const glm::vec3& pos)
  { position = pos;}

  enum Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
  };
  
 private:
  glm::vec3 position;
  glm::vec3 front;
  glm::vec3 up;
  glm::vec3 right;
  glm::vec3 worldUp;

  float yaw;
  float pitch;

  float movementSpeed;
  float mouseSensitivity;

  void updateCameraVectors();
};

#endif
