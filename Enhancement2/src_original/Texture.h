#pragma once

#include <glad/glad.h>

class Texture
{
public:
	Texture();
	~Texture();
	void load(const char* file);
	int getWidth();
	int getHeight();

	void use(const char* uniformName);
	void unuse();
private:
	int width, height;
	GLuint id;
};

