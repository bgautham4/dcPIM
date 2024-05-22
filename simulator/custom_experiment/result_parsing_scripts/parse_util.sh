#! /bin/bash

#Arguments <protocol> <exp_type> <var_param_2>
protocol=$1
exp_type=$2
var_param_2=$3


#if [[ ! -d "bin" ]]; then
#    echo "A virtual python env does not exist in the directory!"
#    echo "Create a virtual environment and install numpy before running script"
#    exit 1
#fi

#source bin/activate #Activate the virtual env

result_dir="../results/$protocol/flow_type_$exp_type"

if [[ ! -f "$result_dir/util.csv" ]]; then
    > "$result_dir/util.csv"
fi


    for util_file in "$result_dir/util_"*; do
        variable_parameter=$(basename -s .txt $util_file | cut -d'_' -f2)
        #util=$(python3 parse_util.py $util_file)
        util=$(awk 'BEGIN{s=0;}{s+=$2;}END{print s/NR}' "$util_file")

        if [[ -z "$var_param_2" ]]; then
            echo "$variable_parameter,$util" >> "$result_dir/util.csv"
        else
            echo "$var_param_2,$variable_parameter,$util" >> "$result_dir/util.csv"
        fi
        echo "Parsed $util_file.."
    done


#deactivate #Exit virtual env




