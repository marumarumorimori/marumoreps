#include "viewrank.h"

//ファイル内に限定されたグローバルフラグ
static int flag_age = 0;
static int flag_height = 0;
static int flag_weight = 0;

/*排他処理が必要かは不明だが、一応宣言*/
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

//コマンド解析用関数
int
get_option(int argc, char *argv){
	int opt = 0;
	int index = 0;
	struct option longopts[] = {
							{"age-sort",	no_argument,		NULL,	'1' },
							{"height-sort",	no_argument,		NULL,	'2' },
							{"weight-sort",	no_argument,		NULL,	'3' },
							{"help",		no_argument,		NULL,	'h' },
							{ 0,			0,					0,		 0  }
							};

	while (-1 != (opt = getopt_long(argc, argv, "123h", longopts, &index))){
		switch(opt){
			case '1':
				flag_age = 1;
				break;
			case '2':
				flag_height = 1; 
				break;
			case '3':
				flag_weight = 1;
				break;
			case 'h':
				//help表示処理追加予定
				break;
			default:
				printf("Invalid argument.\n");
				break;
		}
	}
	return OK;
}

int
check_argument(int argc, char ***argv){
	if(NUMOF_ALLOPTIONS != argc){
		printf("%s: Insufficient arguments\n",__func__);
		return NG;
	}

	return OK;
}

int
structure_memory_free(struct profiles *profile){
	struct profiles *clear_prof = NULL;
	struct profiles *next_clear = NULL;

	clear_prof = profile;

	while(NULL != clear_prof->next_profile){
		next_clear = clear_prof->next_profile;
		free(clear_prof);
		clear_prof = next_clear;
	}
	return OK;
}

int
main(int argc,char *argv[]){
	struct profiles *profile = NULL;
	char export_file[FILE_NAME_LENGTH] = "";
	pthread_t pthread;
	int thread_status = 0;
	//戻り値チェック用変数
	int arg_res = 0;

	arg_res = check_argument(argc, &argv);
	if(NG == arg_res){
		printf("Invalid argument.\n");
		return NG;
	}

	//ファイルデータ格納用の構造体
	//スレッドから構造体リストの先頭を返してもらうためにここで宣言
	profile = (struct profiles *)malloc(sizeof(struct profiles));
	profile->next_profile = NULL;

	//argumentを各変数に割り振る
	strncpy(profile->import_file, argv[FIRST_ARG], FILE_NAME_LENGTH);
	strncpy(export_file, argv[SECOND_ARG], FILE_NAME_LENGTH);
	
	//file読み取りをスレッドで並列処理
	thread_status = pthread_create(&pthread, NULL, &reading_file, profile);
	if(THREAD_CREATE_OK != thread_status){
		printf("Could not create thread.\n");
		return NG;
	}

	//オプション読み込み
	get_option(argc, argv);

	//reading_file()スレッド合流
	pthread_join(pthread, NULL);

	//sort処理
    

	//構造体リストオールクリア	
	structure_memory_free(profile);

	return OK;
}
