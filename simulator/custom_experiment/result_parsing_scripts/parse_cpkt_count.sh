#! /bin/bash

#args : <protocol> <exp_type> <var_param>

protocol=$1
exp_type=$2
var_param=$3

op_dir="../results/$protocol/flow_type_$exp_type"

if [[ ! -f "$op_dir/cpkt_count.csv" ]]; then
    > "$op_dir/cpkt_count.csv"
fi

for result_file in "$op_dir/cpf_"*; do    
    load=$(basename -s .txt $result_file | cut -d'_' -f2)
    count=$(tail -n 1 $result_file | cut -d' ' -f2)

    if [[ -z "$var_param" ]]; then
        echo "$load,$count" >> "$op_dir/cpkt_count.csv"
    else
        echo "$var_param,$load,$count" >> "$op_dir/cpkt_count.csv"
    fi

done

echo "Parsed files for var_param = $var_param"

