/* MPI version of *dot
 * Compile with `mpicc -o dot 4_mpi.c -O1 -lm`
 * Run it with `mpirun -np 4 ./dot`
 */
 
#include <stdio.h>
#include <stdlib.h>
#include "../common.h"
#include "output.h"
#include <math.h>
#include <mpi.h>

#define SCATT_ROOT 0

float sdot(int n, float* x, float* y) {
    float sum = .0f, c = .0f, q, r;
    if (n > 0) {
        for(int i=0; i < n; i++) {
            q = y[i] * x[i] - c;
            r = sum + q;
            c = (r - sum) - q;
            sum = r;
        }
    }
    return sum;
}

double ddot(int n, double* x, double* y) {
    double sum = .0f, c = .0f, q, r;
    if (n > 0) {
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
    int vec_size = -1, ntimes = -1, i, rank, comm_size, partial_vec_size;
    float result_sdot, partial_result_sdot;
    double result_ddot, partial_result_ddot, time_sdot = .0f, time_ddot = .0f;
    float *svecx, *svecy, *partial_svecx, *partial_svecy;
    double *dvecx, *dvecy, *partial_dvecx, *partial_dvecy;
    struct timespec timer;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    
    if(get_arguments(argc, argv, &vec_size, &ntimes) != 0) {
        printf("error while reading command line\n");
        return EXIT_FAILURE;
    }

    partial_vec_size = vec_size / comm_size;
    
    if(rank == SCATT_ROOT) {
        /* allocate */
        svecx = malloc(vec_size * sizeof(float));
        svecy = malloc(vec_size * sizeof(float));
        
        if (svecx == NULL || svecy == NULL) {
            printf("error while allocating svecx and svecy\n");
            return EXIT_FAILURE;
        }
        
        /* fill */
        for(i=0; i < vec_size; i++) {
            svecx[i] = VECX;
            svecy[i] = VECY;
        }
    }
    
    partial_svecx = malloc(sizeof(float) * partial_vec_size);
    partial_svecy = malloc(sizeof(float) * partial_vec_size);

    if(partial_svecx == NULL || partial_svecy == NULL) {
        printf("error while allocating partial_svecx and partial_svecy\n");
        return EXIT_FAILURE;
    }
        
    /* compute sdot */
    for(i = 0; i < ntimes; i++) {
        if(rank == SCATT_ROOT)
            timer_start(&timer);

        MPI_Scatter(svecx, partial_vec_size, MPI_FLOAT, partial_svecx, partial_vec_size, MPI_FLOAT, SCATT_ROOT, MPI_COMM_WORLD);
        MPI_Scatter(svecy, partial_vec_size, MPI_FLOAT, partial_svecy, partial_vec_size, MPI_FLOAT, SCATT_ROOT, MPI_COMM_WORLD);

        partial_result_sdot = sdot(partial_vec_size, partial_svecx, partial_svecy);

        MPI_Reduce(&partial_result_sdot, &result_sdot, 1, MPI_FLOAT, MPI_SUM, SCATT_ROOT, MPI_COMM_WORLD);

        if(rank == SCATT_ROOT)
            time_sdot += timer_stop(&timer);
    }

    free(partial_svecx);
    free(partial_svecy);

    if(rank == SCATT_ROOT) {
        if (fabs(result_sdot - vec_size * VECX * VECY) > __FLT_EPSILON__)
            printf("sdot: the value is incorrect!\n");

        /* free */
        free(svecx);
        free(svecy);

        /* allocate */
        dvecx = malloc(vec_size * sizeof(double));
        dvecy = malloc(vec_size * sizeof(double));

        if (dvecx == NULL || dvecy == NULL) {
            printf("error while allocating dvecx and dvecy\n");
            return EXIT_FAILURE;
        }

        /* fill */
        for(i=0; i < vec_size; i++) {
            dvecx[i] = VECX;
            dvecy[i] = VECY;
        }

    }
    
    partial_dvecx = malloc(sizeof(double) * partial_vec_size);
    partial_dvecy = malloc(sizeof(double) * partial_vec_size);

    if(partial_dvecx == NULL || partial_dvecy == NULL) {
        printf("error while allocating partial_dvecx and partial_dvecy\n");
        return EXIT_FAILURE;
    }

    /* compute ddot */
    for(i = 0; i < ntimes; i++) {
        if(rank == SCATT_ROOT)
            timer_start(&timer);

        MPI_Scatter(dvecx, partial_vec_size, MPI_DOUBLE, partial_dvecx, partial_vec_size, MPI_DOUBLE, SCATT_ROOT, MPI_COMM_WORLD);
        MPI_Scatter(dvecy, partial_vec_size, MPI_DOUBLE, partial_dvecy, partial_vec_size, MPI_DOUBLE, SCATT_ROOT, MPI_COMM_WORLD);

        partial_result_ddot = ddot(partial_vec_size, partial_dvecx, partial_dvecy);

        MPI_Reduce(&partial_result_ddot, &result_ddot, 1, MPI_DOUBLE, MPI_SUM, SCATT_ROOT, MPI_COMM_WORLD);

        if(rank == SCATT_ROOT)
            time_ddot += timer_stop(&timer);
    }

    free(partial_dvecx);
    free(partial_dvecy);

    if(rank == SCATT_ROOT) {
        if (fabs(result_ddot - vec_size * VECX * VECY) > __DBL_EPSILON__)
            printf("ddot: the value is incorrect!\n");

        /* free */
        free(dvecx);
        free(dvecy);

        output_results(time_sdot / ntimes, time_ddot / ntimes);
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}
