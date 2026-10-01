use std::f64::consts::PI;

pub fn dct_type_2(input: &Vec<f64>, n: u32) -> Vec<f64> {
    let n = n as f64;

    let mut dct_output: Vec<f64> = vec![0.0; input.len()];

    let coeff = |x: u32| -> f64 {
        if x == 0 {
            1.0 / f64::sqrt(n)
        } else {
            f64::sqrt(2.0 / n)
        }
    };

    let n = n as u32;

    // The core of the dct. u = current x, v = current y, and this iterates the remaining for
    // each pixel. This is an inefficient algorithm. //TODO: Replace algorithm
    for v in 0..n {
        for u in 0..n {
            let mut sum = 0.0;

            for y in 0..n {
                for x in 0..n {
                    let current_val = input[(n * y + x) as usize];

                    let v = v as f64;
                    let u = u as f64;
                    let x = x as f64;
                    let y = y as f64;
                    let n = n as f64;

                    sum += current_val
                        * f64::cos((2.0 * y + 1.0) * v * PI / (2.0 * n))
                        * f64::cos((2.0 * x + 1.0) * u * PI / (2.0 * n));
                }
            }
            
            dct_output[(n * v + u) as usize] = sum * coeff(u) * coeff(v);
        }
    }

    dct_output
}
