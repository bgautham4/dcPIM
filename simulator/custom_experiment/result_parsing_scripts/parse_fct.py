import sys, os

MSS = 1460 #In bytes
OVERHEAD_BYTES = 40

MTU = MSS + OVERHEAD_BYTES

BDP = (72.5e3 / MTU) * MSS #~71.5KB

file_path = sys.argv[1]

fct_histogram = dict()

with open(file_path,"r") as file:
    for line in file:
        flow_data = line.split(" ")
        try:
            int(flow_data[0]) #Try parsing the value as int
        except ValueError:
            continue
        flow_size = float(flow_data[1]) / BDP #In BDPs
        fct = float(flow_data[6]) #In us
        
        bdp_bin = 0
        index = 0
        
        while(flow_size > 0):
            flow_size = flow_size // 2
            bdp_bin = 2**index
            index += 1

        if(bdp_bin in fct_histogram.keys()):
           fct_histogram[bdp_bin].append(fct)
        else:
           fct_histogram[bdp_bin] = [fct,]

fct_mean_data = dict()

for bdp_bin, fct_list in fct_histogram.items():
    fct_list.sort()
    N = len(fct_list)
    n = int( (99.0/100.0) * (N-1) )
    mean = sum(fct_list) / len(fct_list)

    fct_mean_data[bdp_bin] = (mean, fct_list[n])

print(fct_mean_data)

