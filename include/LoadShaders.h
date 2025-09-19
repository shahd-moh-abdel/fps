#ifndef LOAD_SHADERS_H
#define LOAD_SHADERS_H 
#include <string>

using namespace std;
struct shaderProgram {
  string vertexShader;
  string fragmentShader;
};

shaderProgram parseShader(const string &filePath);
unsigned int compileShader(unsigned int type, const string &source);
unsigned int createShader(const string &vertexShader,
                          const string &fragmentShader);
#endif
