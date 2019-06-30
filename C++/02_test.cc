#include <sstream>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <map>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>


typedef struct connection_info{
    std::time_t connection_time;
    
}connection_info_t;

class SecureLogReader{
    public:

    protected:

    private:
        std::string         ip_address_;
        

};

std::vector<std::string> split(const std::string &str, char separator){
    std::vector<std::string> vect;
    std::stringstream str_stream(str);
    std::string buffer;

    while(std::getline(str_stream, buffer, separator)){
        vect.push_back(buffer);
    }

    return vect;
}

bool search_for_word(std::string serch_word, std::string &line_element, bool now_flag){
    if(serch_word == line_element){
        return true;
    }else if(true == now_flag){
        return true;
    }

    return false;
}

bool str_to_int(const char *str_num, int &int_number){
    char *endptr = NULL;
    long long_number = strtol(str_num, &endptr, 10);

    if(0 != strncmp("\0", endptr, 1)){
        std::cout << "Conversion error. [" << *endptr << "]" << std::endl;
        return false;
    }
    
    if(INT_MAX < long_number ||  INT_MIN > long_number){
        std::cout << "Buffer over flow." << std::endl;
        return false;
    }

    int_number = (int)long_number;
    return true;
}

int main(int argc, char **argv){
    std::string file_line; 
    std::vector<std::string> list;
    std::map<std::string, std::string> log_map_data;
    int limit = 0;
    int count = 0;
    bool ret = false;

    if((NULL == argv[1]) || (NULL == argv[2])){
        std::cout << "Argument error." <<std::endl;
        return false;
    }

    ret = str_to_int(argv[2], limit);
    if(false == ret){
        return 1;
    }

    std::ifstream ifs(argv[1]);
    if(ifs.fail()){
        std::cerr << "Could not read file." << std::endl;
        return 1;
    }

    while(getline(ifs, file_line)){
        int loop = 0;
        bool failed_msg_flag = false;
        bool failed_user_name_flag = false;

        list = split(file_line, ' ');
        for(auto list_element: list){
            ++loop;
            failed_msg_flag = search_for_word("Failed", list_element, failed_msg_flag);
            failed_user_name_flag = search_for_word("invalid", list_element, failed_user_name_flag);
            if(failed_msg_flag){
                if(!failed_user_name_flag && 11 == loop){
                    log_map_data["IP Address"] = list_element;
                    std::cout << "IP Address : " << log_map_data["IP Address"] <<  std::endl;
                }else if(failed_user_name_flag && 11 == loop){
                    log_map_data["User Name"] = list_element;
                    std::cout << "User Name : " << log_map_data["User Name"] << std::endl;
                }
            }
        }
        if(limit < count){
            break;
        }
        ++count;
    } 

    return 0;
}
