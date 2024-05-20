#! /bin/bash
d=$1

sed -i "s/\(^d:\).*/\1 $1/" conf_file.txt

