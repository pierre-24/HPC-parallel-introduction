set term pdfcairo enhanced dashed font 'Helvetica,24' size 20cm,12cm dashed
set output "result_MPI.pdf"

set xlabel "Number of threads"
set ylabel "Average time (ms)"

plot 'Data_MPI.csv' i 0 u 1:2 w lp pt 5 lc rgb "red" t 'saxpy',\
    '' i 0 u 1:3 w lp lc rgb "blue" pt 5 t 'daxpy',\
    '' i 1 u 1:2 w lp lc rgb "red" dashtype 2 pt 7 t 'sdot',\
    '' i 1 u 1:3 w lp lc rgb "blue" dashtype 2 pt 7 t 'ddot',\
    '' i 2 u 1:2 w lp lc rgb "red" dashtype 3 pt 9 t 'sdot (different node)',\
    '' i 2 u 1:3 w lp lc rgb "blue" dashtype 3 pt 9 t 'ddot (different node)'
    
