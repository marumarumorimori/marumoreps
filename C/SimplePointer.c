#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define	OK	0
#define	NG	1

int addition(int number1,int number2,int *result);

int
addition(int number1,int number2,int *result){
	*result = number1 + number2;

	return OK;
}

int
main(int argc,char *argv[]){
	int number1 = 0;
	int number2 = 0;
	int result = 0;

	if(2 > argc){
		printf("引数を入力してください。\n");
		return NG;
	}

	if((6<strlen(argv[1])) && (6<strlen(argv[2]))){
		printf("引数の数値が大きすぎます。\n");
		return NG;
	}

	number1 = atoi(argv[1]);
	number2 = atoi(argv[2]);

	printf("格納された数値は%d,%dです。\n",number1,number2);
	
	addition(number1,number2,&result);
	printf("計算結果は%dです。\n",result);

	return OK;
}
