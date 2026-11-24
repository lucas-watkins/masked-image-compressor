#ifndef MICPP_IMAGE_HPP
#define MICPP_IMAGE_HPP
#include <filesystem>
#include <memory>
#include <vector>

// Image represents a generic image at runtime that we can perform processes on
class Image {
    std::size_t m_width;
    std::size_t m_height;
    std::size_t m_channels;

    // Image data
    std::unique_ptr<unsigned char[]> m_data;

public:
    // Load image from path
    Image(const std::filesystem::path& /* path to image file */);

    // DANGEROUS!!! DO NOT FREE THE POINTER YOU PROVIDE HERE ELSEWHERE!!!
    Image(std::unique_ptr<unsigned char[]>&& /* data */, std::size_t /* width */, std::size_t /* height */,
          std::size_t /* channels */);

    // Move constructor, just the default is okay
    Image(Image&&) = default;

    // The pixel union holds information about a certain pixel as a record
    union Pixel {
        Pixel() = default;

        // Allow construction from parts
        Pixel(const std::uint8_t r, const std::uint8_t g, const std::uint8_t b, const std::uint8_t a)
            : m_channel{ r, g, b, a } {}

        // Getters
        [[nodiscard]] std::uint8_t r() const {
            return m_channel[0];
        }

        [[nodiscard]] std::uint8_t g() const {
            return m_channel[1];
        }

        [[nodiscard]] std::uint8_t b() const {
            return m_channel[2];
        }

        [[nodiscard]] std::uint8_t a() const {
            return m_channel[3];
        }

        [[nodiscard]] std::uint8_t channel(const std::size_t c) const {
            if (c > 3) {
                throw std::out_of_range { "Channel index out of range" };
            }

            return m_channel[c];
        }

        [[nodiscard]] std::uint8_t operator[](const std::size_t c) const {
            return channel(c);
        }

    private:
        // 0 = r, 1 = g, 2 = b, 3 = a
        std::uint8_t m_channel[4];
        std::uint32_t rgba;

        // Image needs internal access
        friend Image;
    };

    // Getters
    [[nodiscard]] std::size_t width() const { return m_width; }
    [[nodiscard]] std::size_t height() const { return m_height; }
    [[nodiscard]] std::size_t channels() const { return m_channels; }

    [[nodiscard]] Pixel pixel(std::size_t /* x */, std::size_t /* y */) const;
    [[nodiscard]] Pixel operator[](std::size_t /* x */, std::size_t /* y */) const;

    // Friend class for ImageWriter to access internal image information. Image and ImageWriter are closely
    // linked so this is okay.
    friend class ImageWriter;
};

#endif //MICPP_IMAGE_HPP
