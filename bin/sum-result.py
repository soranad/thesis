from os import listdir,remove
from os.path import isfile, join
import sys

onlyfiles = listdir(sys.argv[1]+"ping")
path = sys.argv[1]+"ping.csv"
# remove(path)
wfile = open(path,"w") 

for i in onlyfiles :
	path2 = sys.argv[1]+"ping/"+i
	rfile = open(path2,"r") 
	wfile.write(i+","+rfile.readline())
	rfile.close()
	remove(path2)
wfile.close() 