#include "viewrank.h"

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

int
parse_data(char *import_data, struct profiles *profile){
    char data_name[PERSONAL_NAME_LEN] = "";
    float data_age = 0;
    float data_height = 0;
    float data_weight = 0;
    char *saveptr = NULL; //strtok_rの処理状況を保存する変数
    char *excluded_char = NULL;
    int format_incorrect = 0;   //不正なフォーマットの検出に使用
    char format_incorrct_part[FORMAT_IMPORT_CHECK] = "";    //フォーマットの不正箇所表示用
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


int 
sort_age(struct profiles *profile){



    return OK;
}
