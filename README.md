# dcPIM DPDK Implementation

## Setup

### Install DPDK

1. Download the dpdk 18.11.10.

```
wget https://fast.dpdk.org/rel/dpdk-18.11.10.tar.xz
tar -xf dpdk-18.11.10.tar.xz
rm dpdk-18.11.10.tar.xz
```

2. Build DPDK with MLX4 PMD (assuming using MLX ConnectX-3). If you are using Intel NIC

 ```
 cd dpdk-stable-18.11.10
 export RTE_SDK=$PWD
 export RTE_TARGET=x86_64-native-linuxapp-gcc
 make config T=$RTE_TARGET O=$RTE_TARGET
 cd $RTE_TARGET
 ```
 
 If you use Mellanox (eg.ConnectX-3) NIC, open .config and set `CONFIG_RTE_LIBRTE_MLX4_PMD=y`.
 Then make
 
 ```
 make -j
 ```
### Configure IP address and enable hugepages
Change the dpdk src directory in run.sh to `/path/to/dpdk` in your setup.

```
line 17: export RTE_SDK=/path/to/dpdk
```

Then

```
sudo ./run.sh $num_server
```

`$num_server` is the number of servers in the testbed.

## Run experiments

Suppose we have n servers:
For first n-1 servers, run
```
./build/pim -- send CDF_$workload.txt > result_$workload.txt
```
For the last server:
```
./build/pim -- start CDF_$workload.txt > result_$workload.txt
```
The workloads that provided in this repo are imc10, websearch and datamining.

## SIGCOMM 2022 Artifact Evaluation

We conduct our experiment using the [m510](http://docs.cloudlab.us/hardware.html#%28part._cloudlab-utah%29) machines available at CloudLab.

1. Create the cloudlab [account](https://www.cloudlab.us/signup.php) and join existing project and the project name is dcpim.
2. Start the experiments using the [profile](https://www.cloudlab.us/p/688e66f95e2d89e11bd066a597588450beb87dc6).  
3. 

## Cornell Clusters

If you are using Cornell clusters, you need to do following changes:

1. change line 6 to the correct end host addresses in config.sh:
```
ping 10.10.1.$c  -w 5
```
The correct IP addresses are from (5.0.0.10 to 12.0.0.10) depending on the number of servers you are using.

2. Also, you need to change config.py line 7 for the correct IP prefix/surfix. The current is for cloudlab clusters.

3. Copy the src/main.c from the main branch to src/main.c in this branch. Then, changes follwing to line:
```
line 273: pim_receive_start(&epoch, &host, &pacer, 3);
line 702: rte_eal_remote_launch(launch_host_lcore, NULL, 3); 
line 717: if(rte_eal_wait_lcore(3) < 0)
```
to 
```
line 273: pim_receive_start(&epoch, &host, &pacer, RECEIVE_CORE);
line 702: rte_eal_remote_launch(launch_host_lcore, NULL, RECEIVE_CORE); 
line 717: if(rte_eal_wait_lcore(RECEIVE_CORE) < 0)
```


