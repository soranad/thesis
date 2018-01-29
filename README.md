# Thesis

### Controller setup

##### Install Virtual Machine
install KVM
```sh
$ sudo apt-get install qemu-kvm libvirt-bin ubuntu-vm-builder bridge-utils virt-manager
```
##### Install JAVA
install JAVA 7
```sh
$ sudo apt-get install openjdk-7-jdk
```
JAVA_HOME setup
```sh
$ sudo nano /etc/environment
```
`(paste on nantop)`
```sh
export JAVA_HOME=/usr/lib/jvm/java-7-openjdk-amd64/
```

install JAVA 8
```sh
$ sudo apt-get install default-jre default-jdk
```
JAVA_HOME setup
```sh
$ edit .bashrc
```
JAVA_HOME="/usr/lib/jvm/java-8-openjdk-amd64"
export JAVA_HOME
PATH=$PATH:$JAVA_HOME
export PATH


install JAVA oracle 8
```sh
$ sudo apt-get install software-properties-common
$ sudo add-apt-repository ppa:webupd8team/java -y
$ sudo apt-get update
$ sudo apt-get install oracle-java8-installer
```
JAVA_HOME setup
```sh
$ sudo nano /etc/environment
```
`(paste on nantop)`
```sh
export JAVA_HOME="/usr/lib/jvm/java-8-oracle"
```

update maven-3.3.9
```sh
$ wget http://www-eu.apache.org/dist/maven/maven-3/3.3.9/binaries/apache-maven-3.3.9-bin.tar.gz
$ tar -zxvf apache-maven-3.3.9-bin.tar.gz
$ sudo mv apache-maven-3.3.9 /usr/local/.
$ sudo nano /etc/profile 
```
`(paste on nantop)`
```sh
export M2_HOME=/usr/local/apache-maven-3.3.9
export M2=$M2_HOME/bin
export MAVEN_OPTS="-Xms256m -Xmx512m"
export PATH=$M2:$PATH
source /etc/profile
```

##### OpenDayLight
install OpenDayLight
```sh
$ tar -zxvf distribution-karaf-0.3.4-Lithium-SR4.tar.gz
$ mv distribution-karaf-0.3.4-Lithium-SR4 lithium-odl-0.3.4
$ cd ODL/lithium-odl-0.3.4
```
OpenDayLight Cluster Setup 
```sh
$ ./bin/configure_cluster.sh 1 10.200.8.185 10.200.8.186 10.200.8.190
```
run OpenDayLight
```sh
$ JAVA_MAX_MEM=4G JAVA_MAX_PERM_MEM=512m ./bin/karaf
```
install feature odl-l2switch-switch-ui 
```sh
opendaylight-user@root> feature:install odl-l2switch-switch-ui\
```

##### Open Network Operating System

##### open Files as root
```sh
$ sudo nautilus
```
##### change poweroff reboot shutdown permission
```sh
$ sudo chmod u+s /sbin/poweroff
$ sudo chmod u+s /sbin/reboot
$ sudo chmod u+s /sbin/shutdown
```
##### startup setting
```sh
$ sudo nano /etc/rc.local
```
##### install sshpass (remote command)
```sh
$ sudo apt-get install sshpass
```
##### install terminal system monitor 
```sh
$ sudo apt-get htop
```
##### untar .tar.gz
```sh
$ tar -xvzf <file.tar.gz>
```

### Topology Simulation Setup

##### mininet
check mininet Install
```sh
$ dpkg -L mininet
```
install mininet
```sh
$ sudo apt-get install mininet
```
run mininet
```sh
$ sudo mn 
```
run mininet remote controller
```sh
$ --controller=remote,ip=127.0.0.1,port=9933
```
run mininet topology setup 
```sh 
$ sudo mn --topo tree,depth=8,fanout=3
```
run custom tree mininet 
`python tree-topo.py <-a-> <-b-> <-c-> <-d-> <-e(1)-> <-e(2)-> <-f->`
a : number of switch
b : number of host in left switch
c : capture port
d : number of capture package
e : ip address of controller
f : output file (csv format)
Ex.
```sh 
$ python tree-topo.py 150 2 eth0 2000 127.0.0.1 test.csv
```

##### pcap
install pcap
```sh 
$ sudo apt-get install libpcap-dev
```
compile openflow-sniffex
```sh 
$ gcc openflow-sniffex.c -lpcap -o openflow-sniffex
```
run openflow-sniffex
`sudo ./openflow-sniffex <-a-> <-b-> <-c->`
a : capture port
b : number of capture package
c : output file (csv format)
Ex.
```sh 
$ sudo ./openflow-sniffex wlan0 3000 test.csv
```

compile cap_analysis
```sh 
$ gcc cap_analysis.c -o cap_analysis
```

run cap_analysis
`./cap_analysis <capture-file(csv-format)>`
Ex.
```sh 
$ ./cap_analysis test.csv
```