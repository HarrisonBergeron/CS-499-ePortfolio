/* Model.cpp
 * Author: Harrison Bergeron
 * Date: July 23, 2022
 * Course: CS-499-H6772 Computer Science Capstone 22EW6 */

#include "Model.h"

Model::Model(std::vector<GLfloat>& verts, std::vector<GLushort>& indices)
{
    index_count_ = indices.size();
    // Create buffers and vertex array
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_indices_);
    glGenBuffers(1, &vbo_vertices_);
    glBindVertexArray(vao_);
    // Supply OpenGL with vertex data
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices_);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(GLfloat), verts.data(),
        GL_STATIC_DRAW);
    // Supply OpenGL with index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_indices_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLushort), 
        indices.data(), GL_STATIC_DRAW);

    // Vertex position has three float components
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 12, 0);
    glEnableVertexAttribArray(0);
    /* Vertex color has four float components and is offset by position 
     * components */
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 12, 
        (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(1);
    /* Vertex texture coordinates have four components and offset by position 
     * and color components */
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 12, 
        (void*)(sizeof(float) * 7));
    glEnableVertexAttribArray(2);
    /* Vertex normals have three components and offset by position and color 
     * and texture components */
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 12, 
        (void*)(sizeof(float) * 9));
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);
}
Model::~Model()
{
    // Delete OpenGL resources when done
    glDeleteVertexArrays(1, &vao_);
    glDeleteBuffers(1, &vbo_vertices_);
    glDeleteBuffers(1, &vbo_indices_);
}

void Model::Draw(glm::mat4 transform)
{
    /* Get OpenGL shader currently in use so that the model matrix uniform can
     * be supplied. */
    GLint shader = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &shader);

    // Apply transform to model
    GLuint modelLoc = glGetUniformLocation(shader, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(transform));
    // Use this model's VAO
    glBindVertexArray(vao_);
    // Draw triangles with indices
    glDrawElements(GL_TRIANGLES, index_count_, GL_UNSIGNED_SHORT, nullptr);
    // Done using this VAO
    glBindVertexArray(0);
}

