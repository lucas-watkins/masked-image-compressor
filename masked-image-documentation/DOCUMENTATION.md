## Masked Image Compression Example:

Here is the reference image. The main part of the image is the opaque pixels in the center, marked out by the mask. The transparent pixels are those that are not in the mask, which means they are not part of the image and the compression algorithm can do whatever it wants with those pixels. 

<img src="https://github.com/Open-Sourcery-UMD/masked-image-compressor/blob/main/masked-image-documentation/Reference-Masked.png" width="256" height="256" style="image-rendering: pixelated; image-rendering: crisp-edges;" />

When compressing the image using DCT, you need to set those pixels to something, as the DCT works by comparing every pixel. Here, the reference image just sets those pixels to a specific background color. This is not optimal for many reasons, and causes the DCT to compress badly around the edges of the mask.

For comparison, here is a example Optimized version, where the background pixels are set to something that compresses better. You can notice that the background now has a color that is closer to the average color of the image, and it looks like the image blurs into the background. This is just one possible optimization that can be done, so the task of this project is to find better and/or faster ways to choose the background pixels of this image.

<img src="https://github.com/Open-Sourcery-UMD/masked-image-compressor/blob/main/masked-image-documentation/Optimized-Final.png" width="256" height="256" style="image-rendering: pixelated; image-rendering: crisp-edges;" />

Here is where this pays off: When you apply the DCT to the Reference image, the resulting DCT Coefficients are shown on the left image. The right image shows the coefficients for the Optimized version. The Optimized DCT has reduced the high frequency components significantly, which shows up as the coefficients on the right image being dimmer, or even non-existent anymore. Less bits are needed to store the dimmer DCT, so the image has been successfully compressed!

<img src="https://github.com/Open-Sourcery-UMD/masked-image-compressor/blob/main/masked-image-documentation/Reference-DCT-Contrast.png" width="256" height="256" style="image-rendering: pixelated; image-rendering: crisp-edges;" /> <img src="https://github.com/Open-Sourcery-UMD/masked-image-compressor/blob/main/masked-image-documentation/Optimized-DCT-Contrast.png" width="256" height="256" style="image-rendering: pixelated; image-rendering: crisp-edges;" />

Also, note that this doesn’t change the pixels of the Opaque part of the image any, so when you apply the mask afterwards, you get the exact image back. The image has been compressed without any loss of information!

When doing lossy compression, where you get rid of parts of the DCT Coefficients that do have values, you also get better approximations of the image using the Optimized version. Here are some animated images showing what the reconstructed image looks like when you only use a subset of the DCT coefficients. The first frame is the DC approximation (solid color for the entire image), then the next frame adds on low frequency data, and adds progressively higher frequency data until the image has been reconstructed exactly. Because the Optimized version gets rid of unnecessary DCT coefficients, it ends up being a better approximation of the image than the Reference version when directly compared. 

On the left is the Reference, and the right is the Optimized version:

<img src="https://github.com/Open-Sourcery-UMD/masked-image-compressor/blob/main/masked-image-documentation/Reference-Masked-Sequence.gif" width="256" height="256" style="image-rendering: pixelated; image-rendering: crisp-edges;" /> <img src="https://github.com/Open-Sourcery-UMD/masked-image-compressor/blob/main/masked-image-documentation/Optimized-Masked-Sequence.gif" width="256" height="256" style="image-rendering: pixelated; image-rendering: crisp-edges;" />

Notice that the Reference version takes longer to get the correct color the border of the image, while the Optimized version gets the color correct on the border far quicker. This is because the Reference version is trying to get the color of the border correct on both sides of the border, whereas the Optimized version only cares about the color in the object side of the border. It doesn’t care about having the color bleed into the background because the background is irrelevant, so it makes good use of it in the low quality recreations.

Here is the same Reference on the left and Optimized on the right, without the mask:

<img src="https://github.com/Open-Sourcery-UMD/masked-image-compressor/blob/main/masked-image-documentation/Reference-Sequence.gif" width="256" height="256" style="image-rendering: pixelated; image-rendering: crisp-edges;" /> <img src="https://github.com/Open-Sourcery-UMD/masked-image-compressor/blob/main/masked-image-documentation/Optimized-Sequence.gif" width="256" height="256" style="image-rendering: pixelated; image-rendering: crisp-edges;" />

Without the mask, it is made obvious that the Reference version suffers from trying to approximate the harsh boundary between the Opaque pixels and the background. The Optimized version doesn’t need to try to recreate the harsh boundary, so it is able to get away with less high frequency coefficients. Aka, it can afford to be much blurrier, which is better for DCT compression.
