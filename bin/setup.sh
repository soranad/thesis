sudo apt-get install -y libpcap-dev 
gcc openflow-sniffex.c -o openflow-sniffex -lpcap
gcc calculateResult.c -o calculateResult
gcc cap_analysis.c -o cap_analysis

sudo apt-get install -y mininet

sudo apt-get install -y python-pip
pip install numpy

sudo apt-get install python-numpy python-scipy

sudo apt-get install -y sshpass