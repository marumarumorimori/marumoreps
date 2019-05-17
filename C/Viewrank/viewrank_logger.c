#include "viewrank.h"
#include <stdarg.h>
#include <time.h>

#define LOG_FILE_PATH               "/root/exec_viewrank.log"
#define ARGMENTS_MEMBER_MAX         4
#define ARGMENTS_LENGTH             64
#define LOG_LENGTH                  255
#define MESSAGES_LENGTH             128

int
main(int argc, char **argv){
    
    viewrank_logger(__func__, argv[1], argv[2], argv[3], argv[4], argv[5]);

    return 0;
}

int
viewrank_logger(const char *function, const char *format, const char *args, ...){
    char argments[ARGMENTS_MEMBER_MAX][ARGMENTS_LENGTH] = {{""}};
    const char *get_args_loop = "";
    char messages[MESSAGES_LENGTH] = "";
    char form[MESSAGES_LENGTH] = "";
    int copy_loop = 0;
    FILE *log_file = NULL;
    va_list arg_list;
    time_t timer;
    struct tm *utc;

    timer = time(NULL);
    utc = gmtime(&timer);

    va_start(arg_list, args);
    for(get_args_loop=args; NULL != get_args_loop; get_args_loop=va_arg(arg_list, const char*)){
        if(ARGMENTS_MEMBER_MAX <= copy_loop){
            break;
        }else{
            printf("%s\n", get_args_loop);
            snprintf(argments[copy_loop], ARGMENTS_LENGTH-1,"%s", get_args_loop);
        }
        ++copy_loop;
    }   

    log_file = fopen(LOG_FILE_PATH, "a+");
    if(NULL == log_file){
        printf("ERROR : Could not open log file.\n");
        return NG;
    } 
    snprintf(form, 254, "%s", format); 
    snprintf(messages, MESSAGES_LENGTH - 1, form, argments[0], argments[1], argments[2], argments[3]);
    fprintf(log_file, "%4d/%2d/%2d %2d:%2d:%2d [%s] %s\n", 
            utc->tm_year + 1900, utc->tm_mon + 1, utc->tm_mday, utc->tm_hour, 
            utc->tm_min, utc->tm_sec, function, messages);

    return 0;
}
