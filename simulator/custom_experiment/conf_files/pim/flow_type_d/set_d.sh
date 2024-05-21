#! /bin/bash
d=$1

sed -i "s/\(^d:\).*/\1 $d/" conf_file.txt

