/* OMP version of the computation of pi(n).
 * Interesting to check the different scheduling clauses in OMP.
 * Compile with `gcc -o pi 3_omp.c -fopenmp`
 * Execute with something like `OMP_NUM_THREADS=4 ./pi`
 */

#include <stdio.h>
#include "../timer.h"

#define PI_LOOP for(int i=2; i < n; i++) { for (int j=2; j < i; j++) { if (i % j == 0) { not_prime++; break; } } }

int pi_default(int n) {
    int not_prime = 2;
    #pragma omp parallel for reduction(+:not_prime)
    PI_LOOP
    return n - not_prime;
}

int pi_static(int n) {
    int not_prime = 2;
    #pragma omp parallel for schedule(static) reduction(+:not_prime)
    PI_LOOP
    return n - not_prime;
}

int pi_dynamic(int n) {
    int not_prime = 2;
    #pragma omp parallel for schedule(dynamic) reduction(+:not_prime)
    PI_LOOP
    return n - not_prime;
}

int pi_guided(int n) {
    int not_prime = 2;
#pragma omp parallel for schedule(guided) reduction(+:not_prime)
    PI_LOOP
    return n - not_prime;
}

int main() {
    struct timespec timer;
    int pi_n;
    double time_default, time_static, time_dynamic, time_guided;

    printf(""
           "                         default     static    dynamic     guided\n"
           "         n      pi(n)       time       time       time       time\n"
           "-----------------------------------------------------------------\n");

    for (int i=9; i < 20; i++) {
        int N = 2 << i;

        // default
        timer_start(&timer);
        pi_n = pi_default(N);
        time_default = timer_stop(&timer);

        // static
        timer_start(&timer);
        pi_n = pi_static(N);
        time_static = timer_stop(&timer);

        // dynamic
        timer_start(&timer);
        pi_n = pi_dynamic(N);
        time_dynamic = timer_stop(&timer);

        // guided
        timer_start(&timer);
        pi_n = pi_guided(N);
        time_guided = timer_stop(&timer);

        // print
        printf("%10d %10d %10.5f %10.5f %10.5f %10.5f\n", N, pi_n, time_default, time_static, time_dynamic, time_guided);
    }
}