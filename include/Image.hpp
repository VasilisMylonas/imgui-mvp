#pragma once

#include <string>
#include <stdexcept>

class Image
{
public:
    Image(const std::string &path);
    ~Image();

    unsigned width() const
    {
        return m_width;
    }

    unsigned height() const
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

private:
    unsigned m_width;
    unsigned m_height;
    unsigned m_texture;
    std::string m_path;
};
