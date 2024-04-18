#! /bin/bash

alpha=$1

for conf_file in *; do
    sed -i "s/\(alpha:\).*/\1 $alpha/" $conf_file
    sed -i "s/\(pim_alpha:\).*/\1 1/" $conf_file
done

