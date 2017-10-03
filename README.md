# thesis

# install pcap
	sudo apt-get install libpcap-dev

# compile openflow-sniffex
	gcc openflow-sniffex.c -lpcap -o openflow-sniffex

# run openflow-sniffex
	sudo ./openflow-sniffex <-a-> <-b-> <-c->
	a : capture port
	b : number of capture package
	c : output file (csv format)

	Ex.
	sudo ./openflow-sniffex wlan0 3000 test.csv

# compile cap_analysis
	gcc cap_analysis.c -o cap_analysis

# run cap_analysis
	./cap_analysis <capture-file(csv-format)>
	Ex.
	./cap_analysis test.csv

# install mininet
	sudo apt-get install mininet

# run tree topo
	python tree-topo.py <-a-> <-b-> <-c-> <-d-> <-e(1)-> <-e(2)-> <-f->
	a : number of switch
	b : number of host in left switch
	c : capture port
	d : number of capture package
	e : ip address of controller
	f : output file (csv format)

	Ex.
	python tree-topo.py 150 2 eth0 2000 127.0.0.1 test.csv


$sudo apt-get install default-jre default-jdk

sudo apt-get clean

edit .bashrc
JAVA_HOME="/usr/lib/jvm/java-8-openjdk-amd64"
export JAVA_HOME
PATH=$PATH:$JAVA_HOME
export PATH

echo $JAVA_HOME
ODL$ tar -xvzf distribution-karaf-0.6.1-Carbon.tar.gz

# KVM setup

sudo apt-get install qemu-kvm libvirt-bin ubuntu-vm-builder bridge-utils virt-manager


# untar .tar.gz

tar -xvzf <file.tar.gz>

# open Files as root
sudo nautilus

# change poweroff reboot shutdown permission
sudo chmod u+s /sbin/poweroff
sudo chmod u+s /sbin/reboot
sudo chmod u+s /sbin/shutdown

# startup setting
sudo nano /etc/rc.local

#install sshpass (remote command)
sudo apt-get install sshpass

# install terminal system monitor 
sudo apt-get htop

# open Files as root
sudo nautilus

