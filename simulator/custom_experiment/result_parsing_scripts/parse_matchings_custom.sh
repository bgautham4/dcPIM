#! /bin/bash

#args : <var_param>

protocol='pim'
exp_type='custom'
var_param=$1

op_dir="../results/$protocol/flow_type_$exp_type"

if [[ ! -f "$op_dir/match_custom.csv" ]]; then
    > "$op_dir/match_custom.csv"
fi

> "$op_dir/temp.txt"

for result_file in "$op_dir/out_"*; do        
    match_value=$(grep "AVG_MATCHES" $result_file | cut -d' ' -f2)
    echo "$match_value" >> "$op_dir/temp.txt"
done

match_avg=$(awk 'BEGIN{s=0;}{s+=$1;}END{print s/NR}' "$op_dir/temp.txt")

if [[ -z "$var_param" ]]; then
    echo "0,$match_avg" >> "$op_dir/match_custom.csv"
else
    echo "$var_param,$match_avg" >> "$op_dir/match_custom.csv"
fi

rm "$op_dir/temp.txt"

echo "Parsed files for var_param = $var_param"

