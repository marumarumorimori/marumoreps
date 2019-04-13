#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_COLUMNS         256
#define NUM_OF_COLUMNS      4
#define COLUMNS_PATTERN     {1,0,0,0}
#define CONVERSION_TO_CHAR  1
#define CONVERSION_TO_INT   0
#define FILE_READ_BUF_LEN   1000

int
main(void){
    FILE *fp = NULL;
    char tmp[256] = "";
    char strings[256] = "";
    int switch_parse[NUM_OF_COLUMNS] = COLUMNS_PATTERN;
    int loop = 0;
    char *endptr = 0;
    int itmp = 0;

    fp = fopen("testt", "r");

    while(NULL != fgets(strings, FILE_READ_BUF_LEN-1, fp)){
        snprintf(tmp, 255, "%s", strtok(strings, ","));
        loop = 0;
        while((0 != strcmp("(null)",tmp)) || (NUM_OF_COLUMNS > loop)){
            switch(switch_parse[loop]){
                case CONVERSION_TO_CHAR:
                    itmp = strtol(tmp, &endptr, 10);
                    if(0 != itmp){
                        printf("ERROR1:%d\n", itmp);
                        return 1;
                    }
                    printf("%d: %s\n",loop , tmp);
                    snprintf(tmp, 255, "%s", strtok(NULL, ","));
                    break;
                case CONVERSION_TO_INT:
                    itmp = strtol(tmp, &endptr, 10);
                    if((0 != strncmp("\n", endptr, 256) && (NUM_OF_COLUMNS == (loop+1))) 
                                || ((0 != *endptr) && (NUM_OF_COLUMNS > (loop+1)))){
                        printf("ERROR1:%s\n", endptr);
                        return 1;
                    }
                    printf("%d: %d\n",loop ,itmp);
                    snprintf(tmp, 255, "%s", strtok(NULL, ","));
                    break;
                default:
                    printf("ERROR2\n");
                    return 1;
            }
            ++loop;
        }
    }

    return 0;
}
