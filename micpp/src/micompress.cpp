#include <argparse.hpp>
#include <iostream>
#include <image.hpp>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#endif

// Responsible for handling the rest of the program after we parse the arguments
static int run(const argparse::ArgumentParser& parser);

// Helps find how many elements on the top row we should keep
static std::size_t find_top_mask_count(std::size_t total, std::size_t blocksize);

int main(const int argc, char* argv[]) {
    argparse::ArgumentParser parser{ "micompress" };

    parser.add_description("micompress is used to compress masked images with targeted algorithms.");

    parser
        .add_argument("--file")
        .help("The image file to compress.")
        .required();

    parser
        .add_argument("--blocksize")
        .help("Specify n for the n * n processing window for the DCT.")
        .default_value(std::size_t{ 8 })
        .scan<'u', std::size_t>();

    parser
        .add_argument("--percentage")
        .help("How aggressively micompress should compress the image.")
        .default_value(double{ 0.75 })
        .scan<'f', double>();

    try {
        parser.parse_args(argc, argv);
    } catch (std::exception& ex) {
        std::cout << ex.what() << '\n';
        return 1;
    }

    return run(parser);
}

int run(const argparse::ArgumentParser& parser) {
    try {
        const std::filesystem::path input_filepath{ parser.get("--file") };
        const auto blocksize{ parser.get<std::size_t>("--blocksize") };
        const auto compression_percentage{ parser.get<double>("--percentage") };
        const Image image{ input_filepath };

        Image::dct_t adaptor{ image.dct(0) };

        const std::size_t center_x{ adaptor.width() / 2 };
        const std::size_t center_y{ adaptor.height() / 2 };

        // How many coefficients should we keep for each block, depends on the distance from center
        auto coeffs_to_keep{
            [center_x, center_y, blocksize, compression_percentage](const std::size_t x,
                                                                    const std::size_t y) -> std::size_t {
                return static_cast<std::size_t>(
                    (1 - compression_percentage) * (std::pow(blocksize, 2) - (std::pow(center_x - x, 2) +
                    std::pow(center_y - y, 2)))
                );
            }
        };

        // The idea is that we keep more of the dct coefficients the closer we are to the center of the image
        // and also discard more the further we are from the image
        for (std::size_t u{ 0 }; u < adaptor.height(); u += blocksize) {
            for (std::size_t v{ 0 }; v < adaptor.width(); v += blocksize) {
                std::size_t coeffs_in_row{ find_top_mask_count(coeffs_to_keep(v, u), blocksize) };

                for (std::size_t y{ u }; y < std::min(u + blocksize, adaptor.height()); ++y) {
                    std::size_t coeffs_put{ 0 };

                    for (std::size_t x{ v }; x < std::min(v + blocksize, adaptor.width()); ++x) {
                        if (coeffs_put < coeffs_in_row)
                            coeffs_put++;
                        else
                            adaptor[x, y] = 0;
                    }

                    coeffs_in_row--;
                }
            }
        }


        return 0;
    } catch (std::exception& ex) {
        std::cout << ex.what() << '\n';
        return 1;
    }
}

// Complete the square and solve the sum of integer equation
static std::size_t find_top_mask_count(const std::size_t total, const std::size_t blocksize) {
    std::size_t high{ blocksize };
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
