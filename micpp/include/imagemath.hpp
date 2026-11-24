#ifndef MICPP_IMAGEMATH_HPP
#define MICPP_IMAGEMATH_HPP
#include <vector>
#include <image.hpp>
#include <adaptor2d.hpp>

// ImageMath helps do calculations for images
class ImageMath {
public:
    // DCT type alias for result
    using dct_t = Adaptor2D<std::vector<double>>;

private:
    // Helper functions for the DCT
    static double dct_cosine_term(std::size_t /* a */, std::size_t /* b */, std::size_t /* blocksize */);
    static double dct_coefficient_term(std::size_t /* term number */, std::size_t /* blocksize */);

    // Calculates the DCT of a vector in place, already assumes that data is scaled from -1.0 to 1.0
    static void dct_inplace(dct_t& /* to modify */);

public:

    // Returns the DCT for a given image
    static dct_t dct(const Image& /* image to operate on */, std::size_t /* channel */);

    // No making an instance of this class
    ImageMath() = delete;
    ImageMath(const ImageMath&) = delete;
    ImageMath(ImageMath&&) = delete;
};

#endif //MICPP_IMAGEMATH_HPP
