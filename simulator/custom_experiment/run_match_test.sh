#! /bin/bash

exp_type="d"

function set_d () {
    cwd=$(pwd)
    cd "conf_files/pim/flow_type_$exp_type" && ./set_alpha.sh "$1"
    cd "$cwd"
}

function set_alpha () {
    cwd=$(pwd)
    cd "conf_files/pim/flow_type_$exp_type" && ./set_d.sh "$1"
    cd "$cwd"
}

conf_file="conf_files/pim/flow_type_$exp_type/conf_file.txt"
output_dir="results/pim/flow_type_$exp_type/"

if [[ -d "$output_dir" ]]; then
    mkdir -p "$output_dir"
fi

for d in $(seq 1 1 143); do
    for alpha in $(seq 0 -0.5 5); do
        set_d "$d"
        set_alpha "$alpha"
        python run_configurations.py "$conf_file" "$output_dir"
    done
done


