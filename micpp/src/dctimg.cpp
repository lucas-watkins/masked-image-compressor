#include <iostream>
#include <algorithm>
#include <cstring>
#include <argparse.hpp>
#include <image.hpp>
#include <imagewriter.hpp>


// Responsible for handling the rest of the program after we parse the arguments
static int run(const argparse::ArgumentParser& parser);

int main(const int argc, char* argv[]) {
    argparse::ArgumentParser parser{ "dctimg" };

    parser.add_description(
        "dctimg is used to calculate the DCT coefficients of an image and display them in another image.");

    parser
        .add_argument("--file")
        .help("The image file to get the DCT coefficients from.")
        .required();

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
        const Image image{ input_filepath };

        const Adaptor2D adaptor{ image.dct(0) };

        const auto width { static_cast<int>(adaptor.width()) };
        const auto height { static_cast<int>(adaptor.height()) };

        ImageWriter image_writer{
            Image{ std::unique_ptr<unsigned char[]>(new unsigned char[width * height * 4]), width, height, 4}
        };

        const double max{
            std::abs(*std::max_element(adaptor.cbegin(), adaptor.cend(), [](const double x, const double y) {
                return std::abs(x) < std::abs(y);
            }))
        };

        for (std::size_t y{ 0 }; y < adaptor.height(); ++y) {
            for (std::size_t x{ 0 }; x < adaptor.width(); ++x) {
                const auto scaled{ static_cast<std::uint8_t>(adaptor[x, y] / max * 255) };

                const Image::Pixel pixel{ scaled, scaled, scaled, scaled };

                image_writer.pixel(static_cast<int>(x), static_cast<int>(y), pixel);
            }
        }

        image_writer.write("dctimg.png");

        return 0;
    } catch (std::exception& ex) {
        std::cout << ex.what() << '\n';
        return 1;
    }
}
