use image::{GenericImageView, ImageReader};
use std::env::args;
use std::error::Error;

fn main() -> Result<(), Box<dyn Error>> {
    let args: Vec<String> = args().collect();

    if args.len() != 2 {
        eprintln!("Dumps a list of pixels in the image.\nUsage: imgdump [imagefile]");
        return Ok(());
    }

    let img = ImageReader::open(&args[1])?.decode()?;

    for y in 0..img.height() {
        for x in 0..img.width() {
            let pixel = img.get_pixel(x, y).0;
            println!("{:0>2?} -> {:>4?}", (x, y), pixel);
        }
    }

    Ok(())
}
