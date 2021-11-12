# `dot` (`x` · `y`)

Serial and parallel implementation of the dot product between two vectors ([`*dot`](https://www.ibm.com/docs/en/essl/6.2?topic=vss-sdot-ddot-cdotu-zdotu-cdotc-zdotc-dot-product-two-vectors) in LAPACK), in a simplified form (no `incx` and `incy`).

Prototypes:

```c
/* Compute the dot product between x and y, where x and y are n-vectors.
 * n: the number of element in the vector. Must be >= 1, otherwise zero is returned.
 * x: array of size n (remains untouched).
 * y: array of size n (remains untouched).
 */
float sdot(int n, float* x, float* y); // single-precision
float ddot(int n, double* x, double* y); // double-precision
```

Uses a [Kahan sum](https://en.wikipedia.org/wiki/Kahan_summation_algorithm) to mitigate the numerical error.