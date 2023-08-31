TASKSET="0,4,8,12,16,20,24,28,32,36,40,44,48,52,56"
server=0
numiperf=15
SYS=$1
servers=$2
while (( server < servers ));do
	core=0
	iperf=0
	while (( iperf < numiperf ));do

        if [[ $SYS == "dcpim" ]]
        then
                sudo LD_PRELOAD=~/dcPIM/kernel_impl/custom_socket/socket_wrapper.so taskset -c $((core)) iperf3 -s -p $((4000 * (server + 1) + iperf))  > server_"$server"_"$iperf".log &
        else
                sudo taskset -c $((core)) iperf3 -s -p $((4000 * (server + 1) + iperf))  > server_"$server"_"$iperf".log &
        fi
        ((core = (core + 4) % 64))
        (( iperf++ ))
    done
    (( server++ ))
done