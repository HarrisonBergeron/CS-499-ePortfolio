#include "Model.h"

Model::Model(vector<GLfloat>& verts, vector<GLushort>& indices)
{
	indexCount = indices.size();
	// Create buffers and vertex array
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vboIndices);
	glGenBuffers(1, &vboVertices);
	glBindVertexArray(vao);
	// Supply OpenGL with vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vboVertices);
	glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof GLfloat, verts.data(), GL_STATIC_DRAW);
	// Supply OpenGL with index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof GLushort, indices.data(), GL_STATIC_DRAW);

	// Vertex position has three float components
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 12, 0);
	glEnableVertexAttribArray(0);
	// Vertex color has four float components and is offset by position components
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 12, (void*)(sizeof(float) * 3));
	glEnableVertexAttribArray(1);
	// Vertex texture coordinates have four components and offset by position and color components
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 12, (void*)(sizeof(float) * 7));
	glEnableVertexAttribArray(2);
	// Vertex normals have three components and offset by position and color and texture components
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 12, (void*)(sizeof(float) * 9));
	glEnableVertexAttribArray(3);

	glBindVertexArray(0);
}
Model::~Model()
{
	// Delete OpenGL resources when done
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vboVertices);
	glDeleteBuffers(1, &vboIndices);
}

void Model::draw(glm::mat4 transform)
{
	// Get OpenGL shader currently in use so that the model matrix uniform can be supplied
	GLint shader = 0;
	glGetIntegerv(GL_CURRENT_PROGRAM, &shader);

	// Apply transform to model
	GLuint modelLoc = glGetUniformLocation(shader, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));
	// Use this model's VAO
	glBindVertexArray(vao);
	// Draw triangles with indices
	glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, nullptr);
	// Done using this VAO
	glBindVertexArray(0);
}

Model Model::createPyramid()
{
	vector<GLfloat> verts =
	{
		// Bottom
		-1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,

		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f,
		0.0f, -1.0f, 0.0f,

		-1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 1.0f,
		0.0f, -1.0f, 0.0f,

		// Left
		-1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f,
		-0.7071f, 0.7071f, 0.0f,

		-1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f,
		-0.7071f, 0.7071f, 0.0f,

		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 1.0f,
		-0.7071f, 0.7071f, 0.0f,

		// Front
		-1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f,
		0.0f, 0.7071f, 0.7071f,

		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.7071f, 0.7071f,

		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 1.0f,
		0.0f, 0.7071f, 0.7071f,

		// Right
		1.0f, -1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f,
		0.7071f, 0.7071f, 0.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f,
		0.7071f, 0.7071f, 0.0f,

		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 1.0f,
		0.7071f, 0.7071f, 0.0f,

		// Back
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.0f, 0.0f,
		0.0f, 0.7071f, -0.7071f,

		-1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 0.0f,
		0.0f, 0.7071f, -0.7071f,

		0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 1.0f,
		0.0f, 0.7071f, -0.7071f,
	};
	vector<GLushort> indices =
	{
		3, 2, 1,
		3, 1, 0,

		4, 5, 6,

		7, 8, 9,

		10, 11, 12,

		13, 14, 15
	};

	return Model(verts, indices);
}

Model Model::createSphere(int latitudeDivisions, int longitudeDivisions, float uTiling, float vTiling)
{
	vector<GLfloat> verts;
	float angleIncr = 2.0f * glm::pi<float>() / longitudeDivisions;
	float yAngleIncr = -glm::pi<float>() / latitudeDivisions;
	float yAngle = glm::pi<float>();
	float vInc = vTiling / latitudeDivisions;
	float v = 0.0f;
	for (int i = 0; i < latitudeDivisions + 1; i++) 
	{
		float radius = glm::sin(yAngle);
		float angle = 0.0f;
		float uInc = uTiling / longitudeDivisions;
		float u = 0.0f;
		// Create circle of vertices
		for (int j = 0; j < longitudeDivisions + 1; j++) 
		{
			float x = radius * glm::cos(angle);
			float y = glm::cos(yAngle);
			float z = radius * glm::sin(angle);
			addVertex(verts, x, y, z, 1.0f, 1.0f, 1.0f, 1.0f, u, v, x, y, z);
			angle += angleIncr;
			u += uInc;
		}
		yAngle += yAngleIncr;
		v += vInc;
	}

	vector<GLushort> indices;
	// Rows of quads that make up sphere
	for (int i = 0; i < latitudeDivisions; i++) 
	{
		for (int j = 0; j < longitudeDivisions + 1; j++)
		{
			int row1 = i * (longitudeDivisions + 1);
			int row2 = row1 + longitudeDivisions + 1;
			indices.push_back(row1 + j);
			indices.push_back(row1 + (j + 1) % (longitudeDivisions + 1));
			indices.push_back(row2 + (j + 1) % (longitudeDivisions + 1));
			indices.push_back(row1 + j);
			indices.push_back(row2 + (j + 1) % (longitudeDivisions + 1));
			indices.push_back(row2 + j);
		}
	}

	return Model(verts, indices);
}

