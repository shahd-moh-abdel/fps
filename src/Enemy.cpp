#include "../include/glad/glad.h"
#include "../include/glm/gtc/type_ptr.hpp"
#include "../include/Enemy.h"

#include <cmath>
#include <iostream>

EnemyManager::EnemyManager() : killCount(0) {}

void EnemyManager::addEnemy(glm::vec3 position)
{
  enemies.push_back(Enemy(position));
}

void EnemyManager::update(float deltaTime)
{
  for (auto& enemy : enemies)
    { 
      if (!enemy.alive) continue;
      //will add enemy movemnt later
    }
}

glm::mat4 EnemyManager::createBillboardMatrix(glm::vec3 position, glm::vec3 cameraPos, float size)
{

  glm::vec3 look = glm::normalize(cameraPos - position);
  glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0, 1, 0), look));

  glm::vec3 up = glm::vec3(0, 1, 0);

  glm::mat4 billboard = glm::mat4(1.0f);

  billboard[0] = glm::vec4(right * size, 0.0f);
  billboard[1] = glm::vec4(up * size, 0.0f);
  billboard[2] = glm::vec4(-look * size, 0.0f);
  billboard[3] = glm::vec4(position, 1.0f);

  return billboard;
}

void EnemyManager::render(GLuint shaderProgram, GLuint VAO, GLuint texture, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos)
{
  GLint modelLocation = glGetUniformLocation(shaderProgram, "u_model");
  GLint textureLocation = glGetUniformLocation(shaderProgram, "u_texture");

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, texture);
  glUniform1f(textureLocation, 0);

  glBindVertexArray(VAO);

  for (const auto& enemy : enemies)
    {
      if (!enemy.alive) return;

      glm::mat4 model = createBillboardMatrix(enemy.position, cameraPos, enemy.size);

      glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

      glDrawArrays(GL_TRIANGLES, 0, 6);
      
    }

  glBindVertexArray(0);
}

int EnemyManager::getAliveCount() const
{
  int count = 0;
  for (const auto& enemy : enemies)
    {
      if (enemy.alive) count++;
    }
  return count;
}
