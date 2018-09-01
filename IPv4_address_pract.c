#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PATH "input.txt"
#define BUFFER_LENGTH 256
#define V4ADDR_LENGTH 16
#define V6ADDR_LENGTH 39
#define ADDRTYPE_LENGTH 4

#define TNO_V4_OCTET 4
#define V4_OCTFILL 8

struct v4address{
	char v4addrs[V4ADDR_LENGTH];
	int v4prefix[TNO_V4_OCTET];
	struct v4address *next;
};

struct v6address{
	char v6addrs[V6ADDR_LENGTH];
	struct v6address *next;
};

void init_addr_struct(struct v4address *v4addr,struct v6address *v6addr,unsigned short int file_lines);
void file_tkn(struct v4address *v4addr,struct v6address *v6addr);
int v4subnetmask(struct v4address *v4addr,int v4_masknum);
int v6prefix(struct v6address *v6addr,int v6_masknum);
int addr_binary(struct v4address *v4addr,int v4_prefix_len);
int conversion_binary(int decimal_num[],int ret_prefix[],struct v4address *v4addr);
int subnet_calculation(int subnet,int *ret_prefix);
int two_to_the_pow(int remainder);


void 
init_addr_struct(struct v4address *v4addr,struct v6address *v6addr,unsigned short int file_lines){
	unsigned short int init_loop = 0;
	for(init_loop=0;init_loop<file_lines;init_loop++){
		memset(v4addr[init_loop].v4addrs,0x00,sizeof(v4addr[init_loop].v4addrs));
		memset(v6addr[init_loop].v6addrs,0x00,sizeof(v6addr[init_loop].v6addrs));
		v4addr[init_loop].next = &v4addr[init_loop+1];
		v6addr[init_loop].next = &v6addr[init_loop+1];
	}
        v4addr[init_loop].next = &v4addr[init_loop+1];
        v6addr[init_loop].next = &v6addr[init_loop+1];
}


int
main(int argc,char *argv[]){
	FILE *fp;
	int v4_mask = 0;
	int v6_mask = 0;
	int i = 0;
	int j = 0;
	int k = 0;
	unsigned short int file_lines = 0;
	char buff[BUFFER_LENGTH];
	
        fp = fopen(PATH,"r");
        if(fp == NULL){
                printf("Could not open file:%s\n",PATH);
                exit(1);
        }else{
                printf("open file:%s\n",PATH);
        }

        fp = fopen(PATH,"r");
        while(fgets(buff,BUFFER_LENGTH,fp) != NULL ){
                file_lines++;
        }
        fclose(fp);

        struct v4address v4addr[file_lines];
        struct v6address v6addr[file_lines];

	init_addr_struct(v4addr,v6addr,file_lines);

	if(argc == 1){
		printf("Argument is empty.\n");
		exit(1);
	}else if(argc == 2){
		printf("IPv6 prefix length is not correct.\n");
		exit(1);
	}

	file_tkn(v4addr,v6addr);

	v4_mask = atoi(argv[1]);
        v6_mask = atoi(argv[2]);
	
	for(i=0;i<file_lines;i++){
		addr_binary(&v4addr[i],v4_mask);
	}
	
	for(j=0;j<file_lines;j++){
		for(k=0;k<TNO_V4_OCTET-1;k++){
			printf("%d.",v4addr[j].v4prefix[k]);
		}
		printf("%d\n",v4addr[j].v4prefix[TNO_V4_OCTET-1]);
	}
	return 0;
}

void
file_tkn(struct v4address *v4addr,struct v6address *v6addr){
        FILE *fp;
        unsigned short int loop = 0;
        char buff[BUFFER_LENGTH];
	char ip_addr_type[ADDRTYPE_LENGTH];

        fp = fopen(PATH,"r");
        while(fgets(buff,BUFFER_LENGTH,fp) != NULL){
                strcpy(ip_addr_type,strtok(buff,":"));
		if(strcmp(ip_addr_type,"IPv4")==0){
			strcpy(v4addr[loop].v4addrs,strtok(NULL,"/"));
		}else if(strcmp(ip_addr_type,"IPv6")==0){
			strcpy(v6addr[loop].v6addrs,strtok(NULL,"/"));
		}else{
			printf("Format is not correct.\n");
		}
		loop++;
	}
	fclose(fp);
}

int 
v4subnetmask(struct v4address *v4addr,int v4_masknum){
	

	return 0;
}

int
v6prefix(struct v6address *v6addr,int v6_masknum){


	return 0;
}

int
addr_binary(struct v4address *v4addr,int v4_prefix_len){
        int num_addr[4] = {0,0,0,0};
        int loop = 0;
        int v4prefix[4] = {0,0,0,0};
        int prefix_len = 0;

        num_addr[loop] = atoi(strtok(v4addr->v4addrs,"."));
        for(loop=1;loop<3;loop++){
                num_addr[loop] = atoi(strtok(NULL,"."));
        }	
	
        prefix_len = v4_prefix_len;
        subnet_calculation(prefix_len,v4prefix);
        conversion_binary(num_addr,v4prefix,v4addr);

        return 0;
}

int
subnet_calculation(int subnet,int ret_v4prefix[]){
        int subnet_seed;
        int sbnt_remainder = 0;
        int sbnt_loop = 0;
        int i = 0;
        int j = 0;
        int k = 0;

        subnet_seed = subnet - V4_OCTFILL;
        sbnt_remainder = subnet % V4_OCTFILL;
        sbnt_loop = subnet / V4_OCTFILL;//小数点以下切り捨て

        if(subnet_seed<0){
                ret_v4prefix[0] = two_to_the_pow(sbnt_remainder);
                for(i=1;i<TNO_V4_OCTET;i++){
                        ret_v4prefix[i] = 0;
                }
        }else{
                for(i=0;i<sbnt_loop;i++){
                        ret_v4prefix[i] = 255;
                }

                j =sbnt_loop;

                if(sbnt_loop<TNO_V4_OCTET){
                        ret_v4prefix[j] = two_to_the_pow(sbnt_remainder);
                        if(4 < j){
                                return 0;
                        }
                }
                for(k=j+1;k<TNO_V4_OCTET;k++){
                        ret_v4prefix[k] = 0;
                }
        }

        return 0;
}

int
conversion_binary(int decimal_num[],int ret_prefix[],struct v4address *v4addr){
        unsigned short int dec_to_bin_loop = 0;

	for(dec_to_bin_loop=0;dec_to_bin_loop<TNO_V4_OCTET;dec_to_bin_loop++){
                v4addr->v4prefix[dec_to_bin_loop] = decimal_num[dec_to_bin_loop] & ret_prefix[dec_to_bin_loop];
        }
	return 0;
}

int two_to_the_pow(int remainder){
        int i = 0;
        int j = 0;
        int k = 0;
        int ret = 0;
        int tmp_ret = 1;
        for(i=0;i<remainder;i++){
                tmp_ret = 1;
                for(j=0;j<(k=7-i);j++){
                        tmp_ret = 2 * tmp_ret;
                }
                ret = ret + tmp_ret;
        }
        return ret;
}

