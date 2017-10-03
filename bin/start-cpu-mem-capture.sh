echo "start cpu mem moniter"
while true
do
	ps -C java -o "comm %cpu %mem" --no-headers | awk '{a[$1] = $1; b[$1] += $2; c[$1] += $3}END{for (i in a)printf "%s, %0.1f, %0.1f\n", a[i], b[i], c[i]}' >> cpu-mem-uses.txt
	sleep 1
done
