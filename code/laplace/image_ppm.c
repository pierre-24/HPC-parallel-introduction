#include "image_ppm.h"

#define MAGIC_WHITESPACE "%*[ \n\t]"

/* Create a new image, filled with black pixels.
 */
Image* image_new(unsigned int width, unsigned int height) {
    Image* image = malloc(sizeof(Image));

    if(image == NULL)
        return NULL;

    image->width = width;
    image->height = height;
    image->pixels = calloc(image->width * image->height * 3, sizeof(unsigned char));

    return image;
}

Image* image_new_from_file(FILE *f)  {
    /* Create and return an image.
     * Read the PPM format (see http://netpbm.sourceforge.net/doc/ppm.html).
     * Only handles a maxval=255
     */
    if (f == NULL)
        return NULL;

    char c[3] = {(char) fgetc(f), (char) fgetc(f), '\0'};

    // first line should be "P6"
    if(strcmp(c, "P6") != 0) {
        return NULL;
    }

    fgetc(f);

    // second line is a comment
    if(fgetc(f) == '#') {
        while (fgetc(f) != '\n')
            continue;
    }

    // then comes the width, height and maximum value
    unsigned int max, width, height;
    fscanf(f, "%u" MAGIC_WHITESPACE "%u" MAGIC_WHITESPACE "%u" MAGIC_WHITESPACE, &width, &height, &max);

    if (max != 255) // I only handle normal RGB, sorry :/
        return NULL;

    // then, the data
    Image* image = image_new(width, height);
    if(image == NULL) {
        return NULL;
    }

    size_t sz = 3 * image->width * image->height;
    if(fread(image->pixels, sizeof (unsigned char), sz, f) != sz) {
        image_delete(image);
        return NULL;
    }

    return image;
}

int image_write(Image * im, FILE *f)  {
    /* Write an image
     * Write in the PPM format (see http://netpbm.sourceforge.net/doc/ppm.html).
     */

    if (f == NULL || im == NULL)
        return -1;

    // first line is "P6", then width, height, color
    fprintf(f, "P6 %u %u %u\n", im->width, im->height, 255);

    size_t sz = 3 * im->width * im->height;
    if(fwrite(im->pixels, sizeof (unsigned char), 3 * im->width * im->height, f) != sz) {
        return -2;
    }

    return 0;
}

/* Get the pointer to a given pixel
 */
unsigned char* image_get_pixel(Image* image, unsigned int x, unsigned int y) {
    if(image == NULL)
        return NULL;

    return &image->pixels[3*(image->width * y + x)];
}

/* Set the value of a given pixel
 */
int image_set_pixel(Image* image, unsigned int x, unsigned int y, const unsigned char* components) {
    if(image == NULL)
        return -1;

    image->pixels[3*(image->width * y + x) + 0] = components[0];
    image->pixels[3*(image->width * y + x) + 1] = components[1];
    image->pixels[3*(image->width * y + x) + 2] = components[2];

    return 0;
}

void image_delete(Image *image) {
    /* Delete an image (and its pixels).
     * */
    if (image != NULL) {
        if (image->pixels != NULL)
            free(image->pixels);
        free(image);
    }
}