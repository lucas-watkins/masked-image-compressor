use libmicrs::image::Image;
use std::env::args;
use std::error::Error;

fn main() -> Result<(), Box<dyn Error>> {
    let args: Vec<String> = args().collect();

    if args.len() != 2 {
        eprintln!(
            "Dumps a list of pixels transformed by the DCT type II.\nUsage: dctdump [imagefile]"
        );
        return Ok(());
    }

    let img = Image::load(&args[1])?;

    let dct = img.dct_type_2();

    let get_dct = |x, y| dct[(y * img.width() + x) as usize];

    for y in 0..img.height() {
        for x in 0..img.width() {
            println!("{:>02?} -> {:>4?}", (x, y), get_dct(x, y));
        }
    }

    Ok(())
}
