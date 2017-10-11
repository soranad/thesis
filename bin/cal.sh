#number of experiments repeat
for repeat_no in 01
do
	# send rate (flow per sec)
	for send_rate in 020 040 060 080 100 120 140 160 180 200
	do
		# number of switch
		for no_switch in 025 050
		do
			# echo "./calculateResult ../results/tree-$no_switch-sw-$send_rate-ps-$repeat_no/"
			# ./calculateResult ../results/tree-$no_switch-sw-$send_rate-ps-$repeat_no/
			echo "./calculateResult ../results/tree-$no_switch-sw-$send_rate-ps-$repeat_no/"
			./calculateResult ../results/tree-$no_switch-sw-$send_rate-ps-$repeat_no/
		done
		echo ""
	done
done