Model Model::CreatePyramid()
{
    std::vector<GLfloat> verts =
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
    std::vector<GLushort> indices =
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

Model Model::CreateSphere(int latitude_divisions, int longitude_divisions, 
    float u_tiling, float v_tiling)
{
    if (latitude_divisions < 3)
        latitude_divisions = 3;
    if (longitude_divisions < 3)
        longitude_divisions = 3;
    std::vector<GLfloat> verts;
    float angle_incr = 2.0f * glm::pi<float>() / longitude_divisions;
    float y_angle_incr = -glm::pi<float>() / latitude_divisions;
    float y_angle = glm::pi<float>();
    float v_incr = v_tiling / latitude_divisions;
    float v = 0.0f;
    for (int i = 0; i < latitude_divisions + 1; i++) 
    {
        float radius = glm::sin(y_angle);
        float angle = 0.0f;
        float u_incr = u_tiling / longitude_divisions;
        float u = 0.0f;
        // Create circle of vertices
        for (int j = 0; j < longitude_divisions + 1; j++) 
        {
            float x = radius * glm::cos(angle);
            float y = glm::cos(y_angle);
            float z = radius * glm::sin(angle);
            AddVertex(verts, x, y, z, 1.0f, 1.0f, 1.0f, 1.0f, u, v, x, y, z);
            angle += angle_incr;
            u += u_incr;
        }
        y_angle += y_angle_incr;
        v += v_incr;
    }

    std::vector<GLushort> indices;
    // Rows of quads that make up sphere
    for (int i = 0; i < latitude_divisions; i++) 
    {
        for (int j = 0; j < longitude_divisions + 1; j++)
        {
            int row1 = i * (longitude_divisions + 1);
            int row2 = row1 + longitude_divisions + 1;
            indices.push_back(row1 + j);
            indices.push_back(row1 + (j + 1) % (longitude_divisions + 1));
            indices.push_back(row2 + (j + 1) % (longitude_divisions + 1));
            indices.push_back(row1 + j);
            indices.push_back(row2 + (j + 1) % (longitude_divisions + 1));
            indices.push_back(row2 + j);
        }
    }

    return Model(verts, indices);
}

/* Modification of createSphere with equal radius for each section and equal 
 * section height */
Model Model::CreateCylinder(int latitude_divisions, int longitude_divisions, 
    float u_tiling, float v_tiling)
{
    if (latitude_divisions < 3)
        latitude_divisions = 3;
    if (longitude_divisions < 3)
        longitude_divisions = 3;
    std::vector<GLfloat> verts;
    AddVertex(verts, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 
        0.0f, 0.0f, 0.0f, -1.0f, 0.0f);
    float angle_incr = 2.0f * glm::pi<float>() / longitude_divisions;
    float y_incr = 2.0f / latitude_divisions;
    float y = -1.0f;
    float v_incr = v_tiling / latitude_divisions;
    float v = 0.0f;
    for (int i = 0; i < latitude_divisions + 1; i++)
    {
        float radius = 1.0f;
        float angle = 0.0f;
        float u_incr = u_tiling / longitude_divisions;
        float u = 0.0f;
        // Create circle of vertices
        for (int j = 0; j < longitude_divisions + 1; j++)
        {
            float x = radius * glm::cos(angle);
            float z = radius * glm::sin(angle);
            AddVertex(verts, x, y, z, 1.0f, 1.0f, 1.0f, 1.0f, 
                u, v, x, 0.0f, z);
            angle += angle_incr;
            u += u_incr;
        }
        y += y_incr;
        v += v_incr;
    }
    AddVertex(verts, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 
        0.0f, 1.0f, 0.0f, 1.0f, 0.0f);

    std::vector<GLushort> indices;
    // Base of cylinder
    for (int i = 0; i < longitude_divisions + 1; i++)
    {
        indices.push_back(0);
        indices.push_back(i + 1);
        indices.push_back((i + 1) % (longitude_divisions + 1) + 1);
    }
    // Middle sections of cylinder
    for (int i = 0; i < latitude_divisions; i++)
    {
        for (int j = 0; j < (longitude_divisions + 1); j++)
        {
            int row1 = 1 + i * (longitude_divisions + 1);
            int row2 = row1 + longitude_divisions + 1;
            indices.push_back(row1 + j);
            indices.push_back(row1 + (j + 1) % (longitude_divisions + 1));
            indices.push_back(row2 + (j + 1) % (longitude_divisions + 1));
            indices.push_back(row1 + j);
            indices.push_back(row2 + (j + 1) % (longitude_divisions + 1));
            indices.push_back(row2 + j);
        }
    }
    // Top of cylinder
    int top = 1 + (longitude_divisions + 1) * (latitude_divisions + 1);
    for (int i = 0; i < (longitude_divisions + 1); i++)
    {
        indices.push_back(top - i - 1);
        indices.push_back(top - (i + 1) % (longitude_divisions + 1) - 1);
        indices.push_back(top);
    }

    return Model(verts, indices);
}

Model Model::CreateCube()
{
    std::vector<GLfloat> verts;
    std::vector<GLushort> indices;

    // Front
    AddVertex(verts, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 
        0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    AddVertex(verts, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 
        1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    AddVertex(verts, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f);
    AddVertex(verts, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f);

    // Back
    AddVertex(verts, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 
        1.0f, 0.0f, 0.0f, 0.0f, -1.0f);
    AddVertex(verts, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 
        0.0f, 0.0f, 0.0f, 0.0f, -1.0f);
    AddVertex(verts, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 
        0.0f, 1.0f, 0.0f, 0.0f, -1.0f);
    AddVertex(verts, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 
        1.0f, 1.0f, 0.0f, 0.0f, -1.0f);

    // Left
    AddVertex(verts, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
    AddVertex(verts, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 
        1.0f, 0.0f, -1.0f, 0.0f, 0.0f);
    AddVertex(verts, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 
        1.0f, 1.0f, -1.0f, 0.0f, 0.0f);
    AddVertex(verts, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 
        0.0f, 1.0f, -1.0f, 0.0f, 0.0f);

    // Right
    AddVertex(verts, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f);
    AddVertex(verts, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 
        0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
    AddVertex(verts, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f);
    AddVertex(verts, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f);

    // Bottom
    AddVertex(verts, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
        0.0f, 0.0f, 0.0f, -1.0f, 0.0f);
    AddVertex(verts, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 
        1.0f, 0.0f, 0.0f, -1.0f, 0.0f);
    AddVertex(verts, 1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 
        1.0f, 1.0f, 0.0f, -1.0f, 0.0f);
    AddVertex(verts, -1.0f, -1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 
        0.0f, 1.0f, 0.0f, -1.0f, 0.0f);

    // Top
    AddVertex(verts, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 
        0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    AddVertex(verts, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 
        1.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    AddVertex(verts, 1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f);
    AddVertex(verts, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 
        0.0f, 1.0f, 0.0f, 1.0f, 0.0f);

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

Model Model::CreateXZPlane(int divisions_x, int divisions_z,
    float u_tiling, float v_tiling)
{
    if (divisions_x < 1)
        divisions_x = 1;
    if (divisions_z < 1)
        divisions_z = 1;
    std::vector<GLfloat> verts;
    float xf = -1.0f;
    float x_incr = 2.0f / divisions_x;
    float v_incr = v_tiling / divisions_x;
    float v = 0.0f;
    for (int x = 0; x < divisions_x + 1; x++)
    {
        float zf = -1.0f;
        float z_incr = 2.0f / divisions_z;
        float u_incr = u_tiling / divisions_z;
        float u = 0.0f;
        for (int z = 0; z < divisions_z + 1; z++)
        {
            AddVertex(verts, xf, 0.0f, zf, 1.0f, 1.0f, 1.0f, 1.0f, 
                u, v, 0.0f, 1.0f, 0.0f);
            zf += z_incr;
            u += u_incr;
        }
        xf += x_incr;
        v += v_incr;
    }

    std::vector<GLushort> indices;
    for (int x = 0; x < divisions_x; x++)
    {
        int row1 = x * (divisions_z + 1);
        int row2 = row1 + divisions_z + 1;
        for (int z = 0; z < divisions_z; z++)
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

void Model::AddVertex(std::vector<GLfloat>& vertices, 
    float x, float y, float z, 
    float r, float g, float b, float a, 
    float u, float v, 
    float nx, float ny, float nz)
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
