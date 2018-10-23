#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <error.h>
#include <limits.h>

#define PATH_READ "input.txt"
#define PATH_WRITE "output.txt"
#define BUFFER_LENGTH 256
#define V4ADDR_LENGTH 16
#define ADDRTYPE_LENGTH 4
#define TNO_V4_OCTET 4
#define V4_OCTET_LENGTH 8
#define V4_OCTFILL 255 
#define OK 0
#define NG 1

struct v4address{
	char v4addrs[V4ADDR_LENGTH];
	int v4prefix[TNO_V4_OCTET];
	int v4subnetmask[TNO_V4_OCTET];
	struct v4address *next;
};

int file_tkn(struct v4address *start_v4addr,struct v4address *v4addr,struct v4address *new_v4addr);
int v4output(struct v4address *start_v4addr,struct v4address *v4addr);
int addr_binary(struct v4address *start_v4addr,struct v4address *v4addr,int v4_prefix_len);
int conversion_binary(int decimal_num[],struct v4address *v4addr);
int subnet_calculation(int subnet,struct v4address *v4addr);
int notfill_subnet_loop(int remainder);


int
main(int argc,char *argv[]){
	char *p_tolend;
	int v4_mask = 0;
	int ret_check = 0;
	struct v4address start_v4addr;
	struct v4address *v4addr;
	struct v4address *new_v4addr;
	
	if(1==argc){
		printf("Argument is empty.\n");
		return NG;
	}else{
		printf("Argument is correct:Argument=%s\n",argv[1]);
		v4_mask = (int)strtol(argv[1],&p_tolend,10);
	}

	ret_check = file_tkn(&start_v4addr,v4addr,new_v4addr);
	if(1==ret_check){
		prirtf("File_tkn error.\n");
		return 1;
	}

	for(v4addr=&start_v4addr;v4addr->next!=NULL;v4addr=v4addr->next){
		ret_check = addr_binary(&start_v4addr,v4addr,v4_mask);
		if(1==ret_check){
			prirtf("Addr_binary error.\n");
			return 1;
		}
        }
	
	ret_check = v4output(&start_v4addr,v4addr)
	if(1==ret_check){
		prirtf("V4output error.\n");
		return 1;
	}
	

	free(v4addr);
	return OK;
}

int
file_tkn(struct v4address *start_v4addr,struct v4address *v4addr,struct v4address *new_v4addr){
	FILE *fp_read;
	char buff[BUFFER_LENGTH];
	char ip_addr_type[ADDRTYPE_LENGTH];

	v4addr = start_v4addr;
	v4addr->next = NULL;

	fp_read = fopen(PATH_READ,"r");
	if(NULL==fp_read){
		printf("Could not open file:%s\n",PATH_READ);
		return NG;
	}else{
		printf("open file:%s\n",PATH_READ);
	}

        while(fgets(buff,BUFFER_LENGTH,fp_read) != NULL){
		new_v4addr = (struct v4address *)malloc(sizeof(struct v4address));
		strcpy(ip_addr_type,strtok(buff,":"));
		if(NULL==new_v4addr){
			printf("Could not get memory.\n");
			free(new_v4addr);
			return NG;
		}else{
			if(0==strcmp(ip_addr_type,"IPv4")){
				strcpy(v4addr->v4addrs,strtok(NULL,"/"));	
				v4addr->next = new_v4addr;
				new_v4addr->next = NULL;
				v4addr = new_v4addr;
			}else{
				printf("Format is not correct.\n");
				break;
			}
		}
	}
	fclose(fp_read);
	return OK;
}	

int 
v4output(struct v4address *start_v4addr,struct v4address *v4addr){
	FILE *fp_write;
	fp_write = fopen(PATH_WRITE,"w");
	if(NULL==fp_write){
		printf("Could not open file:%s\n",PATH_WRITE);
		return NG;
	}else{
		printf("open file:%s\n",PATH_WRITE);
	}

	for(v4addr=start_v4addr;v4addr->next!=NULL;v4addr=v4addr->next){
		fprintf(fp_write,"IPv4\t\t:%s\n",v4addr->v4addrs);
		fprintf(fp_write,"SUBNETMASK\t:%d.%d.%d.%d\n",v4addr->v4subnetmask[0],
			v4addr->v4subnetmask[1],v4addr->v4subnetmask[2],v4addr->v4subnetmask[3]);
		fprintf(fp_write,"NETWORK ADDR\t:%d.%d.%d.%d\n",v4addr->v4prefix[0],
	                v4addr->v4prefix[1],v4addr->v4prefix[2],v4addr->v4prefix[3]);
	}
	fclose(fp_write);
	return OK;
}

