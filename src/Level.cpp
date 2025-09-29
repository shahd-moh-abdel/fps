#include "../include/Level.h"
#include <fstream>
#include <iostream>
#include <algorithm>

const float CELL_SIZE = 2.0f;
const float WALL_HEIGHT = 3.0f;

/*
CellType charToCell(char c) {
  //convert letters to cells in the world here
}

char cellToChar(CellType cell) {
  //convert world cells to letters to print for debugging later
}

*/


void printLevel(std::vector<std::string> levelLines)
{
  std::cout << "Lavel Map"  << std::endl;
  for (int  i = 0; i < levelLines.size(); i++)
    {
      std::cout << levelLines[i] << std::endl;
    }
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

  printLevel(levelLines);
  
  return true;
}

int main()
{
  Level level;
  loadLevel("../levels/level.txt", level);
}
