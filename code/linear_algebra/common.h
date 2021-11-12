#ifndef INCLUDE_COMMON_H
#define INCLUDE_COMMON_H

#include <string.h>

#include "../timer.h"

#define VECX 2.0
#define VECY 3.0
#define AX 2.0

#define DEFAULT_VEC_SIZE 2<<24 // 128 Mb of double
#define DEFAULT_NTIMES 10

/* Get the program argument (if provided) from the command line options (`-n xx -N yy`) or fallback to defaults.
 * Returns 0 if the number was read, or if no option is provided,
 * Returns -1 if there is no number after the option,
 * Returns -2 if a number is <= 0.
 */
int get_arguments(int argc, char* argv[], int* vec_size, int* ntimes) {
    *vec_size = DEFAULT_VEC_SIZE;
    *ntimes = DEFAULT_NTIMES;
    
    if(argc > 1) {
        for (int i=0; i < argc; i++) {
            if(strcmp(argv[i], "-n") == 0) {
                if((i+1) == argc) { // `-n`, but no number provided :(
                    return -1;
                } else {
                    *vec_size = atoi(argv[i + 1]);
                    if(*vec_size < 1)
                        return -2;
                }
            } else if(strcmp(argv[i], "-N") == 0) {
                 if((i+1) == argc) { // `-N`, but no number provided :(
                    return -1;
                } else {
                    *ntimes = atoi(argv[i + 1]);
                    if(*ntimes < 1)
                        return -2;
                }
            }
        }
    }
    
    return 0;
}


#endif // INCLUDE_COMMON_H
