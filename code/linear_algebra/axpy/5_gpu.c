/* OpenMP version with GPU offloading of *axpy
 * Compile with `gcc -o axpy 5_gpu.c -O1 -fopenmp -foffload=-misa=sm_35 -lm`
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include "../common.h"
#include "output.h"
#include <math.h>
#include <assert.h>

void saxpy(int n, float alpha, float* restrict x, float* restrict y) {
    if (n > 0 && alpha != 0.f) {
        #pragma omp target map(to:x[0:n],alpha) map(tofrom:y[0:n])
        {
            #pragma omp teams distribute parallel for simd
            for(int i=0; i < n; i++) {
                y[i] += alpha * x[i];
            }
        }
    }
}

void daxpy(int n, double alpha, double* restrict x, double* restrict y) {
    if (n > 0 && alpha != 0.f) {
        #pragma omp target map(to:x[0:n],alpha) map(tofrom:y[0:n])
        {
            #pragma omp teams distribute parallel for simd
            for(int i=0; i < n; i++) {
                y[i] += alpha * x[i];
            }
        }
    }
}

int main(int argc, char* argv[]) {
    int vec_size = -1, ntimes = -1, i=0;
    double time_saxpy = .0f, time_daxpy = .0f;
    struct timespec timer;
    
    printf("devices=%d\n", omp_get_num_devices());
    
    if(get_arguments(argc, argv, &vec_size, &ntimes) != 0) {
        printf("error while reading command line\n");
        return EXIT_FAILURE;
    }
    
    /* allocate */
    float* svecx = malloc(vec_size * sizeof(float));
    float* svecy = malloc(vec_size * sizeof(float));
    
    if (svecx == NULL || svecy == NULL) {
        printf("error while allocating svecx and svecy\n");
        return EXIT_FAILURE;
    }
    
    /* fill */
    #pragma omp parallel for simd
    for(i=0; i < vec_size; i++) {
        svecx[i] = VECX;
        svecy[i] = VECY;
    }
    
    /* compute saxpy */
    for(i = 0; i < ntimes; i++) {
        timer_start(&timer);
        saxpy(vec_size, AX, svecx, svecy);
        time_saxpy += timer_stop(&timer);
    }

    /* check */
    #pragma omp parallel for simd
    for(i=0; i < vec_size; i++) {
        if(fabs(svecy[i] - (VECY + ntimes * AX * VECX)) > __FLT_EPSILON__)
            printf("saxpy: error for element %d, got %.f\n", i, svecy[i]);
    }

    /* free */
    free(svecx);
    free(svecy);

    /* allocate */
    double* dvecx = malloc(vec_size * sizeof(double));
    double* dvecy = malloc(vec_size * sizeof(double));

    if (dvecx == NULL || dvecy == NULL) {
        printf("error while allocating dvecx and dvecy\n");
        return EXIT_FAILURE;
    }

    /* fill */
    #pragma omp parallel for simd
    for(i=0; i < vec_size; i++) {
        dvecx[i] = VECX;
        dvecy[i] = VECY;
    }
    
    /* compute daxpy */
    for(i = 0; i < ntimes; i++) {
        timer_start(&timer);
        daxpy(vec_size, AX, dvecx, dvecy);
        time_daxpy += timer_stop(&timer);
    }

    /* check */
    #pragma omp parallel for simd
    for(i=0; i < vec_size; i++) {
        if(fabs(dvecy[i] - (VECY + ntimes * AX * VECX)) > __DBL_EPSILON__)
            printf("daxpy: error for element %d, got %.f\n", i, dvecy[i]);
    }

    /* free */
    free(dvecx);
    free(dvecy);

    output_results(time_saxpy / ntimes, time_daxpy / ntimes);
    return EXIT_SUCCESS;
}
