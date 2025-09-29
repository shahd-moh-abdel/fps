#include "../include/World.h"
#include "../include/Level.h"
#include <iostream>
#include <cmath>

World::World()
{
  //init world data
  if(!loadLevel("../levels/level1.txt"))
    createDefaultLevel();
}

bool World::loadLevel(const std::string& filename)
{
  if (::loadLevel(filename, currentLevel))
    {
      levelLoaded = true;
      return true;
    }
  return false;
}

void World::setLevel(const Level& level)
{
  currentLevel = level;
  levelLoaded = true;
}

void World::createDefaultLevel() {
  ::createDefaultLevel(currentLevel);
  levelLoaded = true;
}

glm::vec3 World::gridToWorldPos(int x, int z, float y) const
{
  return glm::vec3(x * CELL_SIZE, y, z * CELL_SIZE);
}

bool World::isSolid(int x, int z) const
{
  if (x < 0 || x >= currentLevel.width || z < 0 || z >= currentLevel.height)
    return true;

  CellType cell = currentLevel.grid[z][x];
  return cell == WALL || cell == PILLAR;
}

CellType World::getCellType(int x, int z) const
{
  if (!levelLoaded) return WALL;
  if (x < 0 || x >= currentLevel.width || z < 0 || z >= currentLevel.height)
    return WALL;

  return currentLevel.grid[z][x];
}

bool World::isPositionSolid(glm::vec3 position) const
{
  int gridX = (int)floor((position.x + CELL_SIZE/2.0f) / CELL_SIZE);
  int gridZ = (int)floor((position.z + CELL_SIZE/2.0f) / CELL_SIZE);
  return isSolid(gridX, gridZ);
}

void World::buildGeometry(std::vector<RenderBatch>& batches)
{
  batches.clear();
  batches.resize(3);

  batches[0] = {"Walls", {}, 0};
  batches[1] = {"Floors", {}, 1};
  batches[2] = {"Ceilings", {}, 2};

  int wallCount = 0, floorCount = 0, ceilingCount = 0;

  for (int x = 0; x < currentLevel.width; x++)
    {
      for (int z = 0; z < currentLevel.height; z++)
	{
	  CellType cellType = getCellType(x, z);
	  glm::vec3 pos = gridToWorldPos(x, z);
	  
	  if (cellType == WALL) {
	    glm::mat4 wallTransform = glm::mat4(1.0f);
	    wallTransform = glm::translate(wallTransform,
					   pos + glm::vec3(0.f, WALL_HEIGHT/2.0f, 0.0f));
	    wallTransform = glm::scale(wallTransform,
				       glm::vec3(CELL_SIZE, WALL_HEIGHT * 2.0f, CELL_SIZE));
	    batches[0].transforms.push_back(wallTransform);
	    wallCount++;
	  }
	  else if (cellType == PILLAR) {
	    glm::mat4 pillarTransform = glm::mat4(1.0f);
	    pillarTransform = glm::translate(pillarTransform,
					     pos + glm::vec3(0.0f, WALL_HEIGHT/2.0f, 0.0f));
	    pillarTransform = glm::scale(pillarTransform,
					 glm::vec3(CELL_SIZE * 0.6f,
						   WALL_HEIGHT * 2.0f,
						   CELL_SIZE * 0.6f));
	    batches[0].transforms.push_back(pillarTransform);
	    wallCount++;
	  }

	  if (cellType != WALL)
	    {
	      glm::mat4 floorTransform = glm::mat4(1.0f);
	      floorTransform = glm::translate(floorTransform,
					      pos + glm::vec3(0.0f, -WALL_HEIGHT/2.0f, 0.0f));
	      floorTransform = glm::scale(floorTransform,
					  glm::vec3(CELL_SIZE, 0.2f, CELL_SIZE));
	      batches[1].transforms.push_back(floorTransform);
	      floorCount++;

	      glm::mat4 ceilingTransform = glm::mat4(1.0f);
	      ceilingTransform = glm::translate(ceilingTransform,
						pos + glm::vec3(0.0f,
								WALL_HEIGHT + WALL_HEIGHT/2.0f ,
								0.0f));
	      ceilingTransform = glm::scale(ceilingTransform,
						glm::vec3(CELL_SIZE, 0.2f, CELL_SIZE));
	      batches[2].transforms.push_back(ceilingTransform);
	      ceilingCount++;
	    }
	}
    }
}

void World::printCurrentLevel() const
{
  if (levelLoaded)
    {
      printLevel(currentLevel);
    }
  else
    std::cout << "no level" << std::endl;
}
