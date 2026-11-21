#include <cmath>
#include <image.hpp>

#ifndef NDEBUG
#include <cassert>
#endif

// Define needed to make the functions are actually implemented in the header.
// This header can only be included once, and it will only be included here
#ifndef STBI_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STBI_MALLOC(sz) new unsigned char[sz]
#define STBI_FREE(p) delete[] p
#define STBI_REALLOC(p, nsz) ([](void* op, std::size_t sz) -> void* { void* n = new unsigned char[sz]; \
    if (n == nullptr) { return nullptr; }; \
    if (op != nullptr) { memcpy(n, op, sz); }; \
    delete[] op; return n;})(p, nsz)
#include <stb_image.h>
#endif


Image::Image(const std::filesystem::path& path) : m_width{ 0 }, m_height{ 0 }, m_channels{ 0 } {
    std::FILE* file{ std::fopen(path.c_str(), "rb") };

    if (file == nullptr) {
        throw std::runtime_error("Failed to open file");
    }


    std::unique_ptr<unsigned char[]> p_data{
        stbi_load_from_file(file, &m_width, &m_height, &m_channels, 4)
    };

    if (p_data == nullptr) {
        throw std::runtime_error("Failed to load image from file");
    }

    m_data = std::move(p_data);
}

Image::Image(std::unique_ptr<unsigned char[]>&& data, const int width, const int height, const int channels) :
    m_width{ width }, m_height{ height }, m_channels{ channels } {
    if (data == nullptr || width <= 0 || height <= 0) {
        throw std::runtime_error("Invalid parameters");
    }

    m_data = std::move(data);
}

Image::dct_t Image::dct(const std::size_t channel) const {
    if (channel > 4)
        throw std::runtime_error("Illegal channel, must be between 0 and 4");

    std::vector<double> dct{};
    dct.reserve(m_width * m_height);

    auto cos_term{
        [](const std::size_t a, const std::size_t b, const int size) -> double {
            const auto a_double{ static_cast<double>(a) };
            const auto b_double{ static_cast<double>(b) };

            return std::cos((2.0 * a_double + 1.0) * b_double * std::numbers::pi / (2.0 * static_cast<double>(size)));
        }
    };

    auto coefficient{
        [](const std::size_t x, const int size) -> double {
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

                    const double channel_0{ m_data.get()[index] / 255.0 * 2.0 - 1.0 };

                    sum += channel_0 * cos_term(y, v, m_height) * cos_term(x, u, m_width)
                        * coefficient(u, m_width) * coefficient(v, m_height);
                }
            }

            dct.push_back(sum);
        }
    }

#ifndef NDEBUG
    assert(dct.size() == m_width * m_height);
#endif

    return Adaptor2D{ std::move(dct), static_cast<std::size_t>(m_width), static_cast<std::size_t>(m_height) };
}

Image::Pixel Image::pixel(const int x, const int y) const {
    Pixel p{};

    if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
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

Image::Pixel Image::operator[](int x, int y) const {
    return pixel(x, y);
}
