//
// Created by pierre on 04/11/2021.
//

#ifndef HPC_KERNEL_EXAMPLE_OUTPUT_H
#define HPC_KERNEL_EXAMPLE_OUTPUT_H

/* Output results in a standardized way
 * time_saxpy, time_daxpy: average time (in second)
 */
void output_results(double time_saxpy, double time_daxpy) {
    printf("%.4f ms | %.4f ms\n", time_saxpy * 1000, time_daxpy * 1000);
}


#endif //HPC_KERNEL_EXAMPLE_OUTPUT_H
