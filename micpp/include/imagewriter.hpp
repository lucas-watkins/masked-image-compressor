#ifndef MICPP_IMAGEWRITER_HPP
#define MICPP_IMAGEWRITER_HPP

#include <filesystem>
#include <image.hpp>

// The ImageWriter class is designed to be inherited from the implement compression algorithms
// and provide easy ways to write an image.
class ImageWriter {
    Image m_image;

public:
    explicit ImageWriter(Image&& /* image */);

    // Writes a RGBA PNG image to the specified file path
    void write(const std::filesystem::path& /* path */) const;

    // Polymorphic destructor
    virtual ~ImageWriter() = default;
};

#endif //MICPP_IMAGEWRITER_HPP
