import sys, os
import numpy as np
import json

MSS = 1460 #In bytes
OVERHEAD_BYTES = 40

MTU = MSS + OVERHEAD_BYTES

BDP = (72.5e3 / MTU) * MSS #~71.5KB

file_path = sys.argv[1]

slowdown_histogram = dict()

with open(file_path,"r") as file:
    for line in file:
        flow_data = line.split(" ")
        try:
            int(flow_data[0]) #Try parsing the value as int
        except ValueError:
            continue
        flow_size = float(flow_data[1]) / BDP #In BDPs
        slowdown = float(flow_data[8]) #In us
        
        bdp_bin = 0
        index = 0
        
        while(flow_size > 0):
            flow_size = flow_size // 2
            bdp_bin = 2**index
            index += 1

        if(bdp_bin in slowdown_histogram.keys()):
           slowdown_histogram[bdp_bin].append(slowdown)
        else:
           slowdown_histogram[bdp_bin] = [slowdown,]

slowdown_mean_data = dict()

for bdp_bin, slowdown_list in slowdown_histogram.items():
    slowdown_list.sort()
    percentile_99 = np.percentile(slowdown_list, 99)
    mean = sum(slowdown_list) / len(slowdown_list)

    slowdown_mean_data[bdp_bin] = (mean, percentile_99)

dir_path = os.path.dirname(file_path)

with open(f'{dir_path}/data.json', "w") as json_file:
    json.dump(slowdown_mean_data, json_file)




