#include <imagewriter.hpp>

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_STATIC
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#endif

ImageWriter::ImageWriter(Image&& image) : m_image{ std::move(image) } {}

void ImageWriter::write(const std::filesystem::path& path) const {
    // Consider only writing RGB images to save space

    if (stbi_write_png(path.c_str(), m_image.width(), m_image.height(), 4 /* RGBA */, m_image.m_data.get(),
                       4 * m_image.width()) <= 0) {
        throw std::runtime_error{ "Error while writing file!" };
    }
}
