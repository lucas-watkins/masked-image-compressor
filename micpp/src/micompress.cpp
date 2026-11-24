#include <argparse.hpp>
#include <iostream>
#include <image.hpp>

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



        return 0;
    } catch (std::exception& ex) {
        std::cout << ex.what() << '\n';
        return 1;
    }
}

// Complete the square and solve the sum of integer equation
static std::size_t find_top_mask_count(const std::size_t total, const std::size_t blocksize) {

}
