import itertools
import numpy as np
import time, sys

for i in range(30) :
	sys.stdout.write("\r" + str(30-i))
	sys.stdout.flush()
	time.sleep(1)

# print list(itertools.combinations((1, 2, 3), 2))