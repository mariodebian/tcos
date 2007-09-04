#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
	int pid;
        int devnull;

	if(fork())
           return 0;

	chdir("/");
	setsid();
	umask(0);
	pid = fork();
	if(pid) {
           exit(0);
        }

        devnull = open("/dev/null", O_RDWR);
        close(STDIN_FILENO); dup2(STDIN_FILENO, devnull);
        close(STDOUT_FILENO); dup2(STDOUT_FILENO, devnull);
        close(STDOUT_FILENO); dup2(STDERR_FILENO, devnull);
        close(devnull);

	/****** launch daemon ********/
	//system(argv[1]);
        //char *cmd[] = { "sleep", "60", (char *)0 };
        execlp("sh", "sh", "-c", argv[1], (char *)NULL );
        //execvp(*argv, argv);
        //execl(argv[1], argv[2]);
	/**********************************/
        return 0;
}

