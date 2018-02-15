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
	int totalNumberOfSend = 0;
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
				int numberOfSend;

				fscanf(inF, "%d.%d.%d.%d,%d.%d.%d.%d,%lf ms, %d %d",&sIP1,&sIP2,&sIP3,&sIP4,&dIP1,&dIP2,&dIP3,&dIP4,&delay,&isSendSuccess,&numberOfSend); 
				fclose(inF);

				if(count == 0 || delay < minDelay){
					// minSip1 = sIP1;
					// minSip2 = sIP2;
					// minSip3 = sIP3;
					// minSip4 = sIP4;
					// minDip1 = dIP1;
					// minDip2 = dIP2;
					// minDip3 = dIP3;
					// minDip4 = dIP4;
					minDelay = delay;
				}
				if(count == 0 || delay > maxDelay){
					// maxSip1 = sIP1;
					// maxSip2 = sIP2;
					// maxSip3 = sIP3;
					// maxSip4 = sIP4;
					// maxDip1 = dIP1;
					// maxDip2 = dIP2;
					// maxDip3 = dIP3;
					// maxDip4 = dIP4;
					maxDelay = delay;
				}
				if(isSendSuccess != 1){
					// printf("%s\n",part);
					sendErrorCount++;
				}
				else{
					totalNumberOfSend += numberOfSend;
					totalDelay += delay;
					count++;
				}
			}
		}
		closedir(d);
	}
	printf("total:%d   send error:%d   numer of send:%d\n", count, sendErrorCount,totalNumberOfSend);
	printf("%lf,%lf,%lf\n",minDelay,maxDelay,totalDelay/count);


	// calculate control package
	char str[999];
	FILE * file;
	strcpy(part, argv[1]);
	strcat(part, "cap.csv");
	file = fopen(part, "r");

	int types[10];
	int versions[60];
	int i;

	for(i=0; i<10; i++){
		types[i] = 0;
	}
	for(i=0; i<60; i++){
		versions[i] = 0;
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
			if( type >= 0 && type < 10){
				types[type]++;
			}
			if(version >= 0 && version < 60 ){
				versions[version]++;
			}
		}
		fclose(file);
	}

	for(i=0; i<5; i++){
		if(types[i] != 0){
			printf("%2d : %10d \n",i,types[i]);
		}
	}
	printf("\n");
	for(i=0; i<60; i++){
		if(versions[i] != 0){
			printf("%2d : %10d \n",i,versions[i]);
		}
	}


	// calculate controller cpu & mem used
	strcpy(part, argv[1]);
	strcat(part, "cpu-mem-uses.txt");

	file = fopen(part, "r");

	double cpuMin;
	double cpuMax;
	double cpuAvg;
	double memMin;
	double memMax;
	double memAvg;

	if (file) {
		char processName[128];
		double cpu;
		double mem;
		int numLine = 0;

		while(fscanf(file, "%s %lf, %lf", processName, &cpu, &mem) != EOF){
			// printf("%s %lf, %lf\n", processName, cpu, mem);
			if(numLine == 0){
				cpuMin = cpu;
				cpuMax = cpu;
				cpuAvg = cpu;
				memMin = mem;
				memMax = mem;
				memAvg = mem;
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
				cpuAvg += cpu;
				memAvg += mem;
			}
			numLine++;
		}
		cpuAvg = cpuAvg/numLine;
		memAvg = memAvg/numLine;
		fclose(file);
	}
	printf("cpu : %.2lf %.2lf %.2lf\n", cpuMin, cpuMax, cpuAvg);
	printf("mem : %.2lf %.2lf %.2lf\n", memMin, memMax, memAvg);

	// printf("MIN %d.%d.%d.%d -> %d.%d.%d.%d : %lf ms\n", minSip1,minSip2,minSip3,minSip4,minDip1,minDip2,minDip3,minDip4,minDelay);
	// printf("MAX %d.%d.%d.%d -> %d.%d.%d.%d : %lf ms\n", maxSip1,maxSip2,maxSip3,maxSip4,maxDip1,maxDip2,maxDip3,maxDip4,maxDelay);
	// printf("AVG : %lf ms\n", totalDelay/count);
	

  return(0);
}