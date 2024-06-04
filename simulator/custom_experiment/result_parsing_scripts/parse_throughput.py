from sys import argv

file_path = argv[1]

short_flow_size = 49 * 1460

with open(file_path, "r") as f:
    long_flow_bytes = 0
    short_flow_bytes = 0
    current_time = 0
    for line in f:
        data = line.split(" ")
        #Try parsing the first element as an int
                
        try:
            int(data[0])
        except ValueError:
            continue
        
        if (int(data[1]) > short_flow_size):
            long_flow_bytes += int(data[1])
        else:
            short_flow_bytes += int(data[1])

        current_time = (float(data[5]) * 1e-6) - 1
    long_flow_tpt = (long_flow_bytes / current_time) / 8 * 1e-9 #In Gpbs
    short_flow_tpt = (short_flow_bytes / current_time) / 8 * 1e-9 #In Gpbs

    print(f'{long_flow_tpt} {short_flow_tpt}')

