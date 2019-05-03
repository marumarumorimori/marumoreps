#include <iostream>
#include <string>
#include <map>
#include <uuid/uuid.h>      //Need -luuid when compiling.
#include <stdio.h>

#define UUID_LENGHT        32
#define OK                 0
#define NG                 1

class Account{
    private:
        std::string name_;
        std::string id_;
        std::string remarks_;
    public:
        Account(){};
        Account(const std::string& name,
                const std::string& id, 
                const std::string& remarks)
            :name_{name}, id_{id}, remarks_{remarks}{};

        const std::string& get_name() const noexcept{ return name_; } 
        const std::string get_remarks() const noexcept{ return remarks_; }
        const std::string get_id() const noexcept{ return id_; }
};

std::string create_uuid();
int create_account(std::map<std::string, Account>& account_map);
int serch_account(std::map<std::string, Account>& account_map);

std::string create_uuid(){
    uuid_t uuid;
    char string[UUID_LENGHT];

    uuid_generate(uuid);
    uuid_unparse_upper(uuid, string);

    std::string st_string(string, UUID_LENGHT);

    printf("Your key is \"%s\"\n", st_string.c_str());

    return st_string;
}

int create_account(std::map<std::string, Account>& account_map){
    std::string name = "";
    std::string remarks = "";
    std::string id = "";
    int loop = 0;

    for(loop=0; 2>loop; ++loop){
        printf("----------------------------------------------\n");
        //Get name_ here.
        printf("Please enter name :");
        std::getline(std::cin, name);

        //Get remark_ here.
        printf("Please enter remarks :");
        std::getline(std::cin, remarks);

        //Get id_ here.
        id = create_uuid();

        //Pass account_name and account_remark to class Account.
        Account account(name, id, remarks);
        account_map.insert(std::make_pair(account.get_id() ,account));

        //Standard output data.
        printf("----------------------------------------------\n");
        printf("Name\t:%s\n", account.get_name().c_str());
        printf("Remarks\t:%s\n", account.get_remarks().c_str());
        printf("\n");
    }
    printf("----------------------------------------------\n");

    return OK;
}

int serch_account(std::map<std::string, Account>& account_map){
    std::string key = "";

    //Receive key of value that user wants to acquire.
    printf("Please enter your key :");
    std::cin >> key;
    
    //Show result.
    std::map<std::string, Account>::iterator result = account_map.find(key);

    if(result != account_map.end()){
        printf("Name :%s\nRemarks :%s\n",result->second.get_name().c_str(), 
                    result->second.get_remarks().c_str());
    }else{
        printf("Key Error.\n");
    }
    return OK;
}

int main(void){
    std::map<std::string, Account> account_map;

    create_account(account_map);
    serch_account(account_map);

    return OK;
}

