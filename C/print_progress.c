#include <unistd.h>
#include <stdio.h>

#define NUM_OF_LOOP         10000
#define LOADING_SLEEP       1
#define COMRETE_PROGRESS    100

int stdout_progress(int looping, int all_object){
    int percent = 0;

    //達成度をパーセント表記に変換 
    percent = (int)(( looping * 100 ) / all_object );

    printf("\r[%d %]\tNow Loading. ( %d / %d )", percent, looping, all_object);

    //標準出力バッファをクリアする。
    fflush(stdout);

    //返却用の変数に現進捗度に代入する
    if(COMRETE_PROGRESS == percent){
        printf("\n");
    }
    return 0;
}

int main(){
    int loop = 0;
    int percent = 0;

    do{

        //何らかの処理

        stdout_progress(loop, NUM_OF_LOOP);
        ++loop;
    }while(NUM_OF_LOOP >= loop);

    return 0;
}
