use libmicrs::image::*;

#[cfg(test)]
fn test_image_load() {
    let image = Image::load("../comprehensive-examples/input_image.png");

    assert!(image.is_ok());

    let image = image.unwrap();

    assert!(image.height() == 32 && image.width() == 32);
}

#[cfg(test)]
fn test_bad_load() {
    let image = Image::load("totallyanonexistentpath");

    assert!(image.is_err());
}
