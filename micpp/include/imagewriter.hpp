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
    void pixel(int /* x */, int /* y */, Image::Pixel /* pixel */);

    // Gets a pixel's value
    [[nodiscard]] Image::Pixel pixel(int /* x */, int /* y */) const;
    [[nodiscard]] Image::Pixel operator[](int /* x */, int /* y */) const;

    // Polymorphic destructor
    virtual ~ImageWriter() = default;
};

#endif //MICPP_IMAGEWRITER_HPP
