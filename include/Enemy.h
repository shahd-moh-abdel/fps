#ifndef ENEMY_H
#define ENEMY_H

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

  void addEnemy();
  void update();
  void render();

 private:
  std::vector<Enemy> enemies;
  int killCount;

  glm::mat4 createBillboardMatrix();
};

#endif
