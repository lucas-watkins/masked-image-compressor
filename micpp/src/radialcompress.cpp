#include <radialcompress.hpp>
#include <cmath>
#include <imagemath.hpp>

std::size_t RadialCompress::coefficients_to_keep(const std::size_t x, const std::size_t y) const {
    const double compression_constant{ 1.0 - static_cast<double>(m_compression_percentage) / 100.0 };

    const std::size_t center_x{ image().width() / std::size_t{ 2 } };
    const std::size_t center_y{ image().height() / std::size_t{ 2 } };

    return static_cast<std::size_t>(
        compression_constant * (std::pow(m_blocksize, 2) - (std::pow(center_x - x, 2) + std::pow(center_y - y, 2)))
    );
}

std::size_t RadialCompress::top_row_keep_count(const std::size_t total) const {
    std::size_t high{ m_blocksize };
    std::size_t low{ 0 };
    std::size_t mid{};

    while (low <= high) {
        mid = (high + low) / 2;

        if (const std::size_t sum{ (mid * mid + mid) / 2 }; sum < total) {
            low = mid + 1;
        } else if (sum == total) {
            return mid;
        } else {
            high = mid - 1;
        }
    }

    return mid;
}

void RadialCompress::write(const fs::path& path) {
    const std::size_t width{ image().width() };
    const std::size_t height{ image().height() };

    ImageMath::dct_t dct_channel0{ ImageMath::dct(image(), 0) };
    ImageMath::dct_t dct_channel1{ ImageMath::dct(image(), 1) };
    ImageMath::dct_t dct_channel2{ ImageMath::dct(image(), 2) };
    ImageMath::dct_t dct_channel3{ ImageMath::dct(image(), 3) };

    // The idea is that we keep more of the dct coefficients the closer we are to the center of the image
    // and also discard more the further we are from the image
    for (std::size_t u{ 0 }; u < height; u += m_blocksize) {
        for (std::size_t v{ 0 }; v < width; v += m_blocksize) {
            std::size_t coefficients_in_row{ top_row_keep_count(coefficients_to_keep(v, u)) };

            for (std::size_t y{ u }; y < std::min(u + m_blocksize, height); ++y) {
                std::size_t coefficients_put{ 0 };

                for (std::size_t x{ v }; x < std::min(v + m_blocksize, width); ++x) {
                    if (coefficients_put < coefficients_in_row) {
                        coefficients_put++;
                    } else {
                        dct_channel0[x, y] = 0;
                        dct_channel1[x, y] = 0;
                        dct_channel2[x, y] = 0;
                        dct_channel3[x, y] = 0;
                    }
                }

                if (coefficients_in_row > 0) {
                    coefficients_in_row--;
                }
            }
        }
    }

    ImageWriter::write(path);
}
