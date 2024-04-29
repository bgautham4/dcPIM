#! /bin/bash

#args : <protocol> <exp_type> <n_rounds>

protocol=$1
exp_type=$2
n_rounds=$3

op_dir="../results/$protocol/flow_type_$exp_type"

if [[ ! -f "$op_dir/match.csv" ]]; then
    > "$op_dir/match.csv"
fi

for result_file in "$op_dir/flow_"*; do    
    load=$(basename -s .txt $result_file | cut -d'_' -f3)
    match_value=$(grep "AVG_MATCHES" $result_file | cut -d' ' -f2)
    echo "$n_rounds,$load,$match_value" >> "$op_dir/match.csv"
done

echo "Parsed files for r = $n_rounds"

