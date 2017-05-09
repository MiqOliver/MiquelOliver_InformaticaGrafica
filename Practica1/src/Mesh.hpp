#pragma once

#define GLEW_STATIC
#include <GL\glew.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <glm.hpp>
#include "Shader.hpp"

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;	
};

struct Texture {
	GLuint id;
	std::string type;
};