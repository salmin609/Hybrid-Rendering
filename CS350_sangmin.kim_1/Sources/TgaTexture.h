#pragma once
#include <stdexcept>
#include <string>
#include <GL/glew.h>

#include "loadTga.h"
class TgaTexture
{
public:
	TgaTexture(const std::string& path);
    void Use(unsigned shaderId);
    void Bind(int textureSlot = 0);
private:
	unsigned textureId;
};

inline TgaTexture::TgaTexture(const std::string& path)
{
    tTGA  tga;
    if (!load_TGA(&tga, path.c_str())) {
        throw std::runtime_error("ERROR: Could not load particle texture!");
    }

    glGetError();

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        (tga.alpha) ? (GL_RGB) : (GL_RGB8),
        tga.width,
        tga.height,
        0,
        (tga.alpha) ? (GL_RGBA) : (GL_RGB),
        GL_UNSIGNED_BYTE,
        tga.data
    );

    if (glGetError() != GL_NO_ERROR) {
        glDeleteTextures(1, &textureId);
        free_TGA(&tga);
        throw std::runtime_error("ERROR: Could not initialize particle texture!");
    }
	
}

inline void TgaTexture::Use(unsigned shaderId)
{
    glGetError();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);

    auto uniLocation = glGetUniformLocation(shaderId, "texture");

    if (glGetError() != GL_NO_ERROR) {
        throw std::runtime_error("ERROR: Could not use particle texture!");
    }
    glUniform1i(uniLocation, 0);
}

inline void TgaTexture::Bind(int textureSlot)
{
    glActiveTexture(GL_TEXTURE0 + textureSlot);
    glBindTexture(GL_TEXTURE_2D, textureId);
}
