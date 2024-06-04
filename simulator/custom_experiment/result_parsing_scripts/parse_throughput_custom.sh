#! /bin/bash

#Arguments: <var_param>

protocol='pim'
exp_type='custom'
var_param="$1"

op_dir="../results/$protocol/flow_type_$exp_type"

if [[ ! -f "$op_dir/throughput.csv" ]]; then
    > "$op_dir/throughput.csv"
fi

function parse_throughput() {
    tpt="$(python3 parse_throughput.py "$1")"
}

> "$op_dir/temp.txt"

for result_file in "$op_dir/out_"*; do
    parse_throughput "$result_file" 
    echo "$tpt" >> "$op_dir/temp.txt"
done

long_tpt_avg=$(awk 'BEGIN{s=0;}{s+=$1;}END{print s/NR}' "$op_dir/temp.txt")
short_tpt_avg=$(awk 'BEGIN{s=0;}{s+=$2;}END{print s/NR}' "$op_dir/temp.txt")

if [[ -z "$var_param" ]]; then
    echo "0,$long_tpt_avg,$short_tpt_avg" >> "$op_dir/throughput.csv"
else
    echo "$var_param,$long_tpt_avg,$short_tpt_avg" >> "$op_dir/throughput.csv"
fi

rm "$op_dir/temp.txt"

echo "Parsed files for var_param = $var_param"
