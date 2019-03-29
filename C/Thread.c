#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define	OK				0
#define	NG				-1

void *function_thread(void *p);

int g_data = 0;
pthread_mutex_t mutex;

void 
*function_thread(void *p){
	int next = 0;
	int now = 0;
	int i = 0;
	
	printf("start %d\n",*(int*)p);

	for(i=0;i<1000000;++i){
		pthread_mutex_lock(&mutex);
		next = g_data + 1;
		now = g_data;
		g_data = next;
		pthread_mutex_unlock(&mutex);

		printf("[%s] g_data : [%d]\n",__func__,g_data);		
		sleep(1);

		if(now+1 != next){
			printf("Other thread changes g_data+1:[%d],next:[%d]\n",g_data,next);
		}else{
			//None processing
		}
	}
	return ;
}


int
main(void){
	pthread_t pthread;
	int l_data = 0;
	int next = 0;
	int now = 0;
	int i = 0;

	pthread_mutex_init(&mutex,NULL);
	printf("test\n");
	pthread_create(&pthread, NULL, &function_thread, &l_data);

	for(i=0;i<1000000;++i){
		pthread_mutex_lock(&mutex);
		next = g_data + 1;
		now = g_data;
		g_data = next;
		pthread_mutex_unlock(&mutex);

		printf("[%s] g_data : [%d]\n",__func__,g_data);		
		sleep(1);

		if(now+1 != next){
			printf("Other thread changes g_data+1:[%d],next:[%d]\n",g_data,next);
		}else{
			//None processing	
		}
	}
	pthread_join(pthread,NULL);
	printf("g_data:[%d]\n",g_data);

	return OK;
}
