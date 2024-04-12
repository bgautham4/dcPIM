import sys
import numpy as np

#Units are Gpbs

file_path = sys.argv[1]

util_list = []

with open(file_path, "r") as file:
    for line in file:
        line_data = line.split(" ")
        util_list.append(float(line_data[1]))

print(np.mean(util_list))
