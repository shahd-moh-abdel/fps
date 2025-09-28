#ifndef WORLD_H
#define WORLD_H

#include "glm/fwd.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>
#include <string>

using namespace std;
using namespace glm;

enum CellType {
  EMPTY = 0,
  WALL = 1,
  PILLAR = 2
};

struct RenderBatch {
  string name;
  vector<mat4> transforms;
  int textureIndex;
};

class World {
 public:
  World();

  void buildGeometry(vector<RenderBatch>& batches);

  bool isPositionSolid(vec3 position) const;

  vec3 gridToWorldPos(int x,
		      int z,
		      float y = 0.0f) const;
  
  static constexpr int getWorldSize()
  { return WORLD_SIZE; }

  static constexpr float getCellSize()
  { return CELL_SIZE; }

  static constexpr float getWallHeight()
  { return WALL_HEIGHT; }
    
 private:
  static const int WORLD_SIZE = 24;
  static constexpr float CELL_SIZE = 3.0f;
  static constexpr float WALL_HEIGHT = 6.0f;

  int worldData[WORLD_SIZE][WORLD_SIZE];

  bool isSolid(int x, int z) const;
  CellType getCellType(int x, int z) const;
};

#endif
