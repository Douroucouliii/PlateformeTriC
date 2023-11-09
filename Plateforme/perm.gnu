 set title 'Permutations des algorithmes de tris'
 set xlabel 'Taille de la liste'
 set ylabel 'Nombre de comparaisons'

 plot 'perm.dat' using 1:2 title 'Tri par insertion' with linespoints
 replot 'perm.dat' using 1:3 title 'Tri par sélection' with linespoints
 replot 'perm.dat' using 1:4 title 'Tri à bulles' with linespoints
 replot 'perm.dat' using 1:5 title 'Tri fusion' with linespoints
 replot 'perm.dat' using 1:6 title 'Tri rapide' with linespoints
 replot 'perm.dat' using 1:7 title 'Tri par tas' with linespoints

 pause -1
