#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <random>

using namespace std;

class Model
{
public:
	Model(vector<GLfloat>& verts, vector<GLushort>& indices);
	~Model();

	void draw(glm::mat4 transform);

	static Model createPyramid();

	static Model createSphere(int latitudeDivisions, int longitudeDivisions, float uTiling, float vTiling);

	static Model createCylinder(int latitudeDivisions, int longitudeDivisions, float uTiling, float vTiling);

	static Model createCube();

	static Model createXZPlane(int divisionsX, int divisionsZ, float uTiling, float vTiling);
private:
	GLuint vao;
	GLuint vboVertices;
	GLuint vboIndices;
	GLsizei indexCount;

	static void addVertex(vector<GLfloat>& vertices, float x, float y, float z, float r, float g, float b, float a, float u, float v, float nx, float ny, float nz);
};