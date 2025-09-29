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
  case 'P': return PILLAR;
  case 'S': return SPAWN;
  default: return WALL;
  }
}


char cellToChar(CellType cell) {
  switch (cell)
    {
    case EMPTY: return '.';
    case WALL: return '#';
    case PILLAR: return 'P';
    default: return '#';
    }
}

void printLevel(const Level& level)
{
  std::cout << "Lavel Ma"  << std::endl;
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
      std::cout << "error could not open file" << filename << std::endl;
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
	      spawnFound = true;
	      level.grid[z][x] = EMPTY;
	    }
	  
	}
    }

  level.name = filename;

  printLevel(level);
 
  return true;
}

int main()
{
  Level level;
  loadLevel("../levels/level.txt", level);
}
