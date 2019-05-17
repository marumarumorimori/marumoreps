#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>    /*コンパイル時-pthreadを忘れないこと！*/
#include <getopt.h>

/*OPTIONS_CHARACTERはソート方法選択時のgetopt用*/
#define OPTIONS_CHARACTER   "123:"
#define FILE_NAME_LENGTH    24
#define OPTION_LENGTH       4
#define PERSONAL_NAME_LEN   52  /*UTF-8の漢字が3 byte取るので多めに*/
#define FIRST_ARG           1
#define SECOND_ARG          2
#define THIRD_ARG           3
#define NUMOF_ALLOPTIONS    4   /*argv[0]=viewrank込みの配列長*/
#define OPTIONS_LEN         6   /*sort用引数の最大文字列長*/
#define FILE_READ_BUF_LEN   80
#define FORMAT_IMPORT_CHECK 12
#define TMP_BUFFER          80

/*標準出力フォーマット*/
#define HELP_FORMAT         "Usage: viewrank <import file> <export file> -{1|2|3|h}\n\n" \
                            "  <import file>      Specify the file to be imported.\n" \
                            "  <export file>      Specify the file to be exported.\n" \
                            "  --age-sort   ,-1   Sort ages in ascending order.\n" \
                            "  --height-sort,-2   Sort height in descending order\n" \
                            "  --weight-sort,-3   Sort weight in ascending order.\n" \
                            "  --help       ,-h   Display help.\n"
/*戻り値*/
#define OK                  0
#define NG                  1
#define STRTOK_R_OK         0
#define THREAD_CREATE_OK    0
#define THREAD_JOIN_OK      1

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
int add_profile(char *parsed_name,float parsed_age, float parsed_height, float parsed_weight, struct profiles *profile);
int get_option(int argc, char **options);
int structure_memory_free(struct profiles *profile);
struct profiles* sort_age(struct profiles *profile);
struct profiles* sort_height(struct profiles *profile);
struct profiles* sort_weight(struct profiles *profile);
