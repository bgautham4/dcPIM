#! /bin/bash

#argmuents: <protocol> <exp_type> 

protocol=$1
exp_type=$2


conf_dir="conf_files/$protocol/flow_type_$exp_type"
output_dir="results/$protocol/flow_type_$exp_type"
scripts_dir="result_parsing_scripts"

function set_alpha() {
    local cwd=$(pwd)
    cd "$conf_dir" && ./set_alpha.sh "$1"
    cd "$cwd"
}

function set_d() {
    local cwd=$(pwd)
    cd "$conf_dir" && ./set_thin_to.sh "$1"
    cd "$cwd"
}

function parse_results() {
    local cwd=$(pwd)
    cd "$scripts_dir"
    ./parse_cpkt_count.sh "$protocol" "$exp_type" "$1"
    ./parse_matchings.sh "$protocol" "$exp_type" "$1"
    ./parse_util.sh "$protocol" "$exp_type" "$1"
    cd "$cwd"
}

if [[ ! -e "$output_dir" ]]; then
    mkdir -p "$output_dir"
fi

> "$output_dir/match.csv"
> "$output_dir/util.csv"
> "$output_dir/cpkt_count.csv"

thin_counts=(2 3 4 5)
alphas=(-3.5 -3 -2.5 -2)
N=${#thin_counts[@]}

for (( idx = 0; idx < N; idx++)); do
    set_d "${thin_counts[idx]}"
    set_alpha "${alphas[idx]}"
    ./run_experiment_protocol_type.sh "$protocol" "$exp_type"
    echo "Finished exp for d = ${thin_counts[idx]}"
    #Parse the results:
    parse_results "${thin_counts[idx]}"
done
