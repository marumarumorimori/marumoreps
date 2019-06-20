#include <sstream>
#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <map>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

std::vector<std::string> split(const std::string &str, char separator){
    std::vector<std::string> vect;
    std::stringstream str_stream(str);
    std::string buffer;

    while(std::getline(str_stream, buffer, separator)){
        vect.push_back(buffer);
    }

    return vect;
}

bool search_ssh_failure(std::string &line_element, bool failed_msg_flag){
    std::string failure_msg("Failed");

    if(failure_msg == line_element){
        return true;
    }else if(true == failed_msg_flag){
        return true;
    }

    return false;
}

bool search_user_name_failure(std::string &line_element, bool failed_user_name){
    std::string failure_user("invalid");

    if(failure_user == line_element){
        return true;
    }else if(true == failed_user_name){
        return true;
    }

    return false;
}

bool str_to_int(const char *str_num, int &ret_num){
    char *endptr = NULL;
    long l_number = strtol(str_num, &endptr, 10);
 
    if(0 != strncmp("\0", endptr, 1)){
        std::cout << "Conversion error. [" << *endptr << "]" << std::endl;
        std::cout << "In :" << &str_num <<std::endl;
        std::cout << "Err:" << &endptr <<std::endl;
        return false;
    }
    
    if(INT_MAX < l_number ||  INT_MIN > l_number){
        std::cout << "Buffer over flow." << std::endl;
        return false;
    }

    std::cout << "In :" << str_num <<std::endl;
    std::cout << "Err:" << endptr <<std::endl;

    ret_num = (int)l_number;
    return true;
}

int main(int argc, char **argv){
    std::string file_line; 
    std::vector<std::string> list;
    std::map<std::string, std::string> log_map_data;
    int limit = 0;
    int count = 0;
    bool ret = false;

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
        bool failed_user_name = false;

        list = split(file_line, ' ');
        for(auto list_element: list){
            ++loop;
            failed_msg_flag = search_ssh_failure(list_element, failed_msg_flag);
            failed_user_name = search_user_name_failure(list_element, failed_user_name);
            if(failed_msg_flag){
                if(!failed_user_name && 11 == loop){
                    log_map_data["IP Address"] = list_element;
                    std::cout << "IP Address : " << log_map_data["IP Address"] <<  std::endl;
                }else if(failed_user_name && 11 == loop){
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
