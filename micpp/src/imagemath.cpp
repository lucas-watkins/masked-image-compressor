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

void ImageMath::dct_inplace(dct_t& candidate) {
    // We shouldn't copy this ideally but okay whatever
    dct_t old{ candidate };

    for (std::size_t v{ 0 }; v < candidate.height(); ++v) {
        for (std::size_t u{ 0 }; u < candidate.width(); ++u) {
            double sum{ 0.0 };


            for (std::size_t y{ 0 }; y < candidate.height(); ++y) {
                for (std::size_t x{ 0 }; x < candidate.width(); ++x) {
                    sum += old[x, y] * dct_cosine_term(y, v, candidate.height())
                        * dct_cosine_term(x, u, candidate.width())
                        * dct_coefficient_term(u, candidate.width()) * dct_coefficient_term(v, candidate.height());
                }
            }

            candidate[u, v] = sum;
        }
    }
}

ImageMath::dct_t ImageMath::dct(const Image& image, const std::size_t channel) {
    if (channel > 3) {
        throw std::runtime_error{ "Illegal channel, must be between 0 and 3" };
    }

    std::vector<double> dct{};
    dct.reserve(image.width() * image.height());

    for (std::size_t y{ 0 }; y < image.height(); ++y) {
        for (std::size_t x{ 0 }; x < image.width(); ++x) {
            dct.push_back(image[x,y][channel] / 255.0 * 2.0 - 1.0);
        }
    }

    Adaptor2D adaptor{ std::move(dct), image.width(), image.height() };

    dct_inplace(adaptor);

    return adaptor;
}
