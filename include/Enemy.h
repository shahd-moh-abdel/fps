#ifndef ENEMY_H
#define ENEMY_H

#include "GLFW/glfw3.h"
#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include <vector>

struct Enemy {
  glm::vec3 position;
  float health;
  float maxHealth;
  bool alive;
  float size;

  Enemy(glm::vec3 pos, float hp = 100.0f)
    : position(pos), health(hp), maxHealth(hp), alive(true), size(1.0f)
  {}

  void takeDamage(float damage)
  {
    health -= damage;
    if (health <= 0)
      {
	health = 0;
	alive = false;
      }
  }
};

class EnemyManager {
 public:
  EnemyManager();

  void addEnemy(glm::vec3 position);
  void update(float deltaTime);
  void render(GLuint shaderProgram, GLuint VAO, GLuint texture, const glm::mat4& view, const glm::mat4& projection, const glm::vec3& cameraPos);

  bool rayCastShoot(glm::vec3 origin, glm::vec3 direction, float maxDistance);

  const std::vector<Enemy>& getEnemies() const { return enemies; }
  int getAliveCount() const;
  int getKillCount() const { return killCount; }
  
 private:
  std::vector<Enemy> enemies;
  int killCount;

  glm::mat4 createBillboardMatrix(glm::vec3 position, glm::vec3 cameraPos, float size);
};

#endif
