#!/bin/bash

for (( i=1; i<=10; i++ ))
do
	mv "e_bbc_$i.txt" "e_bbc_$i.html"
	mv "e_epoca_negocios_$i.txt" "e_epoca_negocios_$i.html"
	mv "e_exame_$i.txt" "e_exame_$i.html"
	mv "e_noticias_minuto_$i.txt" "e_noticias_minuto_$i.html"
	mv "e_terra_$i.txt" "e_terra_$i.html"
	mv "e_uol_$i.txt" "e_uol_$i.html"
	mv "r_acidade_$i.txt" "r_acidade_$i.html"
	mv "r_diariosp_$i.txt" "r_diariosp_$i.html"
	mv "r_dpernambuco_$i.txt" "r_dpernambuco_$i.html"
	mv "r_g1_$i.txt" "r_g1_$i.html"
	mv "r_jb_$i.txt" "r_jb_$i.html"
	mv "r_migalhas_$i.txt" "r_migalhas_$i.html"
	mv "r_oglobo_$i.txt" "r_oglobo_$i.html"
	mv "r_opovo_$i.txt" "r_opovo_$i.html"
	mv "r_zerohora_$i.txt" "r_zerohora_$i.html"
done
