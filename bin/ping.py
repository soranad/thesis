import sys
import time
import subprocess
import numpy as np

hostname = "127.0.0.1"
if len(sys.argv) > 2 :
	hostname = sys.argv[2]

start = time.time()
response = subprocess.call(["ping", "-c", "1", hostname], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
end = time.time()
count = 1
# response, err = p.communicate()
while response != 0 :
	response = subprocess.call(["ping", "-c", "1", hostname], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	end = time.time()
	count = count + 1
	# response, err = p.communicate()

print "%s,%s,%f ms, %d" % (sys.argv[1],sys.argv[2],(end-start)*1000) , count

# Y = np.random.exponential(1, 1)
# response = subprocess.call(["iperf", "-c", hostname, "-t", str(Y[0])], stdout=subprocess.PIPE, stderr=subprocess.PIPE)