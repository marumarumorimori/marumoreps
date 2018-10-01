#include <stdio.h>
#include <time.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define ONE_HOUR 3600
#define TWO_HOUR 7200
#define INFINITE_LOOP 1
#define PID_FILE "update_no-ip.pid"//未実装
#define CURL_COMMAND "curl -s -m 20 YOUR-USERNAME:PASSWORD@dynupdate.no-ip.com/nic/update?hostname=DOMAIN.DOME.DOME > /ファ/イ/ル/パ/ス/curl_result"
#define RETRY_CHECK_FILE "/ファ/イ/ル/パ/ス/curl_result"
#define BUFF_LEN 256
#define RETRY_LIMIT 20

int update_curltimer();
int create_timerlimit(unsigned int min,unsigned int max,unsigned int *response);
int retry_curl();

int
update_curltimer(){
	unsigned int endTime = 0;
	unsigned int currentTime = 0;
	unsigned int minSecond = ONE_HOUR;
	unsigned int maxSecond = TWO_HOUR;
	unsigned int limitSecond = 0;

	create_timerlimit(minSecond,maxSecond,&limitSecond);
	endTime = time(NULL) + limitSecond;

	while(INFINITE_LOOP){
		if(currentTime>endTime){
			break;
		}
		currentTime = endTime - time(NULL);
	}
	
	system(CURL_COMMAND);
	retry_curl();

	if(0 == remove(RETRY_CHECK_FILE)){
		return 0;
	}else{
		return 1;
	}
}


int
create_timerlimit(unsigned int min,unsigned int max,unsigned int *response){
	*response = min +(unsigned int)(rand()*(max-min+1)/(1+(unsigned int)RAND_MAX));
	return 0;
}

int
retry_curl(){
	FILE *fp_read;
	char buffer[BUFF_LEN];	
	unsigned short int i = 0;

	fp_read = fopen(RETRY_CHECK_FILE,"r");
	if(NULL == fp_read){
		return 1;
	}
	fgets(buffer,BUFF_LEN,fp_read);
	for(i=0;i<RETRY_LIMIT;++i){
		if(0 == strncmp(buffer,"badauth",7) || 0 == strcmp(buffer,"")){
			system(CURL_COMMAND);
			sleep(20);
			fgets(buffer,BUFF_LEN,fp_read);
		}else{
			break;
		}
	}
	fclose(fp_read);
	return 0;
}

int
main(int argc,char *argv[]){

	if(0 == daemon(0,0)){
		while(INFINITE_LOOP){
			update_curltimer();
		}
	}else{
		printf("%s:Error",argv[0]);
	}

	return 0;
}
