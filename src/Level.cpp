#include "../include/Level.h"
#include <fstream>
#include <iostream>
#include <algorithm>
#include <string>

const float CELL_SIZE = 2.0f;
const float WALL_HEIGHT = 3.0f;


CellType charToCell(char c) {
  switch (c) {
  case ' ': case '.': return EMPTY;
  case '#': return WALL;
  case 'P': case 'p': return PILLAR;
  case 'S': case 's': return SPAWN;
  case 'E': case 'e': return ENEMY;
  default: return WALL;
  }
}


char cellToChar(CellType cell) {
  switch (cell)
    {
    case EMPTY: return '.';
    case WALL: return '#';
    case PILLAR: return 'P';
    case SPAWN: return 'S';
    case ENEMY: return 'E';
    default: return '?';
    }
}

void printLevel(const Level& level)
{
  std::cout << "Lavel Map: "<< level.name << std::endl;
  std::cout << "enemies: " << level.enemySpawns.size() << std::endl;

  for (int z = 0; z < level.height; z++)
    {
      for (int x = 0; x < level.width; x++)
	{
	  std::cout << cellToChar(level.grid[z][x]);
	}
      std::cout << std::endl;
    }
  std::cout << std::endl;
}


bool loadLevel(const std::string& filename, Level& level)
{
  std::ifstream file(filename);
  if (!file.is_open())
    {
      std::cout << "error could not open file " << filename << std::endl;
      return false;
    }

  std::string line;
  std::vector<std::string> levelLines;

  while (getline(file, line))
    {
      if (!line.empty())
	levelLines.push_back(line);
    }
  file.close();

  //level dimensions
  level.height = levelLines.size();
  level.width = 0;

  for (const std::string& l: levelLines)
    {
      level.width = std::max(level.width, (int)l.length()); 
    }

  //int grid
  level.grid.resize(level.height);
  for (int z = 0; z < level.height; z++)
    {
      level.grid[z].resize(level.width);
    }

  bool spawnFound = false;
  level.spawnYaw = -90.0f;
  level.enemySpawns.clear();
  
  for (int z = 0; z < level.height; z++)
    {
      const std::string& currentLine = levelLines[z];
      for (int x = 0; x < level.width; x++)
	{
	  char c = (x < currentLine.length()) ? currentLine[x] : '#';
	  CellType cellType = charToCell(c);

	  level.grid[z][x] = cellType;

	  if (cellType == SPAWN)
	    {
	      level.spawnPoint = glm::vec3(x * CELL_SIZE, 0.0f, z * CELL_SIZE);
	      
	      spawnFound = true;
	      level.grid[z][x] = EMPTY;
	    }
	  else if (cellType == ENEMY)
	    {
	      glm::vec3 enemyPos = glm::vec3(x * CELL_SIZE, 0.0f, z * CELL_SIZE);
	      level.enemySpawns.push_back(enemyPos);
	      level.grid[z][x] = EMPTY;
	      std::cout << "enemy spawn found" << std::endl;
	    }
	  
	}
    }

  if (!spawnFound)
    {
      level.spawnPoint = glm::vec3(CELL_SIZE, 0.0f, CELL_SIZE);
    }
  
  level.name = filename;

  return true;
}

void createDefaultLevel(Level& level)
{
  level.width = 12;
  level.height = 12;
  level.name = "defualt level";
  level.spawnPoint = glm::vec3( CELL_SIZE, 0.0f, CELL_SIZE);
  level.spawnYaw = -90.0f;
  level.enemySpawns.clear();
  
  level.grid.resize(level.height);
  for (int z = 0; z < level.height; z++)
    {
      level.grid[z].resize(level.width);
    }

  std::string defaultMap[] = {
    "############",
    "#S.........#",
    "#..........#",
    "#..........#",
    "#......E...#",
    "#..........#",
    "#..........#",
    "#......E...#",
    "#..........#",
    "#..........#",
    "#..........#",
    "############",
  };

  for (int z = 0; z < level.height; z++)
    {
      for (int x = 0; x < level.width; x++)
	{
	  char c = defaultMap[z][x];
	  CellType cellType = charToCell(c);
	  level.grid[z][x] = cellType;

	  if (cellType == ENEMY)
	    {
	      level.enemySpawns.push_back(glm::vec3(x * CELL_SIZE, 0.0f, z * CELL_SIZE));
	      level.grid[z][x] = EMPTY;
	    }
	  else if (cellType == SPAWN)
	    {
	      level.spawnPoint = glm::vec3(x * CELL_SIZE, 0.0f, z * CELL_SIZE);
	      level.grid[z][x] = EMPTY;
	    }
	  
	}
    }
}
