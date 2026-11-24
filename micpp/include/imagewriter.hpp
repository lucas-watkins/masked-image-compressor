#ifndef MICPP_IMAGEWRITER_HPP
#define MICPP_IMAGEWRITER_HPP

#include <filesystem>
#include <image.hpp>

// fs namespace alias for std::filesystem
namespace fs = std::filesystem;

// The ImageWriter class is designed to be inherited from the implement compression algorithms
// and provide easy ways to write an image to a file. Non const image writers can also modify the underlying image
class ImageWriter {
    Image m_image;

protected:
    // Get a reference to the image in the object, const or non-const
    [[nodiscard]] Image& image() { return m_image; }
    [[nodiscard]] const Image& image() const { return m_image; }

public:
    explicit ImageWriter(Image&& /* image */);

    // Writes a RGBA PNG image to the specified file path
    virtual void write(const fs::path& /* path */);

    // Sets a pixel's value
    void pixel(std::size_t /* x */, std::size_t /* y */, Image::Pixel /* pixel */);

    // Gets a pixel's value
    [[nodiscard]] Image::Pixel pixel(std::size_t /* x */, std::size_t /* y */) const;
    [[nodiscard]] Image::Pixel operator[](std::size_t /* x */, std::size_t /* y */) const;

    // Polymorphic destructor
    virtual ~ImageWriter() = default;
};

#endif //MICPP_IMAGEWRITER_HPP
