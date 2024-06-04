#! /bin/bash

#args : <protocol> <exp_type> <var_param>

protocol=$1
exp_type=$2
var_param=$3

op_dir="../results/$protocol/flow_type_$exp_type"

if [[ ! -f "$op_dir/match.csv" ]]; then
    > "$op_dir/match.csv"
fi

for result_file in "$op_dir/flow_"*; do    
    load=$(basename -s .txt $result_file | cut -d'_' -f3)
    
    match_value=$(grep "AVG_MATCHES" $result_file | cut -d' ' -f2)

    if [[ -z "$var_param" ]]; then
        echo "$load,$match_value" >> "$op_dir/match.csv"
    else
        echo "$var_param,$load,$match_value" >> "$op_dir/match.csv"
    fi

done

echo "Parsed files for var_param = $var_param"

