#! /bin/bash

exp_type="d"

conf_dir="conf_files/pim/flow_type_$exp_type"
conf_file="$conf_dir/conf_file.txt"
output_dir="results/pim/flow_type_$exp_type/"

function set_d () {
    cwd=$(pwd)
    cd "$conf_dir" && ./set_alpha.sh "$1"
    cd "$cwd"
}

function set_alpha () {
    cwd=$(pwd)
    cd "$conf_dir" && ./set_d.sh "$1"
    cd "$cwd"
}

function find_average () {
    cwd=$(pwd)
    cd "$output_dir"
    touch temp.txt
    for res_file in "$output_dir/out_"*; do
        local match=$(grep "AVG_MATCHES" "$res_file" | cut -d' ' -f2)
        echo "$match" >> temp.txt
    done
    ret_val=$(awk 'BEGIN{s=0;}{s+=$1;}END{print s/NR;}' temp.txt)
    cd "$cwd"

}

if [[ -d "$output_dir" ]]; then
    mkdir -p "$output_dir"
fi

touch "$output_dir/match.csv"

output_file="$output_dir/match.csv"
printf 'd,' >> "$output_file"
seq -s , 0 -0.5 -5 >> "$output_file"
for d in $(seq 1 1 143); do
    printf '%d' "$d" >> "$output_file"
    for alpha in $(seq 0 -0.5 -5); do
        set_d "$d"
        set_alpha "$alpha"
        python run_configurations.py "$conf_file" "$output_dir" 
        #Parse matching results here
        find_average
        printf ',%f' "$ret_val" >> "$output_file"
    done
   printf '\n' >> "$output_file"
done


