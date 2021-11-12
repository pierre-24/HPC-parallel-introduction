# `axpy` (`a` * `x` + `y`)

Serial and parallel implementation of one of the simplest operation, the generalized vector addition ([`*axpy`](https://www.ibm.com/docs/en/essl/6.2?topic=vss-sdot-ddot-caxpy-zaxpy-multiply-vector-by-scalar-add-vector-store-in-vector) in LAPACK), in a simplified form (no `incx` and `incy`).

Prototypes:

```c
/* Compute y:= alpha * x + y, where alpha is a scalar, and x and y are n-vectors.
 * n: the number of element in the vector. Must be >= 1, otherwise nothing happen.
 * alpha: the scalar. If alpha is 0, nothing happen.
 * x: array of size n (remains untouched).
 * y: array of size n.
 */
void sdot(int n, float alpha, float* x, float* y); // single-precision
void ddot(int n, double alpha, double* x, double* y); // double-precision
```
