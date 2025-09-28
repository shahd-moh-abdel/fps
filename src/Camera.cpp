#include "../include/Camera.h"
#include <algorithm>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) :
  front(glm::vec3(0.0f, 0.0f, -1.0f)),
  movementSpeed(10.0f),
  mouseSensitivity(0.1f)				   
{
  this->position = position;
  this->worldUp = up;
  this->yaw = yaw;
  this->pitch = pitch;
  updateCameraVectors();
}

glm::mat4 Camera::getViewMatrix()
{
  return glm::lookAt(position, position + front, up);
}

void Camera::processKeyboard(int direction, float deltaTime)
{
  float velocity = movementSpeed * deltaTime;

  if (direction == FORWARD)
    {
      //no flying
      glm::vec3 forward =
	glm::normalize(glm::vec3(front.x, 0.0f, front.z));
      position += forward * velocity;
      
    }

  if (direction == BACKWARD)
    {
      glm::vec3 forward =
	glm::normalize(glm::vec3(front.x, 0.0f, front.z));
      position -= forward * velocity;
    }

  if (direction == LEFT)
    {
      glm::vec3 right = glm::normalize(glm::cross(front, up));
      glm::vec3 left =
	glm::normalize(glm::vec3(-right.x, 0.0f, -right.z));
      position += left * velocity;
    }

  if (direction == RIGHT)
    {
      glm::vec3 right = glm::normalize(glm::cross(front, up));
      glm::vec3 rightMove =
	glm::normalize(glm::vec3(right.x, 0.0f, right.z));

      position += rightMove * velocity;
    }
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
  xoffset *= mouseSensitivity;
  yoffset *= mouseSensitivity;

  yaw += xoffset;
  pitch += yoffset;

  if (constrainPitch)
    {
      if (pitch > 89.0f)
	pitch = 89.0f;
      if (pitch < -89.0f)
	pitch = -89.0f;
    }

  updateCameraVectors();
}

void Camera::updateCameraVectors()
{
  glm::vec3 front;
  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

  this->front = glm::normalize(front);
  this->right = glm::normalize(glm::cross(this->front, worldUp));
  this->up = glm::normalize(glm::cross(right, this->front));
}

