#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define	OK						0
#define	NG						1
#define	ARRAY_WIDTH				10
#define	ARRAY_WIDTH_ONE_MINUS	ARRAY_WIDTH - 1 
#define	ONE_SEC_SLEEP			1000000
#define	FLAG_ON					1
#define	FLAG_OFF				0

int bubble_sort(int *numbers);
int show_numbers(int *numbers,int loop_val_i,int loop_val_j,int brackets_flag);

int
bubble_sort(int *numbers){
	int stac = 0;
	//Flag to emphasize exchanged values
	int brackets_flag = FLAG_OFF;
	//loop values
	int i = 0;
	int j = 0;

	for(i=0;i<ARRAY_WIDTH_ONE_MINUS;i++){
		printf("loop:%d\n",i);
		for(j=i+1;j<ARRAY_WIDTH;j++){
			brackets_flag = FLAG_OFF;
			if(numbers[i]<numbers[j]){
				stac = numbers[i];
				numbers[i] = numbers[j];
				numbers[j] = stac;
				brackets_flag = FLAG_ON;
			}
			show_numbers(numbers,i,j,brackets_flag);
			usleep(ONE_SEC_SLEEP);
		}
	}
	
	return OK;
}

int
show_numbers(int *numbers,int loop_val_i,int loop_val_j,int brackets_flag){
	//loop valueds
	int a = 0;
	
	for(a=0;a<ARRAY_WIDTH;a++){
		if(FLAG_ON == brackets_flag && (loop_val_i == a || loop_val_j == a)){
			printf( "[%d]\t",numbers[a]);
		}else{
			printf( " %d\t",numbers[a]);
		}
	}
	printf("\n");

	return OK;
}

int
main(void){
	int numbers[ARRAY_WIDTH] = {1,3,5,2,8,6,7,9,10,4};
	
	printf("Start\n");
	//取り敢えず100決め打ち。。。（何れ直したい）
	show_numbers(numbers,100,100,FLAG_OFF);
	bubble_sort(numbers);
	printf("End\n");

	return OK;
}
