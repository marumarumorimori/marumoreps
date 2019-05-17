#include "viewrank.h"

#define FLAG_ON             1

//ファイル内に限定されたグローバルフラグ
static int flag_age = 0;
static int flag_height = 0;
static int flag_weight = 0;
static int flag_help = 0;

//コマンド解析用関数
int
get_option(int argc, char **options){
    int opt = 0;
    int index = 0;
    struct option longopts[] = {
                            {"age-sort",    no_argument,        NULL,   '1' },
                            {"height-sort", no_argument,        NULL,   '2' },
                            {"weight-sort", no_argument,        NULL,   '3' },
                            {"help",        no_argument,        NULL,   'h' },
                            { 0,            0,                  0,       0  }
                            };

    while (-1 != (opt = getopt_long(argc, (char * const*)&(*options), "123h", longopts, &index))){
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
                printf("%s", HELP_FORMAT);
                flag_help = 1;
                return OK;
            default:
                printf("Invalid argument.\n");
                break;
        }
    }

    if(NUMOF_ALLOPTIONS == argc){
        return OK;
    }else{
        printf("Invalid argument.\n");
        return NG;
    }
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
main(int argc, char **argv){
    struct profiles *profile = NULL;
    char export_file[FILE_NAME_LENGTH] = "";
    pthread_t pthread;
    int thread_status = 0;
    void *thread_join_status = NULL;
    //戻り値チェック用変数
    int check_res = 0;

    //ファイルデータ格納用の構造体
    //スレッドから構造体リストの先頭を返してもらうためにここで宣言
    profile = (struct profiles *)malloc(sizeof(struct profiles));
    memset(profile, 0, sizeof(struct profiles));
    profile->next_profile = NULL;

    //argumentを各変数に割り振る
    snprintf(profile->import_file, FILE_NAME_LENGTH,"%s" ,argv[FIRST_ARG]);
    snprintf(export_file, FILE_NAME_LENGTH, "%s", argv[SECOND_ARG]);

    //オプション読み込み
    check_res = get_option(argc, argv);
    if(FLAG_ON == flag_help){
        return OK;
    }else if(NG == check_res){
        return NG;
    }

    //file読み取りをスレッドで並列処理
    thread_status = pthread_create(&pthread, NULL, (void *)reading_file, profile);
    if(THREAD_CREATE_OK != thread_status){
        printf("Could not create thread.\n");
        return NG;
    }

    //reading_file()スレッド合流
    pthread_join(pthread, NULL);
    //if(THREAD_JOIN_OK != *(int *)thread_join_status){
    //    printf("The thread failed to join.\n");
    //    return NG;
    //}

    printf("main\n");
    struct profiles *debug;
    debug = profile;
    while(NULL != debug){
        printf("%s\t\t\t%f\n", debug->name, debug->age);
        debug = debug->next_profile;
    }

    //sort処理
    if(1 == flag_age){
        profile = sort_age(profile);
    }else if(1 == flag_height){
        profile = sort_height(profile);
    }else if(1 == flag_weight){
        profile = sort_weight(profile);
    }
    printf("sort\n");
    debug = profile;
    while(NULL != debug){
        if(0 != debug->age){
            printf("%s\t", debug->name);
            printf("%f\t", debug->age);
            printf("%f\t", debug->height);
            printf("%f\n", debug->weight);
        }
        debug = debug->next_profile;
    }

    //構造体リストオールクリア    
    structure_memory_free(profile);

    return OK;
}
