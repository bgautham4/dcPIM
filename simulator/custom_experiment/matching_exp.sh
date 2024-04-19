#! /bin/bash

#argmuents: <protocol> <exp_type> 

protocol=$1
exp_type=$2

cwd=$PWD

output_dir="results/$protocol/flow_type_$exp_type/"

if [[ ! -e $output_dir ]]; then
        mkdir -p $output_dir
fi

rounds=(1 2 3 4)

for r in "${rounds[@]}"; do
    cd "conf_files/$protocol/flow_type_$exp_type/" && ./set_nrounds.sh "$r" 
    cd "$cwd"
    ./run_experiment_protocol_type.sh "$protocol" "$exp_type"
    echo "$exp_type ran with matching rounds = $r"

    #Parse the results:
    cd result_parsing_scripts/ && ./parse_matchings.sh "$protocol" "$exp_type" "$r"
    cd "$cwd"
done
