#include <dirent.h> 
#include <stdio.h> 

int main(void) {
	DIR *d;
	struct dirent *dir;
	d = opendir(".");
	int count = 0;
	if(d){
	while((dir = readdir(d)) != NULL){
		if(dir->d_type == DT_REG){
			rintf("%d\t%s\n",++count , dir->d_name);
		}
	}
	closedir(d);
	}
	return(0);
}