/* Example of MPI hello world
 * Compile it with `mpicc -o hello hello.c`
 * Run it with `mpirun -np 4 ./hello`
 */

#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int rank, size, len;
    char node_name[MPI_MAX_PROCESSOR_NAME];
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Get_processor_name(node_name, &len);
    printf("Rank=%d, size=%d (node=%s)\n", rank, size, node_name);
    MPI_Finalize();
    return EXIT_SUCCESS;
}
