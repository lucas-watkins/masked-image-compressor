# Lucas's Image Format

This document aims to describe a file format that is easy to parse, easy to compress, and easy to integrate
into already existing programs.

# Inspiration

The inspiration for this file format is the file formats contained
within [Netpbm](https://en.wikipedia.org/wiki/Netpbm). 

# How it works
Everything should just be pure binary in the file, so to access the data reading the file in binary mode is
required.

1. The first 32 bits are the width of the image.
2. The second 32 bits are the height of the image.
3. Every 24 bits is a pixel where the first 8 bits correspond to the red component, the next 8 bits to the
   green component, and the next 8 bits to the blue component.
4. If the amount of pixels doesn't match the width multiplied by the height of the image, this is an error.
5. All this data is compressed using the [DEFLATE](https://every-algorithm.github.io/2025/02/06/deflate.html)
   algorithm. 

Ideally, you decompress the data stored in the file and operate on it given the above information or compress
the data you have already operated on in the above format. 
