/* Texture.cpp
 * Author: Harrison Bergeron
 * Date: July 23, 2022
 * Course: CS-499-H6772 Computer Science Capstone 22EW6 */

#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Texture::Texture()
{
    width_ = 0;
    height_ = 0;
    id_ = 0;
}

Texture::~Texture()
{
    glDeleteTextures(1, &id_);
}

void Texture::Load(const char* file)
{
    // Delete previously loaded texture
    glDeleteTextures(1, &id_);
    int channels;
    // load image and store width and height
    unsigned char* image = stbi_load(file, &width_, &height_, &channels, 0);
    if (image)
    {
        // Create and bind a new texture
        glGenTextures(1, &id_);
        glBindTexture(GL_TEXTURE_2D, id_);

        // Set the texture wrapping parameters.
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // Set texture filtering parameters.
        glTexParameteri(GL_TEXTURE_2D, 
            GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, 
            GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);

        // Load image data from stb to OpenGL
        if (channels == 3)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, width_, height_, 0, 
                GL_RGB, GL_UNSIGNED_BYTE, image);
        else if (channels == 4)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width_, height_, 0, 
                GL_RGBA, GL_UNSIGNED_BYTE, image);
        else
            return;

        // Create mipmaps for texture scaling
        glGenerateMipmap(GL_TEXTURE_2D);

        // Delete stb image data since it is no longer needed
        stbi_image_free(image);

        // Unind texture for now
        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

int Texture::GetWidth()
{
    return width_;
}

int Texture::GetHeight()
{
    return height_;
}

void Texture::Use(const char* uniform_name)
{
    /* Get OpenGL shader currently in use so that the texture uniform 
     * can be supplied */
    GLint shader = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &shader);

    // Place this texture in texture0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id_);

    // Use texture0 in shader
    glUniform1i(glGetUniformLocation(shader, uniform_name), 0);
}

void Texture::Unuse()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}