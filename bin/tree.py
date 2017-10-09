from mininet.net import Mininet
from mininet.node import Controller, OVSSwitch, RemoteController
from mininet.cli import CLI
from mininet.log import setLogLevel, info

import sys, getopt
import subprocess

import random
import numpy as np
import itertools
import os

from time import sleep

def multiControllerNet():

	info( "*** Creating (reference) controllers" )

	net = Mininet(controller=RemoteController, switch=OVSSwitch)
	controllers = [None] * (len(sys.argv)-8)
	for i in range(len(controllers)):
		print "\n remote controller : " + sys.argv[i+7]
		controllers[i] = net.addController('c'+str(i), controller=RemoteController, ip=sys.argv[i+7], port=6633)


	info( "\n\n*** Creating switches\n" )

	numberOfSwitch = int(sys.argv[1])
	switchs = [None] * numberOfSwitch
	for i in range(numberOfSwitch):
		switchs[i] = net.addSwitch( 's'+str(i) )
		info('s'+str(i)+' ')
	

	info( "\n\n*** Creating hosts\n" )

	numberOfLeave = (numberOfSwitch+1)/2
	numberOfHost = int(sys.argv[2])
	hosts = [None] * numberOfHost
	for i in range(numberOfHost):
		info('h'+str(i)+' ')
		hosts[i] = net.addHost( 'h'+str(i) )


	info( "\n\n*** Creating links\n" )

	for i in range(1, numberOfSwitch):
		net.addLink( switchs[i], switchs[(i-1)/2] )
		info('(s'+str(i)+',s'+str((i-1)/2)+') ')
	info( "\n" )
	leaveNumber = 0;
	startLeave = numberOfSwitch-numberOfLeave
	for i in range (numberOfHost):
		switchs[leaveNumber+startLeave].linkTo(hosts[i])
		info('(s'+str(leaveNumber+startLeave)+',h'+str(i)+') ')
		leaveNumber = leaveNumber+1
		if leaveNumber >= numberOfLeave :
			leaveNumber = 0


	info( "\n\n*** Starting network\n\n" )

	net.build()
	
	for controller in controllers:
		controller.start()
	for i in range(numberOfSwitch):
		switchs[i].start(controllers)
		info( "s%d "% (i))


	info( "\n\n*** Testing network\n\n" )
	
	net.start()

	directoryName = sys.argv[len(sys.argv)-1];
	directoryName = directoryName + "/ping"

	cmd = ['./openflow-sniffex', sys.argv[3], sys.argv[4], sys.argv[len(sys.argv)-1]+"/cap.csv"]
	process = subprocess.Popen(cmd)
	# process = subprocess.Popen(cmd, stdout=subprocess.PIPE)


	# info( "\n\n*** Start iperf Server\n" )
	# for i in range(numberOfHost) :
	# 	hosts[i].cmd("iperf -s &")

	
	info( "\n\n*** gennerate command\n" )

	sendRate = int(sys.argv[5])
	time = int(sys.argv[6])
	numberOfSends = sendRate*time
	beta = 1.0/sendRate
	Y = np.random.exponential(beta, numberOfSends)

	sendHosts = [None] * numberOfSends
	cmds = [None] * numberOfSends
	
	info( "\n\n*** random cmd send package \n" )
	print numberOfHost
	randHost = list(itertools.combinations(hosts, 2))
	random.shuffle(randHost)
	print len(randHost)
	
	for i in range(numberOfSends) :
		cmds[i] = "python ping.py " + randHost[i][1].IP() + " " + randHost[i][0].IP() + " >> "+directoryName+"/"+str(i).zfill(6)+".csv &"
		# cmds[i] = "ping -c 1 -W 100 " + randHost[i][1].IP() + " >> "+directoryName+"/"+str(i).zfill(6)+".txt &"
		# cmds[i] = "iperf -c " + randHost[1].IP() + " -n 10000000 &"

	# sleep(120)
	for i in range(115) :
		sys.stdout.write("\r" + str(120-i) + " ")
		sys.stdout.flush()
		sleep(1)

	cmd2 = ["sshpass", "-p", "password", "ssh", "nick@192.168.1.11", "'./start-cpu-mem-capture.sh' &"]
	process2 = subprocess.Popen(cmd2)

	for i in range(5) :
		sys.stdout.write("\r" + str(5-i) + " ")
		sys.stdout.flush()
		sleep(1)

	info( "\n\n*** Start iperf client\n" )
	for i in range(numberOfSends) :
		randHost[i][0].cmd(cmds[i])
		sleep(Y[i])

	# sleep(120)
	for i in range(120) :
		sys.stdout.write("\r" + str(120-i) + " ")
		sys.stdout.flush()
		sleep(1)
	
	# CLI( net )
	process.kill()
	net.stop()

if __name__ == '__main__':
	setLogLevel( 'info' )  # for CLI output
multiControllerNet()
