controller_IP="127.0.0.1"
username="nick"
password="password"
interface="lo"

delay_before_start_controller=3
delay_after_start_controller=3

no_host=50
sent_long=10

no_capture_package=-1

# send rate (flow per sec)
for send_rate in 10 20
do
	# number of switch
	for no_switch in 25 50
	do
		#number of experiments repeat
		for repeat_no in 01 02
		do
			sudo mn -c
			sudo rm -rf ../results/tree-$no_switch-sw-$send_rate-ps-$repeat_no/
			sudo mkdir -p ../results/tree-$no_switch-sw-$send_rate-ps-$repeat_no/ping

			sudo sshpass -p $password ssh $username@$controller_IP 'reboot'
			
			echo "wait for start controller"
			for delay in $(seq 1 $delay_before_start_controller)
			do
				echo -n "$(( $delay_before_start_controller - $delay )) \r"
				sleep 1s
			done
			echo ""

			sudo sshpass -p $password ssh $username@$controller_IP 'rm cpu-mem-uses.txt' &
			sudo sshpass -p $password ssh $username@$controller_IP './start-cpu-mem-capture.sh' &
			sudo sshpass -p $password ssh $username@$controller_IP './start-controller.sh' &
			
			echo "wait for start topology"
			for delay in $(seq 1 $delay_after_start_controller)
			do
				echo -n "$(( $delay_after_start_controller - $delay )) \r"
				sleep 1s
			done
			echo ""
			
			sudo python tree.py $no_switch $no_host $interface $no_capture_package $send_rate $sent_long $controller_IP ../results/tree-$no_switch-sw-$send_rate-ps-$repeat_no
			sudo sshpass -p $password scp $username@$controller_IP:cpu-mem-uses.txt ../results/tree-$no_switch-sw-$send_rate-ps-$repeat_no
		done
	done
done