#include <cmath>
#include <image.hpp>

#ifndef NDEBUG
#include <cassert>
#endif

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

Image::dct_t Image::dct(const std::size_t channel) const {
    if (channel > 4) {
        throw std::runtime_error{ "Illegal channel, must be between 0 and 4" };
    }

    std::vector<double> dct{};
    dct.reserve(m_width * m_height);

    auto cos_term{
        [](const std::size_t a, const std::size_t b, const std::size_t size) -> double {
            const auto a_double{ static_cast<double>(a) };
            const auto b_double{ static_cast<double>(b) };

            return std::cos((2.0 * a_double + 1.0) * b_double * std::numbers::pi / (2.0 * static_cast<double>(size)));
        }
    };

    auto coefficient{
        [](const std::size_t x, const std::size_t size) -> double {
            if (x == 0) {
                return 1.0 / std::sqrt(static_cast<double>(size));
            }

            return std::sqrt(2.0 / static_cast<double>(size));
        }
    };

    for (std::size_t v{ 0 }; v < m_height; ++v) {
        for (std::size_t u{ 0 }; u < m_width; ++u) {
            double sum{ 0.0 };


            for (std::size_t y{ 0 }; y < m_height; ++y) {
                for (std::size_t x{ 0 }; x < m_width; ++x) {
                    const std::size_t index{ (y * m_width + x) * m_channels + channel };

                    const double channel_data{ m_data.get()[index] / 255.0 * 2.0 - 1.0 };

                    sum += channel_data * cos_term(y, v, m_height) * cos_term(x, u, m_width)
                        * coefficient(u, m_width) * coefficient(v, m_height);
                }
            }

            dct.push_back(sum);
        }
    }

#ifndef NDEBUG
    assert(dct.size() == m_width * m_height);
#endif

    return Adaptor2D{ std::move(dct), m_width, m_height };
}

Image::Pixel Image::pixel(const std::size_t x, const std::size_t y) const {
    Pixel p{};

    if (x >= m_width || y >= m_height) {
        throw std::runtime_error("Pixel index out of bounds");
    }

#ifndef NDEBUG
    assert(m_channels <= 4);
#endif

    for (int i{ 0 }; i < m_channels; ++i) {
        p.rgba |=
            static_cast<std::uint32_t>(m_data.get()[(y * m_width + x) * m_channels + i]) << (8 * i);
    }

    return p;
}

Image::Pixel Image::operator[](const std::size_t x, const std::size_t y) const {
    return pixel(x, y);
}
