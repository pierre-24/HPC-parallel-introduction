/* Ping pong to measure (roughly) the bandwidth. 
 * Inspired by https://github.com/olcf-tutorials/MPI_ping_pong. Assymptotically tends to the bandwidth.
 * Compile it with `mpicc -o ping ping_pong.c`
 * Run it with `mpirun -np 2 ./ping`
 */

#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

#define N_PING_PONG 100
#define N_WARM_UP 5
#define N_LOOP 27 // up to 1Gb of data

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);

    int rank, comm_size;
    double elapsed = .0;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

    for(int i=0; i < N_LOOP; i++) {
        long N = 2 << i;

        double* A = calloc(N, sizeof(double));

        for(int k=0; k < N_WARM_UP; k++) { // warm-up
            if (rank == 0) {
                MPI_Send(A, N, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
                MPI_Recv(A, N, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            } else {
                MPI_Recv(A, N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Send(A, N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
            }
        }

        elapsed = MPI_Wtime();
        for(int k=0; k < N_PING_PONG; k++) {
            if (rank == 0) {
                MPI_Send(A, N, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD);
                MPI_Recv(A, N, MPI_DOUBLE, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            } else {
                MPI_Recv(A, N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                MPI_Send(A, N, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
            }
        }
        elapsed = MPI_Wtime() - elapsed;

        if (rank == 0) {
            long int num_bytes = sizeof(double) * N;
            double size_in_gb = (double) num_bytes / (2 << 29);
            double time_per_transfer = elapsed / (2 * N_PING_PONG);
            printf("Size (B): %10li, Transfer time (s): %15.9f, Bandwidth (GB/s): %15.9f\n", num_bytes, time_per_transfer, size_in_gb / time_per_transfer);
        }

        free(A);
    }
    
    MPI_Finalize();
    return EXIT_SUCCESS;
}
