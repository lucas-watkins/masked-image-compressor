use image::{ImageReader, DynamicImage, GenericImageView};
use std::error::Error;
use std::fmt::{Display, Formatter};
use std::path::Path;
use std::f64::consts::PI;

// The ImageError enum is used for indicating what errors we have while processing
#[derive(Debug)]
pub enum ImageError {
    NotSquareImage
}

// We want to be able to print enum variants
impl Display for ImageError {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(f, "{:?}", self)
    }
}

// Error marking trait to allow this to be treated as an error and be put in a pointer
impl Error for ImageError {}

// The Image struct is responsible for loading and processing images
pub struct Image {
    img: DynamicImage,
}

impl Image {

    // Creates a new image based on a path
    pub fn load(filename: impl AsRef<Path>) -> Result<Image, Box<dyn Error>> {
        let img = ImageReader::open(filename)?.decode()?;

        let bytes = img.as_bytes();

        let i = Image {
            img,
        };

        // Maybe later we'll support non-square images
        match i.width() == i.height() {
            true => Ok(i),
            false => Err(Box::new(ImageError::NotSquareImage))
        }
    }

    // Returns width
    pub fn width(&self) -> u32 {
        self.img.width()
    }

    // Returns height
    pub fn height(&self) -> u32 {
        self.img.height()
    }

    // Returns the value of a pixel
    pub fn pixel(&self, x: u32, y: u32) -> (u8, u8, u8, u8) {
        // I have no idea why there's an anonymous public struct field that does this.
        self.img.get_pixel(x, y).0.into()
    }

    pub fn dct_type_2(&self) -> Vec<i8> {
        let mut dct_output: Vec<i8> = vec![0; (self.width() * self.height()) as usize];

        let pixel_f64 = |x: u32, y: u32| {
            (self.pixel(x, y).3 as f64) - 128.0
        };

        // Do the entire image (the image is square)
        let blocksize = self.width();

        let cos_term = |a: u32, b: u32| -> f64 {
            f64::cos(((2.0 * (a as f64) + 1.0) * (b as f64) * PI) / (2.0 * (blocksize as f64)))
        };

        let coeff = |x: u32| if x == 0 { 1.0 / f64::sqrt(2.0) } else { 1.0 };

        // The core of the dct. i = current x, j = current y, and this iterates the remaining for
        // each pixel. This is an inefficient algorithm. //TODO: Replace algorithm
        for i in 0..blocksize {
            for j in 0..blocksize {
                let mut temp = 0.0;

                for x in 0..blocksize {
                    for y in 0..blocksize {
                        temp += pixel_f64(x, y) * cos_term(x, i) * cos_term(y, j);
                    }
                }

                temp *= f64::sqrt(2.0 * (blocksize as f64)) * coeff(i) * coeff(j);

                dct_output[(self.width() as usize) * (i as usize) + (j as usize)] = temp as i8;
            }
        }

        dct_output
    }
}

