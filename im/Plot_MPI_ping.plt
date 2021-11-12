set term pdfcairo enhanced dashed font 'Helvetica,24' size 20cm,12cm dashed
set output "result_MPI_ping.pdf"

set xlabel "Amount of data (Kib)"
set logscale x
set ylabel "Bandwidth (Gib/s)"

plot 'Data_MPI_ping.csv' u ($1/1024):($2) w lp pt 5 lc rgb "red"  t 'Two separate nodes',\
'' u ($1/1024):($3) w lp pt 5 lc rgb "blue"  t 'My computer'
