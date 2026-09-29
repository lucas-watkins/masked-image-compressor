use std::f64::consts::PI;

pub fn dct_type_2(input: &Vec<f64>, n: u32) -> Vec<i8> {
    let mut dct_output: Vec<i8> = vec![0; input.len()];

    let cos_term = |a: u32, b: u32| -> f64 {
        f64::cos(((2.0 * (a as f64) + 1.0) * (b as f64) * PI) / (2.0 * (n as f64)))
    };

    let coeff = |x: u32| if x == 0 { 1.0 / f64::sqrt(2.0) } else { 1.0 };

    // The core of the dct. i = current x, j = current y, and this iterates the remaining for
    // each pixel. This is an inefficient algorithm. //TODO: Replace algorithm
    for i in 0..n {
        for j in 0..n {
            let mut temp = 0.0;

            for x in 0..n {
                for y in 0..n {
                    temp += input[(y * n + x) as usize] * cos_term(x, i) * cos_term(y, j);
                }
            }

            temp *= f64::sqrt(2.0 * (n as f64)) * coeff(i) * coeff(j);

            dct_output[(n as usize) * (i as usize) + (j as usize)] = temp as i8;
        }
    }

    dct_output
}
