#ifndef SHADER_HPP_
#define SHADER_HPP_

#include "includeGlew.hpp"
#include <GL/gl.h>
#include <fstream>
#include <iostream>
#include <vector>

GLint loadShaderFromFile(const GLenum type, const char* nazwaPliku);
void LinkValidateProgram(GLint program);
void attachShaders(GLint program, std::vector<GLint> shaders);

#endif /* SHADER_HPP_ */
