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

for result_dir in "../results/$protocol/flow_type_$exp_type/"*/; do
    data_file=$(ls | grep "data")
    if [[ ! -z $data_file ]]; then
        continue
    fi

    for result_file in "$result_dir/"*; do
        variable_parameter=$(basename -s .txt $result_file | cut -d'_' -f3)
        python3 parse_slowdown.py $result_file #Create the data.json file
        mv "$result_dir/data.json" "$result_dir/data_$variable_parameter.json"
        echo "Parsed $result_file.."
    done
done

deactivate #Exit virtual env




