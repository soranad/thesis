#number of experiments repeat
for repeat_no in 01
do
	# send rate (flow per sec)
	for send_rate in 100 200 300 400 500
	do
		# number of switch
		for no_switch in 050 100 150 200 250 300 350 400
		do
			# echo "./calculateResult ../results/tree-$no_switch-sw-$send_rate-ps-$repeat_no/"
			# ./calculateResult ../results/tree-$no_switch-sw-$send_rate-ps-$repeat_no/
			echo "./calculateResult ../results/tree-$no_switch-sw-$send_rate-ps-$repeat_no/"
			./calculateResult ../results/tree-$no_switch-sw-$send_rate-ps-$repeat_no/
		done
		echo ""
	done
done
