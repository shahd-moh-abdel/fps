#ifndef WORLD_H
#define WORLD_H

#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "../include/Level.h"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>
#include <string>

using namespace std;
using namespace glm;

const float CELL_SIZE = 2.0f;
const float WALL_HEIGHT = 3.0f;

struct RenderBatch {
  string name;
  vector<mat4> transforms;
  int textureIndex;
};

class World {
 public:
  World();

  bool loadLevel(const std::string& filename);
  void setLevel(const Level& level);
  const Level& getCurrentLevel() const { return currentLevel; }
  
  void buildGeometry(vector<RenderBatch>& batches);

  bool isPositionSolid(vec3 position) const;
  vec3 gridToWorldPos(int x, int z, float y = 0.0f) const;
  bool isSolid(int x, int z) const;
  CellType getCellType(int x, int z) const;

  void printCurrentLevel() const;
    
 private:
  Level currentLevel;
  bool levelLoaded;

};

#endif
