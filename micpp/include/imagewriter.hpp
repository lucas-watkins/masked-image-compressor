#ifndef MICPP_IMAGEWRITER_HPP
#define MICPP_IMAGEWRITER_HPP

#include <filesystem>
#include <image.hpp>

// The ImageWriter class is designed to be inherited from the implement compression algorithms
// and provide easy ways to write an image to a file. Non const image writers can also modify the underlying image
class ImageWriter {
    Image m_image;

public:
    explicit ImageWriter(Image&& /* image */);

    // Writes a RGBA PNG image to the specified file path
    void write(const std::filesystem::path& /* path */) const;

    // Sets a pixel's value
    void pixel(std::size_t /* x */, std::size_t /* y */, Image::Pixel /* pixel */);

    // Gets a pixel's value
    [[nodiscard]] Image::Pixel pixel(std::size_t /* x */, std::size_t /* y */) const;
    [[nodiscard]] Image::Pixel operator[](std::size_t /* x */, std::size_t /* y */) const;

    // Polymorphic destructor
    virtual ~ImageWriter() = default;
};

#endif //MICPP_IMAGEWRITER_HPP
