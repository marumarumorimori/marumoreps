#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>

#define ONE_HOUR 3600
#define TWO_HOUR 7200
#define INFINITE_LOOP 1
#define PID_FILE "/root/pract_directory/04_no-ip/update_no-ip.pid"

int update_curltimer();
int create_timerlimit(unsigned int min,unsigned int max,unsigned int *response);

int
update_curltimer(){
	unsigned int endTime = 0;
	unsigned int currentTime = 0;
	unsigned int minSecond = ONE_HOUR;
	unsigned int maxSecond = TWO_HOUR;
	unsigned int limitSecond = 0;
	char *curlURL = "curl -s ";

	create_timerlimit(minSecond,maxSecond,&limitSecond);
	endTime = time(NULL) + limitSecond;

	while(INFINITE_LOOP){
		if(currentTime>endTime){
			break;
		}
		currentTime = endTime - time(NULL);
		sleep(5);
	}
	
	system(curlURL);
	return 0;
}


int
create_timerlimit(unsigned int min,unsigned int max,unsigned int *response){
	*response = min +(unsigned int)(rand()*(max-min+1)/(1+RAND_MAX));
	return 0;
}

int
main(int argc,char *argv[]){

	if(0 == daemon(0,0)){
		while(INFINITE_LOOP){
			update_curltimer();
		sleep(5);
		}
	}else{
		printf("%s:Error",argv[0]);
	}

	return 0;
}
