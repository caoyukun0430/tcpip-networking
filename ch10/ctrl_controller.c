#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

void ctrl_handler(int sig);

int main(int argc, char *argv[])
{
	struct sigaction act;
	act.sa_handler=ctrl_handler;
	sigemptyset(&act.sa_mask);
	act.sa_flags=0;
	sigaction(SIGINT, &act, 0);

	while(1)
	{
		sleep(1);
		puts("Have a nice day~");
	}

	return 0;
}


void ctrl_handler(int sig)
{
	char ex;
	fputs("Do you want exit(Y to exit)? ", stdout);
	scanf("%c", &ex);
	if(ex=='y' || ex=='Y')
		exit(1);
}

