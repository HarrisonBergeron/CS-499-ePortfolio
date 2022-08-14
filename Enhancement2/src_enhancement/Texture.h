/* Texture.h
 * Author: Harrison Bergeron
 * Date: July 23, 2022
 * Course: CS-499-H6772 Computer Science Capstone 22EW6 */

#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

/* The Texture class represents a handle to an OpenGL texture. This class
 * exists to facilitate the loading of textures through the filesystem as
 * well as the binding of textures through OpenGL. */
class Texture
{
public:
    Texture();
    ~Texture();
    // Load the image data from a file
    void Load(const char* file);
    int GetWidth();
    int GetHeight();

    // Tell OpenGL to use this texture with the given shader variable name
    void Use(const char* uniform_name);
    void Unuse();
private:
    int width_, height_;
    GLuint id_;
};

#endif // TEXTURE_H