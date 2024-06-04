#! /bin/bash

conf_dir='conf_files/pim/flow_type_custom/'
output_dir='results/pim/flow_type_custom'
scripts_dir='result_parsing_scripts'

function set_parameter() {
    local cwd=$(pwd)
    cd "$conf_dir" && ./set_parameter.sh "$1" "$2"
    cd "$cwd"
}

function parse_results() {
    local cwd=$(pwd)
    cd "$scripts_dir"
    ./parse_matchings_custom.sh "$1"
    ./parse_throughput_custom.sh "$1"
    cd "$cwd"
}

if [[ ! -e "$output_dir" ]]; then
    mkdir -p "$output_dir"
fi

> "$output_dir/match_custom.csv"
> "$output_dir/throughput.csv"

rhos=(1 2 3 4 5)
alphas=(1 -4.5 -3.5 -3 -2.5)
N=${#rhos[@]}

for (( idx = 0; idx < N; idx++)); do
    set_parameter "rho" "${rhos[idx]}"
    set_parameter "alpha" "${alphas[idx]}"
    python3 run_configurations.py "$conf_dir/conf_file.txt" "$output_dir"
    #Parse the results:
    parse_results "${rhos[idx]}"
done
