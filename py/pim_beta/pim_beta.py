conf_str_pim = '''init_cwnd: 2
max_cwnd: 6
retx_timeout: 9.50003e-06
queue_size: 36864
propagation_delay: 0.0000002
bandwidth: 10000000000.0
queue_type: 2
flow_type: 116
num_flow: 1000000
flow_trace: ../CDF_{0}.txt
cut_through: 0
mean_flow_size: 0
load_balancing: 0
preemptive_queue: 0
big_switch: 0
host_type: 17
traffic_imbalance: 0
load: 0.7
reauth_limit: 3
magic_trans_slack: 1.1
magic_delay_scheduling: 1
use_flow_trace: 0
smooth_cdf: 1
burst_at_beginning: 0
pim_iter_limit: 5
pim_beta: {1}
pim_alpha: 1
token_initial: 1
token_timeout: 2
token_resend_timeout: 1
token_window: 1
token_window_timeout: 1.1
pim_select_min_iters: 1
ddc: 0
ddc_cpu_ratio: 0.33
ddc_mem_ratio: 0.33
ddc_disk_ratio: 0.34
ddc_normalize: 2
ddc_type: 0
deadline: 0
schedule_by_deadline: 0
avg_deadline: 0.0001
magic_inflate: 1
interarrival_cdf: none
num_host_types: 13
'''

runs = ["pim"]
workloads = ['aditya', 'dctcp', 'datamining', 'constant']
#incasts = [1,143]
for i in range(11):
    for w in workloads:
        #  generate conf file
        conf_str = conf_str_pim.format(w, i)
        confFile = "conf_{0}_{1}.txt".format(w,i)
        with open(confFile, 'w') as f:
            print confFile
            f.write(conf_str)
