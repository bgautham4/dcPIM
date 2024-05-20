#! /bin/bash

alpha=$1

sed -i "s/\(^alpha:\).*/\1 $alpha/" conf_file.txt
