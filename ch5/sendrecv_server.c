// this is exercise 5.4.5 client/server send certain format to each other
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

void error_handling(char *message);

int main(int argc, char *argv[])
{
	int serv_sock;
	int clnt_sock;
	int str_len, i;

	struct sockaddr_in serv_addr;
	struct sockaddr_in clnt_addr;
	socklen_t clnt_addr_size;

	char msg1[]="Hello client!";
	char msg2[]="I'm server.";
	char msg3[]="Nice to meet you.";
	char* str_arr[]={msg1, msg2, msg3};
	char read_buf[100];
	
	if(argc!=2){
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
	
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);
	if(serv_sock == -1)
		error_handling("socket() error");
	
	memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;
	serv_addr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_addr.sin_port=htons(atoi(argv[1]));
	
	if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))==-1)
		error_handling("bind() error"); 
	
	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");
	
	clnt_addr_size=sizeof(clnt_addr);  
	clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_addr,&clnt_addr_size);
	if(clnt_sock==-1)
		error_handling("accept() error");  
	
	for(i=0; i<3; i++)
	{
		str_len=strlen(str_arr[i])+1;
		write(clnt_sock, (char*)(&str_len), 4);//传递字符串之前，先以4字节整数型方式传递字符串长度
		write(clnt_sock, str_arr[i], str_len);//传递字符串，第三个位置是写出去的字符串长度
		
		read(clnt_sock, (char*)(&str_len), 4);//读取字符串长度
		read(clnt_sock, read_buf, str_len);//读取字符串，第三个位置是拟读进来的字符串长度
		puts(read_buf);
	}
	
	close(clnt_sock);
	close(serv_sock);
	return 0;
}

void error_handling(char *message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}