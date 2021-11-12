set term pdfcairo enhanced dashed font 'Helvetica,24' size 20cm,12cm dashed
set output "result_GPU_Laplace.pdf"

set key left
set xlabel "Width of the grid"
set ylabel "Time (s)"

set xtics ("1024" 10, "2048" 11, "4096" 12, "8192" 13, "16384" 14)

plot 'Data_GPU.csv' i 0 u (log($1)/log(2)):2 w lp pt 5 lc rgb "red" t 'SP (OMP)',\
    '' i 0 u (log($1)/log(2)):3 w lp lc rgb "blue" pt 5 t 'DP (OMP)',\
    '' i 0 u (log($1)/log(2)):4 w lp lc rgb "red" dashtype 2 pt 7 t 'SP (GPU)',\
    '' i 0 u (log($1)/log(2)):5 w lp lc rgb "blue" dashtype 2 pt 7 t 'DP (GPU)',\
    
