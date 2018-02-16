import re
import sys
import subprocess

hostname = "127.0.0.1"
if len(sys.argv) > 2 :
	hostname = sys.argv[2]

try:
	ping = subprocess.Popen(["ping", "-c 1 -W 120", hostname], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	out, error = ping.communicate()
	if out:
		try:
			time = re.findall(r"mdev = (\d+\.\d+)", out)[0]
			print "%s,%s,%s ms, 1" % (sys.argv[1], sys.argv[2], time)
		except:
			print "%s,%s,%f ms, 0" % (sys.argv[1], sys.argv[2], -1)
	else:
		print "%s,%s,%f ms, 2" % (sys.argv[1], sys.argv[2], -1)
except subprocess.CalledProcessError:
	print "%s,%s,%f ms, 3" % (sys.argv[1], sys.argv[2], -1)