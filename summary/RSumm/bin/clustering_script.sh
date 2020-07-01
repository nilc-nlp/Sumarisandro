#!/bin/bash

dir="/home/ribaldo/Desktop/AgrupamentoDaRst/C"
dir2="/home/ribaldo/Desktop/AgrupamentoDaRst/Agrupamento/C"

for (( i=1; i<=1; i++ ))
do
	echo $i
	files=$dir$i"/*"
	cmd="./RSumm -segmentado "

	for f in $files
	do
		cmd=$cmd$f" "
	done

	#cmd=$cmd"0.70 -cst ../CST_relations/relacoes_$i -ponderada"
	cmd=$cmd"0.70"

	$cmd
	sleep 3
	mv "agrupamento.txt" "$dir2$i""/C"$i"_agrupamento.txt"
#	sleep 1
#	mv "summary.segmentado" "$dir$i""/C"$i".segmentado"
	sleep 1
done

