for i in 050 100 150 200
do
	for j in 050 100 150 200
	do
		for k in 02
		do
			echo "./calculateResult result/tree-$i-sw-$j-ps-$k/"
			./calculateResult result/tree-$i-sw-$j-ps-$k/
		done
		echo ""
	done
done
