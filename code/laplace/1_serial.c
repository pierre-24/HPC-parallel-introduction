/* Serial version of the 4-point jacobi stencil to solve the Laplace equation.
 * Compile it with `gcc -o laplace.serial 1_serial.c image_ppm.c -lm -O1`.
 * Run it with `./laplace.serial -i example_input.ppm`
 */

#include "image_ppm.h"
#include "common.h"
#include <math.h>

#define G(x,y) (U[(y) * width + (x)])
#define T(x,y) (tmp[(y) * width + (x)])

/* Compute the Laplace equation until the maximal change is lower than `threshold` or the number of iteration exceed `max_iter`.
 * The first and last row/column are used to get the values for the Dirichlet (i.e., fixed value) boundary conditions.
 * U: function
 * max_iter: maximal number of iteration
 * threshold: minimal change
 */
int laplace(FLT* U, unsigned int width, FLT dx, unsigned int height, FLT dy, int max_iter, FLT threshold) {
    if(U != NULL) {
        
        FLT* tmp = malloc(width * height * sizeof(FLT));
        if(tmp == NULL)
            return -1;
        
        FLT error = .0f;

        for(int iter=0; iter < max_iter; iter++) {
            error = .0f;

            for(int y=1; y < (height - 1); y++) {
                for(int x=1; x < (width - 1); x++) {
                    T(x, y) = (dy * dy * ( G(x+1, y) + G(x-1, y) ) +  dx * dx * ( G(x, y+1) + G(x, y-1) )) / (2 * dx * dx + 2 * dy * dy);
                }
            }

            for(int y=1; y < (height - 1); y++) {
                for(int x=1; x < (width - 1); x++) {
                    error = fmax(error, fabs(G(x,y) - T(x,y)));
                    G(x,y) = T(x,y);
                }
            }

            if (error < threshold) {
                break;
            }
        }
        
        printf("final error=%f\n", error);

        free(tmp);
        return 0;
    }
}

int main(int argc, char* argv[]) {
    unsigned int niter, width, height;
    FLT threshold;
    char *input_path, *output_path;

    printf("using sizeof(FLT)=%d\n", sizeof(FLT));

    /* fetch inputs */
    if(get_arguments(argc, argv, &niter, &threshold, &input_path, &output_path) != 0) {
        printf("error while reading command line\n");
        return EXIT_FAILURE;
    }

    if(input_path == NULL) {
        printf("input (-i) is required\n");
        return EXIT_FAILURE;
    }

    FILE* input = fopen(input_path, "r");
    if(input == NULL) {
        printf("error while opening input image\n");
        return EXIT_FAILURE;
    }

    Image* in = image_new_from_file(input);
    if (in == NULL) {
        printf("error while reading input image\n");
        return EXIT_FAILURE;
    }

    fclose(input);

    /* allocate */
    width = in->width;
    height = in->width;

    FLT* values = malloc(width * height * sizeof(FLT));
    if (values == NULL) {
        printf("error while allocating values\n");
        return EXIT_FAILURE;
    }

    /* fill */
    for(int i=0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            values[i * width + j] = .0;
        }
    }

    for (int j=0; j < width; j++)  {
        values[0 * width + j] = (FLT) (in->pixels[3*(TOP * in->width + j) + 0] - in->pixels[3* (TOP * in->width + j) + 2]) / 255;
        values[(height-1) * width + j] = (FLT) (in->pixels[3*(BOTTOM * in->width + j) + 0] - in->pixels[3* (BOTTOM * in->width + j) + 2]) / 255;
        values[j * width + 0] = (FLT) (in->pixels[3*(LEFT * in->width + j) + 0] - in->pixels[3* (LEFT* in->width + j) + 2]) / 255;
        values[j * width + (width-1)] = (FLT) (in->pixels[3*(RIGHT * in->width + j) + 0] - in->pixels[3* (RIGHT * in->width + j) + 2]) / 255;
    }

    image_delete(in);

    /* compute */
    struct timespec timer;
    timer_start(&timer);
    if(laplace(values, width, .1, height, .1, niter, threshold) != 0) {
        printf("error while executing laplace()\n");
        return EXIT_FAILURE;
    }
    printf("total time = %.3f secs\n", timer_stop(&timer));
    
    /* save output */
    if (output_path != NULL) {

        /* find output range */
        FLT max_positive = .0f, min_negative = .0f;
        for(int i=0; i < height; i++) {
            for(int j=0; j < width; j++) {
                FLT val = values[i * width + j];
                if (val < .0f)
                    min_negative = fmin(min_negative, val);
                else
                    max_positive = fmax(max_positive, val);
            }
        }

        printf("min_negative = %.3f, max_positive = %.3f\n", min_negative, max_positive);

        Image* im = image_new(width, height);
        for(int i=0; i < height; i++) {
            for(int j=0; j < width; j++) {
                FLT val = values[i * width + j];
                if (val < .0f)
                    im->pixels[3*(i * width + j) + 2] = (unsigned char) (val / min_negative * 255);
                else
                    im->pixels[3*(i * width + j) + 0] = (unsigned char) (val / max_positive * 255);
            }
        }

        FILE* output = fopen(output_path, "w");
        if (output == NULL) {
            printf("error while opening output image\n");
            return EXIT_FAILURE;
        }

        image_write(im, output);
        fclose(output);

        image_delete(im);
    }

    free(values);

    return EXIT_SUCCESS;
}
