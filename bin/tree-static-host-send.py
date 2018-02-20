from mininet.net import Mininet
from mininet.node import Controller, OVSKernelSwitch, RemoteController
from mininet.cli import CLI
from mininet.log import setLogLevel, info

import sys, getopt
import subprocess

import random
import numpy as np
import itertools
import os

from time import sleep

def emptyNet():

	net = Mininet(controller=RemoteController, switch=OVSKernelSwitch)


	info( "*** Creating (reference) controllers" )
	
	controllers = [None] * (len(sys.argv)-7)
	for i in range(len(controllers)):
		print "\n remote controller : " + sys.argv[i+6]
		controllers[i] = net.addController('c'+str(i), controller=RemoteController, ip=sys.argv[i+6], port=6633)

	
	info( "\n\n*** Creating switches\n" )
	
	numberOfSwitch = int(sys.argv[1])
	switchs = [None] * numberOfSwitch
	for i in range(numberOfSwitch):
		switchs[i] = net.addSwitch( 's'+str(i) )
		info('s'+str(i)+' ')

	
	info( "\n\n*** Creating hosts\n" )
	
	numberOfLeave = (numberOfSwitch+1)/2
	numberOfHost = int(sys.argv[2])
	numberOfSends = numberOfHost/2

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
	net.start()
	

	info( "\n\n*** gennerate send rate \n" )

	sendRate = int(sys.argv[5])
	beta = 1.0/sendRate
	Y = np.random.exponential(beta, numberOfSends)


	info( "\n\n*** gennerate random send package command \n" )

	directoryName = sys.argv[len(sys.argv)-1];
	directoryName = directoryName + "/ping"

	print numberOfHost
	randHost = hosts[:]
	random.shuffle(randHost)
	print len(randHost)

	cmds = [None] * numberOfSends
	for i in range(numberOfSends) :
		cmds[i] = "python ping.py " + randHost[i].IP() + " " + randHost[i+numberOfSends].IP() + " >> "+directoryName+"/"+str(i).zfill(6)+".csv &"

	for i in range(30) :
		sys.stdout.write("\r" + str(30-i) + " ")
		sys.stdout.flush()
		sleep(1)


	info( "\n\n*** Testing network\n\n" )

	cmd2 = ["sshpass", "-p", "password", "ssh", "nick@192.168.1.11", "'./start-cpu-mem-capture.sh' &"]
	process2 = subprocess.Popen(cmd2)

	cmd = ['./openflow-sniffex', sys.argv[3], sys.argv[4], sys.argv[len(sys.argv)-1]+"/cap.csv"]
	process = subprocess.Popen(cmd)

	info( "\n\n*** Start send package\n" )
	for i in range(numberOfSends) :
		randHost[i].cmd(cmds[i])
		sleep(Y[i])

	for i in range(30) :
		sys.stdout.write("\r" + str(120-i) + " ")
		sys.stdout.flush()
		sleep(1)
	process.kill()
	for i in range(90) :
		sys.stdout.write("\r" + str(90-i) + " ")
		sys.stdout.flush()
		sleep(1)
		
	# CLI( net )
	net.stop()
	

if __name__ == '__main__':
	setLogLevel( 'info' )
	emptyNet()