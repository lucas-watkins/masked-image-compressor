#include <imagemath.hpp>
#include <stdexcept>
#include <cmath>

double ImageMath::dct_cosine_term(const std::size_t a, const std::size_t b, const std::size_t blocksize) {
    const auto a_double{ static_cast<double>(a) };
    const auto b_double{ static_cast<double>(b) };

    return std::cos((2.0 * a_double + 1.0) * b_double * std::numbers::pi / (2.0 * static_cast<double>(blocksize)));
}

double ImageMath::dct_coefficient_term(const std::size_t term_number, const std::size_t blocksize) {
    if (term_number == 0) {
        return 1.0 / std::sqrt(static_cast<double>(blocksize));
    }

    return std::sqrt(2.0 / static_cast<double>(blocksize));
}

ImageMath::dct_t ImageMath::dct(const Image& image, const std::size_t channel) {
    if (channel > 4) {
        throw std::runtime_error{ "Illegal channel, must be between 0 and 4" };
    }

    std::vector<double> dct{};
    dct.reserve(image.width() * image.height());

    for (std::size_t v{ 0 }; v < image.height(); ++v) {
        for (std::size_t u{ 0 }; u < image.width(); ++u) {
            double sum{ 0.0 };


            for (std::size_t y{ 0 }; y < image.height(); ++y) {
                for (std::size_t x{ 0 }; x < image.width(); ++x) {


                    const double channel_data{ image[x, y][channel] / 255.0 * 2.0 - 1.0 };

                    sum += channel_data * dct_cosine_term(y, v, image.height()) * dct_cosine_term(x, u, image.width())
                        * dct_coefficient_term(u, image.width()) * dct_coefficient_term(v, image.height());
                }
            }

            dct.push_back(sum);
        }
    }

    return Adaptor2D{ std::move(dct), image.width(), image.height() };
}
