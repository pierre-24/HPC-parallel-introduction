set term pdfcairo enhanced dashed font 'Helvetica,24' size 20cm,12cm dashed
set output "result_OMP_saxpy.pdf"

set xlabel "Number of threads"
set ylabel "Average time (ms)"

plot 'Data_OMP.csv' i 0 u 1:2 w lp pt 5 lc rgb "red"  dashtype 2 t 'SP (v1)',\
    '' i 0 u 1:3 w lp lc rgb "blue" dashtype 2 pt 5 t 'DP (v1)',\
    '' i 1 u 1:2 w lp lc rgb "red" pt 7 t 'SP (v2)',\
    '' i 1 u 1:3 w lp lc rgb "blue" pt 7 t 'DP (v2)'
    
 
set output "result_OMP_dot.pdf"

plot 'Data_OMP.csv' i 2 u 1:2 w lp pt 5 lc rgb "red"  t 'SP (v2)',\
    '' i 2 u 1:3 w lp lc rgb "blue" pt 5 t 'DP (v2)',\
