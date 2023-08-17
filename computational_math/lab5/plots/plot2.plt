#!/usr/bin/gnuplot

set termoption enhanced
set terminal svg size 1400,900 font "Arial, 16"
set output "func2.svg"
set style line 1 lc rgb "0xDC143C" lt 1 lw 4 pt 9 ps 1
set border lw 2
set grid
set key top left
set xlabel "x"
set ylabel "y" rotate by 90
set mxtics
plot "rezult.txt" using 1:2 title "func(x, y)" with linespoints ls 1