#include <cmath>
#include <image.hpp>

// Define needed to make the functions are actually implemented in the header.
// This header can only be included once, and it will only be included here
#ifndef STBI_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include <stb_image_mem.hpp>
#include <stb_image.h>
#endif


Image::Image(const std::filesystem::path& path) : m_width{ 0 }, m_height{ 0 }, m_channels{ 0 } {
    std::FILE* file{ std::fopen(path.c_str(), "rb") };

    if (!file) {
        throw std::runtime_error{ "Failed to open file" };
    }

    int width{};
    int height{};
    int channels{};


    std::unique_ptr<unsigned char[]> p_data{
        stbi_load_from_file(file, &width, &height, &channels, 4)
    };

    if (!p_data) {
        throw std::runtime_error{ "Failed to load image from file" };
    }

    m_width = width;
    m_height = height;
    m_channels = channels;

    m_data = std::move(p_data);
}

Image::Image(std::unique_ptr<unsigned char[]>&& data, const std::size_t width, const std::size_t height,
             const std::size_t channels) : m_width{ width }, m_height{ height }, m_channels{ channels } {
    if (!data || width <= 0 || height <= 0) {
        throw std::runtime_error{ "Invalid parameters" };
    }

    m_data = std::move(data);
}

Image::Pixel Image::pixel(const std::size_t x, const std::size_t y) const {
    Pixel p{};

    if (x >= m_width || y >= m_height) {
        throw std::out_of_range{ "Pixel index out of bounds" };
    }

    for (int i{ 0 }; i < m_channels; ++i) {
        p.rgba |=
            static_cast<std::uint32_t>(m_data.get()[(y * m_width + x) * m_channels + i]) << (8 * i);
    }

    return p;
}

Image::Pixel Image::operator[](const std::size_t x, const std::size_t y) const {
    return pixel(x, y);
}
