#! /bin/bash

#Arguments <protocol> <exp_type>
protocol=$1
exp_type=$2

if [[ ! -d "bin" ]]; then
    echo "A virtual python env does not exist in the directory!"
    echo "Create a virtual environment and install numpy before running script"
    exit 1
fi

source bin/activate #Activate the virtual env
result_dir="../results/$protocol/flow_type_$exp_type"
#=for result_dir in "../results/$protocol/flow_type_$exp_type/"*/; do
#=    data_file=$(ls | grep "data")
#=    if [[ ! -z $data_file ]]; then
#=        continue
#=    fi

    for util_file in "$result_dir/util_"*; do
        variable_parameter=$(basename -s .txt $util_file | cut -d'_' -f2)
        util=$(python3 parse_util.py $util_file)
        echo "$variable_parameter,$util" >> "$result_dir/util.csv"
        echo "Parsed $util_file.."
    done
#=done

deactivate #Exit virtual env




