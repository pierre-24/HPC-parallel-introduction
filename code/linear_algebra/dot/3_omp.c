/* OMP version of *dot (with the reduction clause!!)
 * Compile with `gcc -o dot 3_omp_v1.c -O1 -lm -fopenmp`
 * Don't forget `export OMP_NUM_THREADS=xx` to run.
 */

#include <stdio.h>
#include <stdlib.h>
#include "../common.h"
#include "output.h"
#include <math.h>

float sdot(int n, float* restrict x, float* restrict y) {
    float sum = .0f, c = .0f, q, r;
    if (n > 0) {
        #pragma omp parallel for reduction(+:sum) firstprivate(c) private(q, r)
        for(int i=0; i < n; i++) {
            q = y[i] * x[i] - c;
            r = sum + q;
            c = (r - sum) - q;
            sum = r;
        }
    }
    return sum;
}

double ddot(int n, double* restrict x, double* restrict y) {
    double sum = .0f, c = .0f, q, r;
    if (n > 0) {
        #pragma omp parallel for reduction(+:sum) firstprivate(c) private(q, r)
        for(int i=0; i < n; i++) {
            q = y[i] * x[i] - c;
            r = sum + q;
            c = (r - sum) - q;
            sum = r;
        }
    }
    return sum;
}

int main(int argc, char* argv[]) {
    int vec_size = -1, ntimes = -1, i;
    float result_sdot;
    double result_ddot, time_sdot = .0f, time_ddot = .0f;
    struct timespec timer;
    
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
    
    /* compute sdot */
    for(i = 0; i < ntimes; i++) {
        timer_start(&timer);
        result_sdot = sdot(vec_size, svecx, svecy);
        time_sdot += timer_stop(&timer);
    }

    if (fabs(result_sdot - vec_size * VECX * VECY) > __FLT_EPSILON__)
        printf("sdot: the value is incorrect!\n");

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

    /* compute ddot */
    for(i = 0; i < ntimes; i++) {
        timer_start(&timer);
        result_ddot = ddot(vec_size, dvecx, dvecy);
        time_ddot += timer_stop(&timer);
    }

    if (fabs(result_ddot - vec_size * VECX * VECY) > __DBL_EPSILON__)
        printf("ddot: the value is incorrect!\n");

    /* free */
    free(dvecx);
    free(dvecy);

    output_results(time_sdot / ntimes, time_ddot / ntimes);
    return EXIT_SUCCESS;
}
