#ifndef HPC_KERNEL_EXAMPLE_IMAGE_PPM_H
#define HPC_KERNEL_EXAMPLE_IMAGE_PPM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Image_ {
    /* Image (array of pixels with a given width and height)
     *
     * The pixels are stored in a one dimentional array (dynamically allocated).
     * A pixel may be accessed as `(y * width + x) * 3 + c`, where `x` and `y` are the position of the pixel, and `c` is the component (red, green, blue).
     *
     * Note that an image have its origin on the top left corner.
     * */
    unsigned int width;
    unsigned int height;
    unsigned char * pixels; // dynamically allocated
} Image;

Image* image_new(unsigned int width, unsigned int height);
Image* image_new_from_file(FILE *f);
unsigned char* image_get_pixel(Image* image, unsigned int x, unsigned int y);
int image_set_pixel(Image* image, unsigned int x, unsigned int y, const unsigned char* components);
int image_write(Image* image, FILE *f);
void image_delete(Image *image);

#endif //HPC_KERNEL_EXAMPLE_IMAGE_PPM_H