int
addr_binary(struct v4address *start_v4addr,struct v4address *v4addr,int v4_prefix_len){
        int num_addr[TNO_V4_OCTET];
	int ret_check = 0;
        unsigned short int loop = 0;
	char buffer[BUFFER_LENGTH];
	char *p_tolend_ab;
	char *p_tmp;

	p_tmp = buffer;
	strcpy(buffer,v4addr->v4addrs);
	for(loop=0;loop<TNO_V4_OCTET;++loop){
		num_addr[loop] = (int)strtol(strtok(p_tmp,"."),&p_tolend_ab,10);
		p_tmp = NULL;
	}
	ret_check = subnet_calculation(v4_prefix_len,v4addr);
	if(1==ret_check){
		prirtf("Subnet_calculation error.\n");
		return 1;
	}

	ret_check = conversion_binary(num_addr,v4addr);
	if(1==ret_check){
		prirtf("Conversion_binary error.\n");
		return 1;
	}

        return OK;
}

int
subnet_calculation(int prefix_len,struct v4address *v4addr){
        int subnet_seed;
        int sbnt_remainder = 0;
        int sbnt_loop = 0;
	int tmp_subnet[TNO_V4_OCTET];
        unsigned short int i = 0;
        unsigned short int j = 0;
        unsigned short int k = 0;

        subnet_seed = prefix_len - V4_OCTET_LENGTH;
        sbnt_remainder = prefix_len % V4_OCTET_LENGTH;
	/*↑ (prefix長/8)の余りの数            */
	/*255未満のsubnetをとるoctetのprefix長*/
        sbnt_loop = prefix_len / V4_OCTET_LENGTH;
	/*↑ (prefix長/8)=255が入るoctetの数	     */
	/*int型のため、少数以下切り捨て   	     */
	/*gcc (GCC) 4.8.5 20150623 (Red Hat 4.8.5-28)*/

        if(0>subnet_seed){
                tmp_subnet[0] = notfill_subnet_loop(sbnt_remainder);
                for(i=1;i<TNO_V4_OCTET;i++){
                        tmp_subnet[i] = 0;
                }
        }else{
                for(i=0;i<sbnt_loop;++i){
                        tmp_subnet[i] = V4_OCTFILL;
                }
                if(sbnt_loop<TNO_V4_OCTET){
                        tmp_subnet[sbnt_loop] = notfill_subnet_loop(sbnt_remainder);
                        if(TNO_V4_OCTET < j){
                                return OK;
                        }
                }
                for(j=sbnt_loop+1;j<TNO_V4_OCTET;++j){
                        tmp_subnet[j] = 0;
                }
        }
	for(k=0;k<TNO_V4_OCTET;++k){
		v4addr->v4subnetmask[k] = tmp_subnet[k];
	}
        return OK;
}

int
conversion_binary(int decimal_num[],struct v4address *v4addr){
        int dec_to_bin_loop = 0;

	for(dec_to_bin_loop=0;dec_to_bin_loop<TNO_V4_OCTET;++dec_to_bin_loop){
                v4addr->v4prefix[dec_to_bin_loop] = decimal_num[dec_to_bin_loop] & v4addr->v4subnetmask[dec_to_bin_loop];
        }
	return OK;
}

int
notfill_subnet_loop(int remainder){
	int ret = 0;//戻り値
	int tmp_ret = 1;//2^0の値
	int i = 0;
	int j = 0;
	int k = 0;
        
	/*2^i計算のループ*/
        for(i=0;i<remainder;++i){
                tmp_ret = 1;
		/*2^7から2^1までを足し算計算*/
 		/*Σ2^kを8>k>0の範囲で降順で */
		/*実行する。		    */
                for(j=0;j<(k=7-i);++j){
                        tmp_ret = 2 * tmp_ret;
                }
                ret = ret + tmp_ret;
        }
        return ret;
}
