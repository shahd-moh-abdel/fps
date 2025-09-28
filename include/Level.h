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
};

struct Level {
  int width, hight;
  std::vector<std::vector<CellType>> grid;
  std::string name;
};

bool loadLevel();
void createDefaultLevel(Level &level);

#endif 
