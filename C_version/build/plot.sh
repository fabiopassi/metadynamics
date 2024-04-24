#!/bin/bash

#		SCRIPT TO PLOT THE RESULTS OF THE METADYNAMICS SIMULATION		#

# Check for gnuplot
command=$(which gnuplot)

if [ command == "" ]; then
	echo -e "Error: gnuplot not installed\n"
fi

# Plot
gnuplot_filename="dynamic_plot.gp"
if [ ! -f ${gnuplot_filename} ]; then
	touch $gnuplot_filename
fi

echo "set xlabel \"x\"
set ylabel \"U(x)\"
set title \"Metadynamics\"

i=0

unset key
" > $gnuplot_filename

end_idx=$(wc -l metadyn_pot.txt | awk '{print $1}')


echo "plot \"ref_pot.txt\" w l lt 1 lw 2
do for [i=0:${end_idx}-1]{
        replot \"metadyn_pot.txt\" every ::i::(i+1) w p ps 2 pt 7 lc rgb \"#a83655\"
}

pause -1

" >> $gnuplot_filename

gnuplot $gnuplot_filename

rm $gnuplot_filename
