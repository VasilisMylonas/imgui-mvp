#include "Image.hpp"

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void Image::upload(void *data)
{
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
}

Image::Image(const std::vector<unsigned char> &bytes) : m_path{""}
{
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    stbi_uc *data = stbi_load_from_memory(bytes.data(), bytes.size(), &m_width, &m_height, nullptr, 0);

    if (data == nullptr)
    {
        throw std::runtime_error("Could not load image");
    }

    upload(data);
    stbi_image_free(data);
}

Image::Image(const std::string &path) : m_path{path}
{
    glGenTextures(1, &m_texture);
    glBindTexture(GL_TEXTURE_2D, m_texture);

    stbi_uc *data = stbi_load(path.c_str(), &m_width, &m_height, nullptr, 0);

    if (data == nullptr)
    {
        throw std::runtime_error("Could not load image");
    }

    upload(data);
    stbi_image_free(data);
}

Image::~Image()
{
    glDeleteTextures(1, &m_texture);
}
