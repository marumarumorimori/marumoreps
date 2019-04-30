#include <iostream>
#include <string>
#include <map>
#include <uuid/uuid.h>      //Need -luuid when compiling.
#include <stdio.h>

class Account{
    private:
        std::string account_name;
        std::string account_id;
        std::string account_remarks;
    public:
        Account(std::string name="", std::string remarks=""){
            account_name = name;
            account_remarks = remarks;
            account_id = create_uuid();
        }

        //For comparison operator function calls.
        //Account(){}

        std::string create_uuid(){
            uuid_t uuid;
            char string[32];

            uuid_generate(uuid);
            uuid_unparse_upper(uuid, string);

            std::string st_string(string, 32);

            printf("Your key is \"%s\"\n", st_string.c_str());

            return st_string;
        }

        std::string get_account_name(){
            return account_name;
        }
        
        std::string get_account_remarks(){
            return account_remarks;
        }
        
        std::string get_account_id(){
            return account_id;
        }
        
        bool operator<(const Account &rhs) const{
            return account_id < rhs.account_id;
        }

};

int main(void){
    std::string name = "";
    std::string remarks = "";
    std::string key = "";
    std::map<std::string, Account> account_map;
    int loop = 0;

    for(loop=0; 2>loop; ++loop){
        //Get account_name here.
        printf("Please enter name :");
        std::getline(std::cin, name);

        //Get account_remark here.
        printf("Please enter remarks :");
        std::getline(std::cin, remarks);

        //Pass account_name and account_remark to class Account.
        Account account(name, remarks);
        account_map.insert(std::make_pair(account.get_account_id() ,account));

        //Standard output data.
        printf("Name\t:%s\n", account.get_account_name().c_str());
        printf("Remarks\t:%s\n", account.get_account_remarks().c_str());
        printf("\n");
    }

    //Receive key of value that user wants to acquire.
    printf("Please enter your key :");
    std::cin >> key;
    
    //Show result.
    printf("Name :%s\nRemarks :%s\n",account_map[key].get_account_name().c_str(), 
                account_map[key].get_account_remarks().c_str());
  
    return 0;
}

