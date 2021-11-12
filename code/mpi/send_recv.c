/* Send and receive in ring
 * Compile it with `mpicc -o sr send_recv.c`
 * Run it with `mpirun -np 4 ./sr`
 */

#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

#define USE_ASYNC 0

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int rank, comm_size, next_rank, prev_rank, send_buffer, receive_buffer, sum = 0;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    next_rank = (rank + 1) % comm_size;
    prev_rank = (rank + comm_size - 1) % comm_size;
    send_buffer = rank;
    for(int i=0; i < comm_size; i++) {
#if USE_ASYNC == 0
        if (rank % 2 == 0) {
            MPI_Send(&send_buffer, 1, MPI_INT, next_rank, rank, MPI_COMM_WORLD);
            MPI_Recv(&receive_buffer, 1, MPI_INT, prev_rank, prev_rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        } else {
            MPI_Recv(&receive_buffer, 1, MPI_INT, prev_rank, prev_rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(&send_buffer, 1, MPI_INT, next_rank, rank, MPI_COMM_WORLD);
        }
#elif USE_ASYNC == 1
        MPI_Request requests[2];
        MPI_Isend(&send_buffer, 1, MPI_INT, next_rank, rank, MPI_COMM_WORLD, &requests[0]);
        MPI_Irecv(&receive_buffer, 1, MPI_INT, prev_rank, prev_rank, MPI_COMM_WORLD, &requests[1]);
        MPI_Waitall(2, requests, MPI_STATUS_IGNORE);
#elif USE_ASYNC == 2
        MPI_Request request;
        MPI_Isend(&send_buffer, 1, MPI_INT, next_rank, rank, MPI_COMM_WORLD, &request);
        MPI_Recv(&receive_buffer, 1, MPI_INT, prev_rank, prev_rank, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Wait(&request, MPI_STATUS_IGNORE);
#endif
        sum += receive_buffer;
        send_buffer = receive_buffer;
    }
    printf("as %d, sum is %d\n", rank, sum);
    
    MPI_Finalize();
    return EXIT_SUCCESS;
}
