#include <dirent.h> 
#include <stdio.h> 
#include <string.h>

int main(int argc, char **argv)
{
	// calculate ping latency 
	DIR *d;
	struct dirent *dir;
	char part[128];

	int maxSip1,maxSip2,maxSip3,maxSip4;
	int maxDip1,maxDip2,maxDip3,maxDip4;
	double maxDelay;

	int minSip1,minSip2,minSip3,minSip4;
	int minDip1,minDip2,minDip3,minDip4;
	double minDelay;

	double totalDelay = 0;
	strcpy(part, argv[1]);
	strcat(part, "ping/");
	d = opendir(part);
	int count = 0;
	int sendErrorCount = 0;
	if (d) {
		while ((dir = readdir(d)) != NULL){
			if(dir->d_type == DT_REG){
				strcpy(part, argv[1]);
				strcat(part, "ping/");
				strcat(part, dir->d_name);

				FILE *inF;
				if ( argc > 1) {
					inF = fopen (part, "r");
				}
				else{
					return(-1);
				}

				int sIP1,sIP2,sIP3,sIP4;
				int dIP1,dIP2,dIP3,dIP4;
				double delay;
				int isSendSuccess;

				fscanf(inF, "%d.%d.%d.%d,%d.%d.%d.%d,%lf ms, %d",&sIP1,&sIP2,&sIP3,&sIP4,&dIP1,&dIP2,&dIP3,&dIP4,&delay,&isSendSuccess); 
				fclose(inF);
				
				if(isSendSuccess == 1){
					if(count == 0){
						minDelay = delay;
						maxDelay = delay;
					}
					else{
						if( delay < minDelay){
							minDelay = delay;
						}
						if( delay > maxDelay){
							maxDelay = delay;
						}
					}
					totalDelay += delay;
					count++;
				}
				else{
					sendErrorCount++;
					
				}
			}
		}
		closedir(d);
	}
	
	double avgDelay = 0.0;
	if(count > 0){
		totalDelay/count;
	}

	printf("\n\x1b[32m#####  LATENCY  #####\x1b[0m\n");
	printf("ping success : %10d\n", count);
	printf("ping error   : %10d\n", sendErrorCount);
	printf("ping min   : %12.3lf\n", minDelay);
	printf("ping max   : %12.3lf\n", maxDelay);
	printf("ping avg   : %12.3lf\n", totalDelay/count);

	// calculate controller cpu & mem used
	FILE * file;
	strcpy(part, argv[1]);
	strcat(part, "cpu-mem-uses.txt");

	file = fopen(part, "r");

	double cpuMin;
	double cpuMax;
	double sumCPU = 0;
	double memMin;
	double memMax;
	double sumMem = 0;
	int numLine = 0;

	if (file) {
		char processName[128];
		double cpu ;
		double mem ;

		while(fscanf(file, "%s %lf, %lf", processName, &cpu, &mem) != EOF){
			// printf("%s %lf, %lf\n", processName, cpu, mem);
			if(numLine == 0){
				cpuMin = cpu;
				cpuMax = cpu;
				memMin = mem;
				memMax = mem;
			}
			else{
				if(cpu < cpuMin){
					cpuMin = cpu;
				}
				if(cpu > cpuMax){
					cpuMax = cpu;
				}
				if(mem < memMin){
					memMin = mem;
				}
				if(mem > memMax){
					memMax = mem;
				}
			}
			numLine++;
			sumCPU += cpu;
			sumMem += mem;
		}
		fclose(file);
	}

	double cpuAvg = 0.0;
	double memAvg = 0.0;
	if(numLine > 0){
		cpuAvg = sumCPU/numLine;
		memAvg = sumMem/numLine;
	}

	printf("\n\x1b[32m#####  CONTROLLER USE  #####\x1b[0m\n");
	printf("cpu use min    : %12.3lf\n", cpuMin);
	printf("cpu use max    : %12.3lf\n", cpuMax);
	printf("cpu use avg    : %12.3lf\n", cpuAvg);
	printf("memory use min : %12.3lf\n", memMin);
	printf("memory use max : %12.3lf\n", memMax);
	printf("memory use avg : %12.3lf\n", memAvg);

	// calculate control package
	char * nameList[][6] = {
		{"Type\\Version","OFP 1.0","OFP 1.1","OFP 1.2","OFP 1.3","OFP 1.4"},
		{"0","Hello","Hello","Hello","Hello","Hello"},
		{"1","Error","Error","Error","Error","Error"},
		{"2","EchoReq","EchoReq","EchoReq","EchoReq","EchoReq"},
		{"3","EchoRes","EchoRes","EchoRes","EchoRes","EchoRes"},
		{"4","Vendor","Experimenter","Experimenter","Experimenter","Experimenter"},
		{"5","FeatureReq","FeatureReq","FeatureReq","FeatureReq","FeatureReq"},
		{"6","FeatureRes","FeatureRes","FeatureRes","FeatureRes","FeatureRes"},
		{"7","GetConfigReq","GetConfigReq","GetConfigReq","GetConfigReq","GetConfigReq"},
		{"8","GetConfigRes","GetConfigRes","GetConfigRes","GetConfigRes","GetConfigRes"},
		{"9","SetConfig","SetConfig","SetConfig","SetConfig","SetConfig"},
		{"10","PacketIn","PacketIn","PacketIn","PacketIn","PacketIn"},
		{"11","FlowRemoved","FlowRemoved","FlowRemoved","FlowRemoved","FlowRemoved"},
		{"12","PortStatus","PortStatus","PortStatus","PortStatus","PortStatus"},
		{"13","PacketOut","PacketOut","PacketOut","PacketOut","PacketOut"},
		{"14","FlowMod","FlowMod","FlowMod","FlowMod","FlowMod"},
		{"15","PortMod","GroupMod","GroupMod","GroupMod","GroupMod"},
		{"16","StatsReq","PortMod","PortMod","PortMod","PortMod"},
		{"17","StatsRes","TableMod","TableMod","TableMod","TableMod"},
		{"18","BarrierReq","StatsReq","StatsReq","MultipartReq","MultipartReq"},
		{"19","BarrierRes","StatsRes","StatsRes","MultipartRes","MultipartRes"},
		{"20","QueueGetConfigReq","BarrierReq","BarrierReq","BarrierReq","BarrierReq"},
		{"21","QueueGetConfigRes","BarrierRes","BarrierRes","BarrierRes","BarrierRes"},
		{"22","-","QueueGetConfigReq","QueueGetConfigReq","QueueGetConfigReq","-"},
		{"23","-","QueueGetConfigRes","QueueGetConfigRes","QueueGetConfigRes","-"},
		{"24","-","-","RoleReq","RoleReq","RoleReq"},
		{"25","-","-","RoleRes","RoleRes","RoleRes"},
		{"26","-","-","-","GetAsyncReq","GetAsyncReq"},
		{"27","-","-","-","GetAsyncRes","GetAsyncRes"},
		{"28","-","-","-","SetAsync","SetSync"},
		{"29","-","-","-","MeterMod","MeterMod"},
		{"30","-","-","-","-","RoleStatus"},
		{"31","-","-","-","-","TableStatus"},
		{"32","-","-","-","-","RequestForward"},
		{"33","-","-","-","-","BundleControl"},
		{"34","-","-","-","-","BundleAddMessage"}
	};

	strcpy(part, argv[1]);
	strcat(part, "cap.csv");
	file = fopen(part, "r");

	int packageTypes[6][35];
	int i;
	int j;
	long long int totalPayload = 0;
	int packageCount = 0;

	for(i=0; i<6; i++){
		for(j=0; j<35; j++){
			packageTypes[i][j] = 0;
		}
	}

	if (file) {

		char* line = NULL;
		size_t len = 0;
		getline(&line, &len, file);

		int index;
		int sIP1,sIP2,sIP3,sIP4;
		int dIP1,dIP2,dIP3,dIP4;
		int sourcePort;
		int destinstionPort;
		double time;
		int winsize;
		int payload;
		int type;
		int version;

		while(fscanf(file, "%d,%lf,%d.%d.%d.%d,%d,%d.%d.%d.%d,%d,%d,%d,%d,%d", &index, &time, &sIP1,&sIP2,&sIP3,&sIP4, &sourcePort, &dIP1,&dIP2,&dIP3,&dIP4, &destinstionPort, &winsize, &payload, &type, &version) != EOF){
			// printf("%d %d %d\n",index, type, version);
			if( type >= 0 && type < 10 && version >= 0 && version < 60 ){
				packageCount++; 
				packageTypes[type][version]++;
				totalPayload += payload;
			}
		}
		fclose(file);
	}

	printf("\n\x1b[32m#####  NETWORK USE  #####\x1b[0m\n");
	printf("number of package : %12d\n", packageCount); 
	printf("payload sumation  : %12lld\n", totalPayload);

	for(j=0; j<35; j++){
		for(i=0; i<6; i++){
			if(packageTypes[i][j] > 0){
				printf("%-15s %-15s : %12d \n",nameList[0][i],nameList[j][i], packageTypes[i][j]);
			}
		}
	}
	printf("\n");
  return(0);
}