#ifndef MICPP_RADIALCOMPRESS_HPP
#define MICPP_RADIALCOMPRESS_HPP
#include <imagewriter.hpp>

class RadialCompress : public ImageWriter {
    // How much we should compress the package
    std::size_t m_compression_percentage;
    std::size_t m_blocksize;

    // Function to determine how many coefficients we will keep
    std::size_t coefficients_to_keep(std::size_t /* x */, std::size_t /* y */) const;

    // Function to determine how many coefficients should be kept at the top of a row in the block
    std::size_t top_row_keep_count(std::size_t /* total coefficients */) const;

public:
    // Proxy superclass constructor
    explicit RadialCompress(Image&& image, const std::size_t compression_percentage, const std::size_t blocksize) :
        ImageWriter{ std::move(image) }, m_compression_percentage{ compression_percentage }, m_blocksize{ blocksize } {
        if (m_compression_percentage > 100) {
            throw std::runtime_error{ "The compression percentage can't be over 100" };
        }
    }

    void write(const fs::path& path) override;
};

#endif //MICPP_RADIALCOMPRESS_HPP
