#include <stdio.h>

typedef struct {
	int portNo;
	int maxWinSize;
	int sumWinSize;
	int numOfPackage;
	double firstTime;
	double lastTime;
	double timeToStable;
} port;

int main(int argc, char **argv){

	FILE *inF;
	if ( argc > 1) {
		printf("%s\n", argv[1]);
		inF = fopen (argv[1], "r");
	}
	else{
		inF = fopen ("fileIn.csv", "r");
	}

	FILE* outF;
	if ( argc > 2) {
		outF = fopen (argv[2], "w");
	}
	else{
		outF = fopen ("result.txt", "w");
	}

	port p[50000];
	int numOfPort = 0;

	char* line = NULL;
	size_t len = 0;
	getline(&line, &len, inF);
	printf("%s\n", line);

	int index;
	int sIP1,sIP2,sIP3,sIP4;
	int dIP1,dIP2,dIP3,dIP4;
	int sourcePort;
	int destinstionPort;
	double time;
	int winsize;
	int payload;

	while(fscanf(inF, "%d,%d.%d.%d.%d,%d.%d.%d.%d,%d,%d,%lf,%d,%d", &index, &sIP1,&sIP2,&sIP3,&sIP4, &dIP1,&dIP2,&dIP3,&dIP4, &sourcePort, &destinstionPort, &time, &winsize, &payload) != EOF){
		if( winsize < 10000 && ( sourcePort == 6655 || sourcePort == 6653 || sourcePort == 6633 )){
			int position = -1;
			int i;
			for(i=0; i<numOfPort; i++){
				if(destinstionPort == p[i].portNo){
					position = i;
					break;
				}
			}
			if(position >= 0){
				if( winsize > p[position].maxWinSize ){
					p[position].maxWinSize = winsize;
					p[position].timeToStable = time;
				}
				p[position].lastTime = time;
				p[position].sumWinSize += winsize;
				p[position].numOfPackage = p[position].numOfPackage + 1;
			}
			else{
				p[numOfPort].portNo = destinstionPort;
				p[numOfPort].maxWinSize = winsize;
				p[numOfPort].sumWinSize = winsize;
				p[numOfPort].numOfPackage = 1;
				p[numOfPort].firstTime = time;
				p[numOfPort].lastTime = time;
				p[numOfPort].timeToStable = time;
				numOfPort++;
			}
		}
		
	}

	double min ;
	double max ;
	double avg = 0.0;
	int numberOfSitch = 0;

	int i;
	for(i=0; i<numOfPort; i++){
		if(p[i].numOfPackage  > 10){
			if(numberOfSitch == 0){
				min = p[i].timeToStable - p[i].firstTime;
				max = p[i].timeToStable - p[i].firstTime;
			}
			numberOfSitch++;
			double useTime = p[i].timeToStable - p[i].firstTime;
			if(min > useTime){
				min = useTime;
			}
			if(max < useTime){
				max = useTime;
			} 
			avg += useTime;
			printf("%d\t%d\t%d\t%d\t%d %10.3lf %10.3lf %10.3lf\n",i, p[i].portNo, p[i].maxWinSize, p[i].sumWinSize, p[i].numOfPackage, p[i].timeToStable, p[i].firstTime, useTime);
		}
	}
	avg = avg / numberOfSitch;

	printf("\n%d : switch    max : %lf    min: %lf    avg: %lf\n\n",numberOfSitch,max,min,avg);

	return 0;
}
