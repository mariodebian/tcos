#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>

/*

  Based on: http://www.perlmonks.org/?node_id=597765

*/

void daemonize(void) {
    int fd;
    if (setsid() == -1) {
        fprintf(stderr, "couldn't create new processgroup exiting");
        exit(4);
    }
    /* close stdin stdout stderr*/
    if ((fd = open("/dev/null", O_RDWR, 0)) != -1) {
        (void)dup2(fd, STDIN_FILENO);
        (void)dup2(fd, STDOUT_FILENO);
        (void)dup2(fd, STDERR_FILENO);
        if (fd > STDERR_FILENO)
            (void)close(fd);
    } else {
        fprintf(stderr, "couldn't close stdout, stderr, stdin\n");
    }
}

int main(int argc, char** argv) {
    pid_t pid;

    pid = fork();
    if (pid == 0) {
        
        if (chdir("/\0") != 0) {
            exit(5);
        }

        daemonize();

        char *proc = argv[1];
        char *args[3];
        args[0] = argv[1];
        args[1] = argv[2];
        args[2] = NULL;
        
        
        char* envi[0 + 1];
        
        envi[0] = NULL;
        return execve(proc, args, envi);
        
    } else if (pid == -1) {
        fprintf(stderr, "couldn't fork, exiting\n");
        exit(3);
    }
    
    exit(0);
}





