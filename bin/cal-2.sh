#number of experiments repeat
for repeat_no in 01
do

	# send rate (flow per sec)
	for send_rate in 100 200
	do
		# number of switch
		for no_switch in 50
		do
			echo "./calculateResult-2 ../result/tree-$no_switch-sw-$send_rate-ps-$repeat_no/"
			./calculateResult-2 ../result/tree-$no_switch-sw-$send_rate-ps-$repeat_no/
		done
		echo ""
	done
done
