#ifndef MAT4_H
#define MAT4_H

#include <cmath>

struct Mat4 {
  float m[16];

  Mat4() {
    // init as identity mat
    for (int i = 0; i < 16; i++) m[i] = 0.0f;
    m[0] = m[5] = m[10] = m[15] = 1.0f;
  }

  // translation matrix
  static Mat4 translate(float x, float y, float z) {
    Mat4 result;
    result.m[12] = x;
    result.m[13] = y;
    result.m[14] = z;
    return result;
  }

  static Mat4 scale(float x, float y, float z) {
    Mat4 result;
    result.m[0] = x;
    result.m[5] = y;
    result.m[10] = z;
    return result;
  }

  //rotate around z
  static Mat4 rotateZ(float angleRadians) {
    Mat4 result;
    float c = cos(angleRadians);
    float s = sin(angleRadians);
    result.m[0] = c;
    result.m[1] = s;
    result.m[4] = -s;
    result.m[5] = c;
    return result;
  }

  // matrix multiplication
  Mat4 operator*(const Mat4& other) const {
    Mat4 result;
    for (int i = 0; i < 4; i++)
      {
	for (int j = 0; j < 4; j++)
	  {
	    result.m[i*4 + j] = 0;
	    for (int k = 0; k < 4; k++)
	      result.m[i*4 + j] += m[i*4 + k] * other.m[k*4 + j];
	  }
      }
    return result;
  }
};

#endif
