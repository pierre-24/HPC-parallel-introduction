/* MPI version of axpy
 * Compile with `mpicc -o axpy 4_mpi.c -O1`
 * Run with `mpirun -np 4 ./axpy`
 */

#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include "../common.h"
#include "output.h"

#define SCATT_ROOT 0

void saxpy(int n, float alpha, float* restrict x, float* restrict y) {
    if (n > 0 && alpha != 0.f) {
        for(int i=0; i < n; i++) {
            y[i] += alpha * x[i];
        }
    }
}

void daxpy(int n, double alpha, double* restrict x, double* restrict y) {
    if (n > 0 && alpha != 0.f) {
        for(int i=0; i < n; i++) {
            y[i] += alpha * x[i];
        }
    }
}

int main(int argc, char* argv[]) {
    int vec_size = -1, ntimes = -1, i=0, rank, comm_size, partial_vec_size;
    double time_saxpy = .0f, time_daxpy = .0f;
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
    
    /* compute saxpy */
    for(i = 0; i < ntimes; i++) {
        if(rank == SCATT_ROOT)
            timer_start(&timer);

        MPI_Scatter(svecx, partial_vec_size, MPI_FLOAT, partial_svecx, partial_vec_size, MPI_FLOAT, SCATT_ROOT, MPI_COMM_WORLD);
        MPI_Scatter(svecy, partial_vec_size, MPI_FLOAT, partial_svecy, partial_vec_size, MPI_FLOAT, SCATT_ROOT, MPI_COMM_WORLD);

        saxpy(partial_vec_size, AX, partial_svecx, partial_svecy);

        MPI_Gather(partial_svecy, partial_vec_size, MPI_FLOAT, svecy, partial_vec_size, MPI_FLOAT, SCATT_ROOT, MPI_COMM_WORLD);
        
        if(rank == SCATT_ROOT)
            time_saxpy += timer_stop(&timer);
    }
    
    free(partial_svecx);
    free(partial_svecy);

    if(rank == SCATT_ROOT) {
        /* check */
        for(i=0; i < vec_size; i++) {
            if(fabs(svecy[i] - (VECY + ntimes * AX * VECX)) > __FLT_EPSILON__)
                printf("saxpy: error for element %d, got %.f\n", i, svecy[i]);
        }

        /* free */
        free(svecx);
        free(svecy);
    }

    if(rank == SCATT_ROOT) {
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
    
    /* compute saxpy */
    for(i = 0; i < ntimes; i++) {
        if(rank == SCATT_ROOT)
            timer_start(&timer);

        MPI_Scatter(dvecx, partial_vec_size, MPI_DOUBLE, partial_dvecx, partial_vec_size, MPI_DOUBLE, SCATT_ROOT, MPI_COMM_WORLD);
        MPI_Scatter(dvecy, partial_vec_size, MPI_DOUBLE, partial_dvecy, partial_vec_size, MPI_DOUBLE, SCATT_ROOT, MPI_COMM_WORLD);

        daxpy(partial_vec_size, AX, partial_dvecx, partial_dvecy);

        MPI_Gather(partial_dvecy, partial_vec_size, MPI_DOUBLE, dvecy, partial_vec_size, MPI_DOUBLE, SCATT_ROOT, MPI_COMM_WORLD);
        
        if(rank == SCATT_ROOT)
            time_daxpy += timer_stop(&timer);
    }
    
    free(partial_dvecx);
    free(partial_dvecy);
    
    if(rank == SCATT_ROOT) {
        /* check */
        for(i=0; i < vec_size; i++) {
            if(fabs(dvecy[i] - (VECY + ntimes * AX * VECX)) > __DBL_EPSILON__)
                printf("daxpy: error for element %d, got %.f\n", i, dvecy[i]);
        }

        /* free */
        free(dvecx);
        free(dvecy);

        /* output */
        output_results(time_saxpy / ntimes, time_daxpy / ntimes);
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}
