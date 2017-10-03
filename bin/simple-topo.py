#!/usr/bin/python

from mininet.net import Mininet
from mininet.node import Controller, OVSSwitch, RemoteController
from mininet.cli import CLI
from mininet.log import setLogLevel, info

import subprocess

import random
import numpy as np
from time import sleep

def emptyNet():

   net = Mininet(controller=RemoteController, switch=OVSSwitch)

   c1 = net.addController('c1', controller=RemoteController, ip="127.0.0.1", port=6633)
   c2 = net.addController('c2', controller=RemoteController, ip="127.0.0.1", port=6633)

   numberOfHost = 4
   hosts = [None] * numberOfHost
   for i in range(numberOfHost) :
      hosts[i] = net.addHost( 'h'+str(i+1), ip='10.0.0.'+str(i+1) )

   s1 = net.addSwitch( 's1' )
   s2 = net.addSwitch( 's2' )

   s1.linkTo( s2 )
   s1.linkTo( hosts[0] )
   s1.linkTo( hosts[1] )
   s2.linkTo( hosts[2] )
   s2.linkTo( hosts[3] )

   net.build()
   c1.start()
   c2.start()
   s1.start([c1,c2])
   s2.start([c1,c2])

   net.start()

   cmd = ['./openflow-sniffex', 'lo', '10000000', 'testtest.csv']
   process = subprocess.Popen(cmd, stdout=subprocess.PIPE)

   info( "\n\n*** Start iperf Server\n" )
   for i in range(numberOfHost) :
      hosts[i].cmd("iperf -s &")
   info( "***done\n" )

   numberOfSends = 2000
   sendHosts = [None] * numberOfSends
   cmds = [None] * numberOfSends
   for i in range(numberOfSends) :
      randHost = random.sample(hosts, 2)
      sendHosts[i] = randHost[0];
      # cmds[i] = "iperf -c " + randHost[1].IP() + " -n 10000000 >> IPERF/"+str(randHost[0].IP())+"-"+str(randHost[1].IP())+".txt &"
      cmds[i] = "iperf -c " + randHost[1].IP() + " -n 10000000 &"

   time = 10
   alpha = numberOfSends/time
   beta = 1.0/alpha
   Y = np.random.exponential(beta, numberOfSends)

   sleep(5)

   info( "\n\n*** Start iperf client\n" )
   for i in range(numberOfSends) :
      # print str(i) + " " + str(Y[i]) + " " + sendHosts[i].name + " -> " + cmds[i]
      sendHosts[i].cmd(cmds[i])
      sleep(Y[i])
   info( "*** done\n" )

   sleep(10)
   # process.wait()

   # CLI( net )
   net.stop()

if __name__ == '__main__':
    setLogLevel( 'info' )
    emptyNet()