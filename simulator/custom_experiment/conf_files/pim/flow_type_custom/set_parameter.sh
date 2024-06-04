param=$1
value=$2

sed -i "s/\(^$param:\).*/\1 $value/" conf_file.txt
