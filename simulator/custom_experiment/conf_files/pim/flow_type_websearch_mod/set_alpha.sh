#! /bin/bash

alpha=$1

for conf_file in *; do
                sed -i "s/\(alpha:\)\s-[0-9]\+/\1 $alpha/" $conf_file
done

