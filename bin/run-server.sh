controller_IP="192.168.1.11"
username="nick"
password="password"
interface="ens3"

delay_before_start_controller=30
delay_after_start_controller=60

no_host=1000
sent_long=60

no_capture_package=-1

#number of experiments repeat
# for repeat_no in 01 02 03
for repeat_no in 01 02
do

	# send rate (flow per sec)
	# for send_rate in 010 020 030 040 050 060 070 080 090 100
	for send_rate in 100 200 300 400 500 600 700 800 900 1000
	do
		# number of switch
		# for no_switch in 010 020 030 040 050 060 070 080 090 100
		for no_switch in 100 200 300 400 500
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
			sudo sshpass -p $password ssh $username@$controller_IP './start-controller.sh' &
			
			echo "wait for start topology"
			for delay in $(seq 1 $delay_after_start_controller)
			do
				echo -n "$(( $delay_after_start_controller - $delay )) \r"
				sleep 1s
			done
			echo ""
			
			sudo sshpass -p $password ssh $username@$controller_IP './start-cpu-mem-capture.sh' &
			
			sudo python tree.py $no_switch $no_host $interface $no_capture_package $send_rate $sent_long $controller_IP ../results/tree-$no_switch-sw-$send_rate-ps-$repeat_no
			sudo sshpass -p $password scp $username@$controller_IP:cpu-mem-uses.txt ../results/tree-$no_switch-sw-$send_rate-ps-$repeat_no
		done
	done
done
