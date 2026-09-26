use image::ImageReader;
use std::error::Error;
use std::fmt::{Display, Formatter};
use std::path::Path;

#[derive(Debug)]
pub enum ImageError {
    NotSquareImage
}

impl Display for ImageError {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        write!(f, "{:?}", self)
    }
}

// Error marking trait
impl Error for ImageError {}

// The Image struct is responsible for loading and processing images
pub struct Image {
    data: Vec<u8>,
    width: u32,
    height: u32,
}

impl Image {

    // Creates a new image based on a path
    pub fn load(filename: impl AsRef<Path>) -> Result<Image, Box<dyn Error>> {
        let img = ImageReader::open(filename)?.decode()?;

        let bytes = img.as_bytes();

        let i = Image {
            data: bytes.to_owned(),
            width: img.width(),
            height: img.height()
        };

        // Maybe later we'll support non-square images
        match i.width == i.height {
            true => Ok(i),
            false => Err(Box::new(ImageError::NotSquareImage))
        }
    }

    pub fn width(&self) -> u32 {
        self.width
    }

    pub fn height(&self) -> u32 {
        self.height
    }
}

