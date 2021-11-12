#!/bin/bash

function _in {
  # credits: https://stackoverflow.com/a/8574392
  local e match="$1"
  shift
  for e; do [[ "$e" == "$match" ]] && return 0; done
  return 1
}

# serial
if $(_in "+full" "$@") || $(_in "+serial" "$@") ; then
  exec="bench_axpy_serial"
  gcc -o $exec 1_serial.c -O0 -lm
  echo -n "Serial -O0  | " & ./$exec $1
  gcc -o $exec 1_serial.c -O1 -lm
  echo -n "Serial -O1  | " & ./$exec $1
  rm $exec
  fi

# vector
if $(_in "+full" "$@") || $(_in "+vector" "$@") ; then
  exec="bench_axpy_vector"
  gcc -o $exec 2_vector.c -O1 -lm -ftree-vectorize
  echo -n "Vector      | " & ./$exec $1
  gcc -o $exec 2_vector.c -O1 -lm -ftree-vectorize -march=native -mtune=native
  echo -n "Vector arch | " & ./$exec $1
  gcc -o $exec 2_vector.c -O1 -lm -ftree-vectorize -mavx2
  echo -n "Vector AVX  | " & ./$exec $1
  rm $exec
  fi

# OMP
if $(_in "+full" "$@") || $(_in "+omp" "$@") ; then
  exec="bench_axpy_omp"
  gcc -o $exec 3_omp_v1.c -lm -O1 -fopenmp
  export OMP_NUM_THREADS=1
  echo -n "OMP (v1) 1T | " & ./$exec $1
  export OMP_NUM_THREADS=2
  echo -n "OMP (v1) 2T | " & ./$exec $1
  export OMP_NUM_THREADS=4
  echo -n "OMP (v1) 4T | " & ./$exec $1
  export OMP_NUM_THREADS=8
  echo -n "OMP (v1) 8T | " & ./$exec $1
  export OMP_NUM_THREADS=16
  echo -n "OMP (v1) 16T| " & ./$exec $1
  gcc -o $exec 3_omp_v2.c -lm -O1 -fopenmp
  export OMP_NUM_THREADS=1
  echo -n "OMP (v2) 1T | " & ./$exec $1
  export OMP_NUM_THREADS=2
  echo -n "OMP (v2) 2T | " & ./$exec $1
  export OMP_NUM_THREADS=4
  echo -n "OMP (v2) 4T | " & ./$exec $1
  export OMP_NUM_THREADS=8
  echo -n "OMP (v2) 8T | " & ./$exec $1
  export OMP_NUM_THREADS=16
  echo -n "OMP (v2) 16T|" & ./$exec $1
  rm $exec
  fi

# MPI
if $(_in "+full" "$@") || $(_in "+mpi" "$@") ; then
  exec="bench_axpy_mpi"
  mpicc -o $exec 4_mpi.c -lm -O1
  echo -n "MPI 1T      | " & mpirun -np 1 ./$exec $1
  echo -n "MPI 2T      | " & mpirun -np 2 ./$exec $1
  echo -n "MPI 4T      | " & mpirun -np 4 ./$exec $1
  echo -n "MPI 8T      | " & mpirun -np 8 ./$exec $1
  echo -n "MPI 16T     | " & mpirun -np 16 ./$exec $1
  rm $exec
  fi
