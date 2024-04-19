#! /bin/bash
#args: <n_rounds>

n_rounds=$1

for conf_file in conf_*; do
    sed -i "s/\(pim_iter_limit:\).*/\1 $n_rounds/" "$conf_file"
done

