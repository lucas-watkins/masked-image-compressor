#include <imagewriter.hpp>

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_STATIC
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_mem.hpp>
#include <stb_image_write.h>
#endif

ImageWriter::ImageWriter(Image&& image) : m_image{ std::move(image) } {}

void ImageWriter::write(const std::filesystem::path& path) const {
    // Consider only writing RGB images to save space

    const auto width{ static_cast<int>(m_image.width()) };
    const auto height{ static_cast<int>(m_image.height()) };


    if (stbi_write_png(path.c_str(), width, height, 4 /* RGBA */, m_image.m_data.get(), 4 * width) <= 0) {
        throw std::runtime_error{ "Error while writing file!" };
    }
}

Image::Pixel ImageWriter::operator[](const std::size_t x, const std::size_t y) const {
    return m_image[x, y];
}

Image::Pixel ImageWriter::pixel(const std::size_t x, const std::size_t y) const {
    return m_image[x, y];
}

void ImageWriter::pixel(const std::size_t x, const std::size_t y, const Image::Pixel pixel) {
    unsigned char* data{ m_image.m_data.get() + (y * m_image.width() + x) * m_image.channels() };

    data[0] = pixel.r();
    data[1] = pixel.g();
    data[2] = pixel.b();
    data[3] = pixel.a();
}
