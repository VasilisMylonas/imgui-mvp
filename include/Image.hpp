#pragma once

#include <string>
#include <vector>
#include <stdexcept>

class Image
{
public:
    Image(const std::string &path);
    Image(const std::vector<unsigned char> &bytes);
    ~Image();

    int width() const
    {
        return m_width;
    }

    int height() const
    {
        return m_height;
    }

    const std::string &path() const
    {
        return m_path;
    }

    void *texture() const
    {
        return reinterpret_cast<void *>(static_cast<uintptr_t>(m_texture));
    }

    void upload();

private:
    void *m_data;
    int m_width;
    int m_height;
    unsigned m_texture;
    std::string m_path;
};
