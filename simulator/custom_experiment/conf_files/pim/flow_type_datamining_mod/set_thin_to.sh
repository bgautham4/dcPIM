#! /bin/bash
d=$1

for conf_file in "conf_"*; do
    sed -i "s/\(^thin_to:\).*/\1 $d/" "$conf_file"
done

