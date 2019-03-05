#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define	OK	0
#define	NG	1

int bubble_sort(int *numbers);
int show_numbers(int *numbers);

int
bubble_sort(int *numbers){
	int stac = 0;
	//loop values
	int i = 0;
	int j = 0;

	for(i=0;i<9;i++){
		printf("loop:%d\n",i);
		for(j=i+1;j<10;j++){
			if(numbers[i]<numbers[j]){
				stac = numbers[i];
				numbers[i] = numbers[j];
				numbers[j] = stac;
			}
			show_numbers(numbers);
			usleep(1000000);
		}
	}
	
	return OK;
}

int
show_numbers(int *numbers){
	//loop valueds
	int a = 0;
	
	for(a=0;a<10;a++){
		printf("%d",numbers[a]);
		if(a<9){
			printf(",");
		}else{
			printf("\n");
		}
	}
	return OK;
}

int
main(void){
	int numbers[10] = {1,3,5,2,8,6,7,9,10,4};
	
	printf("Start\n");
	show_numbers(numbers);
	bubble_sort(numbers);
	printf("End\n");

	return OK;
}
