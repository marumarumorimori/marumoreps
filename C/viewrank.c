#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>	//コンパイル時-pthreadを忘れないこと！
#include <getopt.h>

//OPTIONS_CHARACTERはソート方法選択時のgetopt用		
#define	OPTIONS_CHARACTER	"123:"
#define	FILE_NAME_LENGTH	24
#define	OPTION_LENGTH		4
#define	PERSONAL_NAME_LEN	52	//UTF-8の漢字が3 byte取るので多めに 
#define	FIRST_ARG			1
#define	SECOND_ARG			2
#define	THIRD_ARG			3
#define	NUMOF_ALLOPTIONS	4	//argv[0]=viewrank込みの配列長
#define OPTIONS_LEN			6	//sort用引数の最大文字列長
#define	FILE_READ_BUF_LEN	80
#define	FORMAT_IMPORT_CHECK	12
#define TMP_BUFFER			80

//戻り値
#define	OK					0
#define	NG					1
#define	STRTOK_R_OK			0
#define	THREAD_CREATE_OK	0

struct profiles{
	char name[PERSONAL_NAME_LEN];
	float age;
	float height;
	float weight;
	char import_file[FILE_NAME_LENGTH];
	struct profiles *next_profile;
};

void *reading_file(struct profiles *profile);
int parse_data(char *import_data, struct profiles *profile);
static int check_argument(int argc, char ***argv);
int add_profile(char *parsed_name,float parsed_age, float parsed_height, float parsed_weight, struct profiles *profile);
int get_option(int argc, char *options);
int structure_memory_free(struct profiles *profile);

//ファイル内に限定されたグローバルフラグ
static int flag_age = 0;
static int flag_height = 0;
static int flag_weight = 0;
//排他処理が必要かは不明だが、一応宣言
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/*import_fileで指定したファイルのデータ読み込み用関数*/
void
*reading_file(struct profiles *profile){
	FILE *p_read_file = NULL;
	char import_data[FILE_READ_BUF_LEN] = "";
	//関数戻り値チェック用変数
	int res = 0;
	char tmp[TMP_BUFFER] = "";

	strncpy(tmp, profile->import_file, FILE_NAME_LENGTH);
	p_read_file = fopen(profile->import_file,"r");
	if(NULL == p_read_file){
		printf("%s:Could not open file.\n",__func__);
		exit(NG);
	}
	
	//\0末端考慮のためFILE_READ_BUF_LEN-1を1行の読み取り上限としている
	while(NULL != fgets(import_data, FILE_READ_BUF_LEN-1, p_read_file)){
		if( NULL == import_data){
			exit(NG);
		}

		res = parse_data(import_data, profile);
		if(NG == res){
			exit(NG);
		}
	}
	fclose(p_read_file);

	return OK;
}


//ごちゃごちゃしているので共通処理を新規関数におこせないか要検討
int
parse_data(char *import_data, struct profiles *profile){
	char data_name[PERSONAL_NAME_LEN] = "";
	float data_age = 0;
	float data_height = 0;
	float data_weight = 0;
	char *saveptr = NULL; //strtok_rの処理状況を保存する変数
	char *excluded_char = NULL;	
	int format_incorrect = 0;	//不正なフォーマットの検出に使用
	char format_incorrct_part[FORMAT_IMPORT_CHECK] = "";	//フォーマットの不正箇所表示用
	char *tmp = NULL;
	
	//名前データを抽出
	tmp = strtok_r(import_data, ",", &saveptr);
	strncpy(data_name, tmp, PERSONAL_NAME_LEN);

	//年齢データを抽出
	tmp = strtok_r(NULL, ",", &saveptr);
	if(0 == strcmp("\n",tmp)){
		++format_incorrect;
		return NG;
	}
	data_age = strtol(tmp, &excluded_char,10);	
	if((STRTOK_R_OK  != *excluded_char) && ('\n' != *excluded_char)){
		++format_incorrect;
		strncpy(format_incorrct_part, excluded_char, FORMAT_IMPORT_CHECK);	
		printf("INCORRECT FORMAT AGE : This part is not correct : %s\n",format_incorrct_part);
	}

	//身長データを抽出
	tmp = strtok_r(NULL, ",", &saveptr);
	if(0 == strcmp("\n",tmp)){
		++format_incorrect;
		return NG;
	}
	data_height = strtol(tmp, &excluded_char,10);	
	if((STRTOK_R_OK != *excluded_char) && ('\n' != *excluded_char)){
		++format_incorrect;
		strncpy(format_incorrct_part, excluded_char, FORMAT_IMPORT_CHECK);	
		printf("INCORRECT FORMAT HEIGHT : This part is not correct : %s\n",format_incorrct_part);
	}

	//体重データを抽出
	if(0 == strcmp("\n", tmp)){
		++format_incorrect;
		return NG;
	}
	tmp = strtok_r(NULL, ",", &saveptr);
	data_weight = strtol(tmp, &excluded_char,10);	
	if((STRTOK_R_OK  != *excluded_char) && ('\n' != *excluded_char)){
		++format_incorrect;
		strncpy(format_incorrct_part, excluded_char, FORMAT_IMPORT_CHECK);	
		printf("INCORRECT FORMAT WEIGHT: This part is not correct : %s\n",format_incorrct_part);
	}

	//不正フォーマットを検出した場合はプロセス強制終了
	if(0 != format_incorrect){
		printf("INCORRECT FORMAT : There was [%d] incorrect format.\n",format_incorrect);
		return NG;
	}

	add_profile(data_name, data_age, data_height, data_weight, profile);

	return OK;
}

int
add_profile(char *parsed_name, float parsed_age, float parsed_height, float parsed_weight, struct profiles *profile){
	//追加データ一時格納用	
	struct profiles *new_prof;	
	//現最後尾(最終更新データ)の構造体
	struct profiles *current_prof;	

	current_prof = profile;
	//データ追加用の構造体メモリを確保
	new_prof = (struct profiles *)malloc(sizeof(struct profiles));
	if(NULL == new_prof){
		printf("%s: Failed to get dynamic memory.\n",__func__);	
		return NG;
	}

	//解析済みデータをnewに一時格納	
	strncpy(new_prof->name, parsed_name, PERSONAL_NAME_LEN);
	new_prof->age = parsed_age;
	new_prof->height = parsed_height;
	new_prof->weight = parsed_weight;
	new_prof->next_profile = NULL;

	//最後尾の構造体まで移動
	while(current_prof->next_profile != NULL){
		current_prof = current_prof->next_profile;
	}
	
	//最後尾のcurrent_profに追加データを紐付ける
	current_prof->next_profile = new_prof;

	return OK;
}


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

static int
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
