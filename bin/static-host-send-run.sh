controller_IP="192.168.1.11"
username="nick"
password="password"
interface="enp7s0"

delay_before_start_controller=60
delay_after_start_controller=120
delay_after_stop_controller=30

no_host=2000
sent_long=30

no_capture_package=-1

# rm -rf ../results/

#number of experiments repeat
# for repeat_no in 01 02 03
for repeat_no in 01
do

	# number of switch
	# for no_switch in 010 020 030
	for no_switch in 050 100 150 200
	do
	
		# send rate (flow per sec)
		for send_rate in 025 050 075 100 125 150 175 200 225 250 275 300 325 350 375 400
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

			sudo sshpass -p $password ssh $username@$controller_IP 'rm cpu-mem-uses.txt'
			sudo sshpass -p $password ssh $username@$controller_IP './start-controller.sh' &
			sudo sshpass -p $password ssh $username@$controller_IP './start-cpu-mem-capture.sh' &
			
			echo "wait for start topology"
			for delay in $(seq 1 $delay_after_start_controller)
			do
				echo -n "$(( $delay_after_start_controller - $delay )) \r"
				sleep 1s
			done
			echo ""
			
			# sudo sshpass -p $password ssh $username@$controller_IP './start-cpu-mem-capture.sh' &
			
			sudo python tree-static-host-send.py $no_switch $no_host $interface $no_capture_package $send_rate $sent_long $controller_IP ../results/tree-$no_switch-sw-$send_rate-ps-$repeat_no
			sudo sshpass -p $password scp $username@$controller_IP:cpu-mem-uses.txt ../results/tree-$no_switch-sw-$send_rate-ps-$repeat_no

			sudo sshpass -p $password ssh $username@$controller_IP './stop-controller.sh' &

			echo "" >> ../results/result.txt
			echo "----- tree-$no_switch-sw-$send_rate-ps-$repeat_no/ -----" >> ../results/result.txt
			./calculateResult ../results/tree-$no_switch-sw-$send_rate-ps-$repeat_no/ >> ../results/result.txt

			echo "wait for stop controller"
			for delay in $(seq 1 $delay_after_stop_controller)
			do
				echo -n "$(( $delay_after_stop_controller - $delay )) \r"
				sleep 1s
			done
			echo ""
		done
	done
done

foldername=$(date '+result-%y-%m-%d-%H-%M')
mv ../results/ ../$foldername
zip -r ../$foldername.zip ../$foldername
rm -rf ../$foldername