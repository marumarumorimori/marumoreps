#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PATH_READ "input.txt"
#define PATH_WRITE "output.txt"
#define BUFFER_LENGTH 256
#define V4ADDR_LENGTH 16
#define ADDRTYPE_LENGTH 4

#define TNO_V4_OCTET 4
#define V4_OCTET_LENGTH 8
#define V4_OCTFILL 255 

struct v4address{
	char v4addrs[V4ADDR_LENGTH];
	int v4prefix[TNO_V4_OCTET];
	int v4subnetmask[TNO_V4_OCTET];
};

void init_addr_struct(struct v4address *v4addr,unsigned short int file_lines);
void file_tkn(struct v4address *v4addr);
int v4output(struct v4address *v4addr,FILE *fp_write);
int addr_binary(struct v4address *v4addr,int v4_prefix_len);
int conversion_binary(int decimal_num[],struct v4address *v4addr);
int subnet_calculation(int subnet,struct v4address *v4addr);
int two_to_the_pow(int remainder);


void 
init_addr_struct(struct v4address *v4addr,unsigned short int file_lines){
	unsigned short int init_loop = 0;
	for(init_loop=0;init_loop<file_lines;init_loop++){
		memset(v4addr[init_loop].v4addrs,0,sizeof(v4addr[init_loop].v4addrs));
		memset(v4addr[init_loop].v4prefix,0,sizeof(v4addr[init_loop].v4prefix));
		memset(v4addr[init_loop].v4subnetmask,0,sizeof(v4addr[init_loop].v4subnetmask));
	}
}


int
main(int argc,char *argv[]){
	FILE *fp_read;
	FILE *fp_write;
	int v4_mask = 0;
	unsigned short int file_lines = 0;
	unsigned short int i = 0;
	unsigned short int j = 0;
	char buff[BUFFER_LENGTH];
	
        fp_read = fopen(PATH_READ,"r");
        if(fp_read == NULL){
                printf("Could not open file:%s\n",PATH_READ);
                exit(1);
        }else{
                printf("open file:%s\n",PATH_READ);
        }

        fp_read = fopen(PATH_READ,"r");
        while(fgets(buff,BUFFER_LENGTH,fp_read) != NULL ){
                file_lines++;
        }
        fclose(fp_read);

        struct v4address v4addr[file_lines];
	init_addr_struct(v4addr,file_lines);

	if(argc == 1){
		printf("Argument is empty.\n");
		exit(1);
	}else{
		printf("Argument is correct:Argument=%s\n",argv[1]);
	}

	file_tkn(v4addr);

	v4_mask = atoi(argv[1]);

	for(i=0;i<file_lines;i++){
		addr_binary(&v4addr[i],v4_mask);
	}

	fp_write = fopen(PATH_WRITE,"w");	
	for(j=0;j<file_lines;j++){
		v4output(&v4addr[j],fp_write);
	}
	fclose(fp_write);

	return 0;
}

void
file_tkn(struct v4address *v4addr){
        FILE *fp_read;
        unsigned short int loop = 0;
        char buff[BUFFER_LENGTH];
	char ip_addr_type[ADDRTYPE_LENGTH];

        fp_read = fopen(PATH_READ,"r");
        while(fgets(buff,BUFFER_LENGTH,fp_read) != NULL){
                strcpy(ip_addr_type,strtok(buff,":"));
		if(strcmp(ip_addr_type,"IPv4")==0){
			strcpy(v4addr[loop].v4addrs,strtok(NULL,"/"));
		}else{
			printf("Format is not correct.\n");
		}
		loop++;
	}
	fclose(fp_read);
}	

int 
v4output(struct v4address *v4addr,FILE *fp_write){	
	fprintf(fp_write,"IPv4\t\t:%s\n",v4addr->v4addrs);
	fprintf(fp_write,"SUBNETMASK\t:%d.%d.%d.%d\n",v4addr->v4subnetmask[0],
		v4addr->v4subnetmask[1],v4addr->v4subnetmask[2],v4addr->v4subnetmask[3]);
	fprintf(fp_write,"NETWORK ADDR\t:%d.%d.%d.%d\n",v4addr->v4prefix[0],
                v4addr->v4prefix[1],v4addr->v4prefix[2],v4addr->v4prefix[3]);
	return 0;
}

int
addr_binary(struct v4address *v4addr,int v4_prefix_len){
        int num_addr[TNO_V4_OCTET] = {0,0,0,0};
        unsigned short int loop = 0;
	char buffer[BUFFER_LENGTH];

	strcpy(buffer,v4addr->v4addrs);
        num_addr[loop] = atoi(strtok(buffer,"."));
        for(loop=1;loop<3;loop++){
                num_addr[loop] = atoi(strtok(NULL,"."));
        }		
        subnet_calculation(v4_prefix_len,v4addr);
        conversion_binary(num_addr,v4addr);

        return 0;
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

        if(subnet_seed<0){
                tmp_subnet[0] = two_to_the_pow(sbnt_remainder);
                for(i=1;i<TNO_V4_OCTET;i++){
                        tmp_subnet[i] = 0;
                }
        }else{
                for(i=0;i<sbnt_loop;i++){
                        tmp_subnet[i] = V4_OCTFILL;
                }
                if(sbnt_loop<TNO_V4_OCTET){
                        tmp_subnet[sbnt_loop] = two_to_the_pow(sbnt_remainder);
                        if(TNO_V4_OCTET < j){
                                return 0;
                        }
                }
                for(j=sbnt_loop+1;j<TNO_V4_OCTET;j++){
                        tmp_subnet[j] = 0;
                }
        }
	for(k=0;k<TNO_V4_OCTET;k++){
		v4addr->v4subnetmask[k] = tmp_subnet[k];
	}
        return 0;
}

int
conversion_binary(int decimal_num[],struct v4address *v4addr){
        unsigned short int dec_to_bin_loop = 0;

	for(dec_to_bin_loop=0;dec_to_bin_loop<TNO_V4_OCTET;dec_to_bin_loop++){
                v4addr->v4prefix[dec_to_bin_loop] = decimal_num[dec_to_bin_loop] & v4addr->v4subnetmask[dec_to_bin_loop];
        }
	return 0;
}

int two_to_the_pow(int remainder){
	int ret = 0;//戻り値
	int tmp_ret = 1;//2^0の値
	int i = 0;
	int j = 0;
	int k = 0;
        
	/*2^i計算のループ*/
        for(i=0;i<remainder;i++){
                tmp_ret = 1;
		/*2^7から2^1までを足し算計算*/
 		/*Σ2^kを8>k>0の範囲で降順で */
		/*実行する。		    */
                for(j=0;j<(k=7-i);j++){
                        tmp_ret = 2 * tmp_ret;
                }
                ret = ret + tmp_ret;
        }
        return ret;
}
