// exercise 5.4.6 file send/recv 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char *argv[])
{
	int serv_sd, clnt_sd;
	FILE * fp;
	char buf[BUF_SIZE];
	char file_name[BUF_SIZE];
	int read_cnt;
	
	struct sockaddr_in serv_adr, clnt_adr;
	socklen_t clnt_adr_sz;
	
	if(argc!=2) {
		printf("Usage: %s <port>\n", argv[0]);
		exit(1);
	}
	
	serv_sd=socket(PF_INET, SOCK_STREAM, 0);   
	
	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET;
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));
	
	bind(serv_sd, (struct sockaddr*)&serv_adr, sizeof(serv_adr));
	listen(serv_sd, 5);
	
	clnt_adr_sz=sizeof(clnt_adr);    
	clnt_sd=accept(serv_sd, (struct sockaddr*)&clnt_adr, &clnt_adr_sz);
	
	read(clnt_sd, file_name, BUF_SIZE);
	fp=fopen(file_name, "rb"); // rb read binary
	if(fp!=NULL)
	{
		while(1)
		{
			read_cnt=fread((void*)buf, 1, BUF_SIZE, fp); // num. of char we will read within 1*BUF_SIZE times from fp and put in buf
            printf("read_cnt %d", read_cnt);
			if(read_cnt<BUF_SIZE)
			{
				write(clnt_sd, buf, read_cnt); // if we put in less word than BUF_SIZE, read all read_cnt
				break;
			}
			write(clnt_sd, buf, BUF_SIZE); // otherwise read all BUF_SIZE
		}
	}
	
	fclose(fp);
	close(clnt_sd); close(serv_sd);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}