#number of experiments repeat
for repeat_no in 01
do

	# send rate (flow per sec)
	for send_rate in 050 100 150 200 250 300
	do
		# number of switch
		for no_switch in 025 050 075 100
		do
			echo "./calculateResult-2 ../result-17-10-03-02/tree-$no_switch-sw-$send_rate-ps-$repeat_no/"
			./calculateResult-2 ../result-17-10-03-02/tree-$no_switch-sw-$send_rate-ps-$repeat_no/
		done
		echo ""
	done
done
