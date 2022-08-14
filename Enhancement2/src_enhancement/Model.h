/* Model.h
 * Author: Harrison Bergeron
 * Date: July 23, 2022
 * Course: CS-499-H6772 Computer Science Capstone 22EW6 */

#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <random>

/* The Model class contains 3D geometry data to represent a 3D object in space.
 * It may be displayed on the screen with a transform matrix to allow it to be
 * moved, rotated, or scaled. Additionally, a texture may be bound externally
 * to affect the appearance of the model. This class was included to simplify
 * the generation and rendering of 3D geometric primitives. */
class Model
{
public:
    // Construct the model with specified geometry data.
    Model(std::vector<GLfloat>& verts, std::vector<GLushort>& indices);
    ~Model();

    // Draw this model on the screen with a given linear transfomration.
    void Draw(glm::mat4 transform);

    // Create a pyramid primitive.
    static Model CreatePyramid();

    /* Create a sphere primitive. Latitude and longitude divisions control
     * the resolution of the sphere mesh. */
    static Model CreateSphere(int latitude_divisions, int longitude_divisions, 
        float u_tiling, float v_tiling);

    /* Create a cylinder primitive. Latitude and longitude divisions control
     * the resolution of the cylinder mesh. */
    static Model CreateCylinder(int latitude_divisions, 
        int longitude_divisions, float u_tiling, float v_tiling);

    // Create a cube primitive.
    static Model CreateCube();

    /* Create a plane primitive aligned with the XZ plane. The x and z
     * divisions control the resolution of the plane mesh. The u tiling
     * and v tiling parameters control the scale of the model texture. */
    static Model CreateXZPlane(int divisions_x, int divisions_z, 
        float u_tiling, float v_tiling);
private:
    // Vertex array object and vertex buffer objects to hold geometry data.
    GLuint vao_;
    GLuint vbo_vertices_;
    GLuint vbo_indices_;
    GLsizei index_count_;

    /* Add a vertex to a vertex array with the given position, color, 
     * texture coordinate, and normal data. */
    static void AddVertex(std::vector<GLfloat>&vertices,
        float x, float y, float z,
        float r, float g, float b, float a, 
        float u, float v, 
        float nx, float ny, float nz);
};

#endif // MODEL_H