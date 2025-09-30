#ifndef LEVEL_H
#define LEVEL_H

#include <vector>
#include <string>
#include "./glm/glm.hpp"

enum CellType {
  EMPTY = 0,
  WALL = 1,
  PILLAR = 2,
  SPAWN = 3,
  ENEMY = 4,
};

struct Level {
  int width, height;
  std::vector<std::vector<CellType>> grid;
  glm::vec3 spawnPoint;
  float spawnYaw;
  std::string name;
  std::vector<glm::vec3> enemySpawns;
};

CellType charToCell(char c);
char cellToChar(CellType cell);
bool loadLevel(const std::string& filename, Level& level);
void createDefaultLevel(Level& level);
void printLevel(const Level& level);

#endif 
