//
// Created by pierre on 04/11/2021.
//

#ifndef HPC_KERNEL_EXAMPLE_OUPTPUT_H
#define HPC_KERNEL_EXAMPLE_OUPTPUT_H

/* Output results in a standardized way
 * result_sdot, result_ddot: result of the dot product
 * time_sdot, time_ddot: average time (in second)
 */
void output_results(double time_sdot, double time_ddot) {
    printf("%.4f ms | %.4f ms\n", time_sdot * 1000, time_ddot * 1000);
}

#endif //HPC_KERNEL_EXAMPLE_OUPTPUT_H
