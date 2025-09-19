#include "../include/LoadShaders.h"
#include "../include/glad/glad.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

GLuint g_VAO = 0;
GLuint g_VBO = 0;

shaderProgram parseShader(const string &filePath)
{
  ifstream stream(filePath);

  string line;
  stringstream ss[2];
  int type = -1; // -1 == NONE

  while (getline(stream, line))
    {
      if (line.find("#shader") != string::npos)
	{
	  if (line.find("vertex") != string::npos)
	    type = 0; // 0 == VERTEX
	  if (line.find("fragment") != string::npos)
	    type = 1; // 1 == FRAGMENT
	}
      else
	ss[(int)type] << line << '\n';

      cout << line << endl;
    }

  return {ss[0].str(), ss[1].str()};
}

// compile shader
unsigned int compileShader(unsigned int type, const string& source)
{
  GLuint id = glCreateShader(type);
  const char* src = source.c_str();
  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);
  if (result == GL_FALSE)
    {
      int length;
      glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
      char* message = (char*)alloca(length * sizeof(char));
      glGetShaderInfoLog(id, length, &length, message);
      cout << "ERROR: error in" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " Shader: " << message << endl;
      glDeleteShader(id);
    }
  
  return id;
}

// create shaders
unsigned int createShader(const string& vertexShader, const string& fragmentShader)
{
  GLuint program = glCreateProgram();
  GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShader);
  GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

  glAttachShader(program, vs);
  glAttachShader(program, fs);
  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vs);
  glDeleteShader(fs);

  return program;
}
