use libmicrs::image::*;

#[test]
fn test_image_load() {
    let image = Image::load("../comprehensive-examples/input_image.png");

    assert!(image.is_ok());

    let image = image.unwrap();

    assert!(image.height() == 32 && image.width() == 32);
}

#[test]
fn test_bad_load() {
    let image = Image::load("totallyanonexistentpath");

    assert!(image.is_err());
}

#[test]
fn print_non_square_error() {
    println!("{}", ImageError::NotSquareImage);
}
