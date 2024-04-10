#! /bin/bash

#argmuents: <protocol> <exp_type> 

timestamp=$(date +'%m-%dT%H-%M-%S')
protocol=$1
exp_type=$2

#output_dir="results/$protocol/flow_type_$exp_type/$timestamp"

output_dir="results/$protocol/flow_type_$exp_type/"

if [[ ! -e $output_dir ]]; then
        mkdir -p $output_dir
fi
   
for conf_file in "conf_files/$protocol/flow_type_$exp_type"/*; do
    
    variable_parameter=$(basename -s .txt "$conf_file" | cut -d'_' -f3)
    
    ../simulator 1 $conf_file > "$output_dir/flow_details_$variable_parameter.txt" &

done

echo "Experiment $exp_type : $timestamp started..."

wait 

echo "Experiment $exp_type : $timestamp completed."


