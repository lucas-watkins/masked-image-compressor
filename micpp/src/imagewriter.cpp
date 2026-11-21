#include <imagewriter.hpp>

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_STATIC
#define STBI_MALLOC(sz) new unsigned char[sz]
#define STBI_FREE(p) delete[] p
#define STBI_REALLOC(p, nsz) ([](void* op, std::size_t sz) -> void* { void* n = new unsigned char[sz]; \
    if (n == nullptr) { return nullptr; }; \
    if (op != nullptr) { memcpy(n, op, sz); }; \
    delete[] op; return n;})(p, nsz)
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

Image::Pixel ImageWriter::operator[](const int x, const int y) const {
    return m_image[x, y];
}

Image::Pixel ImageWriter::pixel(const int x, const int y) const {
    return m_image[x, y];
}

void ImageWriter::pixel(const int x, const int y, const Image::Pixel pixel) {
    unsigned char* data { m_image.m_data.get() + (y * m_image.width() + x) * m_image.channels() };

    data[0] = pixel.r();
    data[1] = pixel.g();
    data[2] = pixel.b();
    data[3] = pixel.a();
}