// Modification of createSphere with equal radius for each section and equal section height
Model Model::createCylinder(int latitudeDivisions, int longitudeDivisions, float uTiling, float vTiling)
{
	vector<GLfloat> verts;
	addVertex(verts, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f);
	float angleIncr = 2.0f * glm::pi<float>() / longitudeDivisions;
	float yIncr = 2.0f / latitudeDivisions;
	float y = -1.0f;
	float vInc = vTiling / latitudeDivisions;
	float v = 0.0f;
	for (int i = 0; i < latitudeDivisions + 1; i++)
	{
		float radius = 1.0f;
		float angle = 0.0f;
		float uInc = uTiling / longitudeDivisions;
		float u = 0.0f;
		// Create circle of vertices
		for (int j = 0; j < longitudeDivisions + 1; j++)
		{
			float x = radius * glm::cos(angle);
			float z = radius * glm::sin(angle);
			addVertex(verts, x, y, z, 1.0f, 1.0f, 1.0f, 1.0f, u, v, x, 0.0f, z);
			angle += angleIncr;
			u += uInc;
		}
		y += yIncr;
		v += vInc;
	}
	addVertex(verts, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);

	vector<GLushort> indices;
	// Base of cylinder
	for (int i = 0; i < longitudeDivisions + 1; i++)
	{
		indices.push_back(0);
		indices.push_back(i + 1);
		indices.push_back((i + 1) % (longitudeDivisions + 1) + 1);
	}
	// Middle sections of cylinder
	for (int i = 0; i < latitudeDivisions; i++)
	{
		for (int j = 0; j < (longitudeDivisions + 1); j++)
		{
			int row1 = 1 + i * (longitudeDivisions + 1);
			int row2 = row1 + longitudeDivisions + 1;
			indices.push_back(row1 + j);
			indices.push_back(row1 + (j + 1) % (longitudeDivisions + 1));
			indices.push_back(row2 + (j + 1) % (longitudeDivisions + 1));
			indices.push_back(row1 + j);
			indices.push_back(row2 + (j + 1) % (longitudeDivisions + 1));
			indices.push_back(row2 + j);
		}
	}
	// Top of cylinder
	int top = 1 + (longitudeDivisions + 1) * (latitudeDivisions + 1);
	for (int i = 0; i < (longitudeDivisions + 1); i++)
	{
		indices.push_back(top - i - 1);
		indices.push_back(top - (i + 1) % (longitudeDivisions + 1) - 1);
		indices.push_back(top);
	}

	return Model(verts, indices);
}

Model Model::createCube()
{
	vector<GLfloat> verts;
	vector<GLushort> indices;

	// Front
	addVertex(verts, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	addVertex(verts, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	addVertex(verts, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f);
	addVertex(verts, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);

	// Back
	addVertex(verts, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f);
	addVertex(verts, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f);
	addVertex(verts, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f);
	addVertex(verts, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f);

	// Left
	addVertex(verts, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	addVertex(verts, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f);
	addVertex(verts, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f);
	addVertex(verts, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f);

	// Right
	addVertex(verts, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	addVertex(verts, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
	addVertex(verts, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f);
	addVertex(verts, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f);

	// Bottom
	addVertex(verts, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f);
	addVertex(verts, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f);
	addVertex(verts, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f);
	addVertex(verts, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f);

	// Top
	addVertex(verts, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	addVertex(verts, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
	addVertex(verts, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f);
	addVertex(verts, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);

	for (int i = 0; i < 24; i += 4)
	{
		indices.push_back(0 + i);
		indices.push_back(1 + i);
		indices.push_back(2 + i);
		indices.push_back(0 + i);
		indices.push_back(2 + i);
		indices.push_back(3 + i);
	}

	return Model(verts, indices);
}

Model Model::createXZPlane(int divisionsX, int divisionsZ, float uTiling, float vTiling)
{
	vector<GLfloat> verts;
	float xf = -1.0f;
	float xIncr = 2.0f / divisionsX;
	float vInc = vTiling / divisionsX;
	float v = 0.0f;
	for (int x = 0; x < divisionsX + 1; x++)
	{
		float zf = -1.0f;
		float zIncr = 2.0f / divisionsZ;
		float uInc = uTiling / divisionsZ;
		float u = 0.0f;
		for (int z = 0; z < divisionsZ + 1; z++)
		{
			addVertex(verts, xf, 0.0f, zf, 1.0f, 1.0f, 1.0f, 1.0f, u, v, 0.0f, 1.0f, 0.0f);
			zf += zIncr;
			u += uInc;
		}
		xf += xIncr;
		v += vInc;
	}

	vector<GLushort> indices;
	for (int x = 0; x < divisionsX; x++)
	{
		int row1 = x * (divisionsZ + 1);
		int row2 = row1 + divisionsZ + 1;
		for (int z = 0; z < divisionsZ; z++)
		{
			indices.push_back(row1 + z);
			indices.push_back(row1 + z + 1);
			indices.push_back(row2 + z);
			indices.push_back(row1 + z + 1);
			indices.push_back(row2 + z + 1);
			indices.push_back(row2 + z);
		}
	}

	return Model(verts, indices);
}

void Model::addVertex(vector<GLfloat>& vertices, float x, float y, float z, float r, float g, float b, float a, float u, float v, float nx, float ny, float nz)
{
	vertices.push_back(x);
	vertices.push_back(y);
	vertices.push_back(z);
	vertices.push_back(r);
	vertices.push_back(g);
	vertices.push_back(b);
	vertices.push_back(a);
	vertices.push_back(u);
	vertices.push_back(v);
	vertices.push_back(nx);
	vertices.push_back(ny);
	vertices.push_back(nz);
}
