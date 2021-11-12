#ifndef COMMON_H
#define COMMON_H

#include "../timer.h"

#define DEFAULT_NITER 10000
#define DEFAULT_THRESHOLD 1e-6

#ifndef FLT
#define FLT double
#endif


 __inline FLT ffmax(FLT left, FLT right) { // definition, since CUDA does not seems to have that one
    return (left > right) ? left : right; 
}

enum {
    TOP,
    BOTTOM,
    LEFT,
    RIGHT
};

int get_arguments(int argc, char* argv[], unsigned int* n_iter, FLT* threshold, char** input, char** output) {
    *n_iter = DEFAULT_NITER;
    *threshold = DEFAULT_THRESHOLD;
    *input = NULL;
    *output = NULL;

    if(argc > 1) {
        for (int i=0; i < argc; i++) {
            if(strcmp(argv[i], "-N") == 0) {
                if((i+1) == argc) { // `-N`, but no number provided :(
                    return -1;
                } else {
                    *n_iter = atoi(argv[i + 1]);
                    if(*n_iter < 1)
                        return -2;
                }
            } else if(strcmp(argv[i], "-t") == 0) {
                if((i+1) == argc) { // `-t`, but no number provided :(
                    return -1;
                } else {
                    char* end;
                    *threshold = strtod(argv[i + 1], &end);
                    if(argv[i + 1] == end)
                        return -2;
                }
            } else if(strcmp(argv[i], "-i") == 0) {
                if((i+1) == argc) { // `-i`, but nothing!
                    return -1;
                } else {
                    *input = argv[i + 1];
                }
            } else if(strcmp(argv[i], "-o") == 0) {
                if((i+1) == argc) { // `-o`, but nothing!
                    return -1;
                } else {
                    *output = argv[i + 1];
                }
            }
        }
    }

    return 0;
}

#endif // COMMON_H
