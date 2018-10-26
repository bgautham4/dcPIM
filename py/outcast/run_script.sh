#!/bin/bash

outcasts=(1 2 4 9 18 36 72 143)
algos=(pfabric phost random)

OUTPUT_FOLDER=../result/outcast
DATE=$1
mkdir $OUTPUT_FOLDER
mkdir $OUTPUT_FOLDER/"$DATE"
for i in ${!algos[*]}
do 
	for index in ${!outcasts[*]};
	do
	    outcast=${outcasts[$index]}
	    algo=${algos[$i]}
	    # echo conf_"$algo"_dctcp_$incast.txt
	    # echo "$OUTPUT_FOLDER"/result_"$algo"_dctcp_"$outcast".txt
	    ../../simulator 1 conf_"$algo"_datamining_$outcast.txt > "$OUTPUT_FOLDER/$DATE"/result_"$algo"_datamining_"$outcast".txt 
	#	nohup ./batch_simulate_sflow.py -P $p -F ../../../data/ -t ${threshold[$index]} -i 10 -N 1000 -s 1 -l results/conext18/flows/percentage-${percentage[$index]}.log &
	done
done
