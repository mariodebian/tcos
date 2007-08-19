/*
** print_server.c -- a jetdirect emulator
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MYPORT 9100	// printer port

#define BACKLOG 10	 // how many pending connections queue will hold


#define BUFSIZE 1024




int main(int argc, char **argv)
{
	int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
	struct sockaddr_in my_addr;	// my address information
	struct sockaddr_in their_addr; // connector's address information
	socklen_t sin_size;
	int yes=1;
	
	char *buf;
	int n_bytes;
    

    if(argc != 2) {
        printf("Error: need a printer ex:  \n\n\t%s /dev/lp0\n", argv[0]);
        exit(1);
    }

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
		exit(1);
	}

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1) {
		perror("setsockopt");
		exit(1);
	}
	
	my_addr.sin_family = AF_INET;		 // host byte order
	my_addr.sin_port = htons(MYPORT);	 // short, network byte order
	my_addr.sin_addr.s_addr = INADDR_ANY; // automatically fill with my IP
	memset(my_addr.sin_zero, '\0', sizeof my_addr.sin_zero);

	if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) {
		perror("bind");
		exit(1);
	}

	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

    

	while(1) {  // main accept() loop
		sin_size = sizeof(struct sockaddr_in);
		if ((new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size)) == -1) {
			perror("accept");
			continue;
		}
        printf("print_server: got connection from %s\n",inet_ntoa(their_addr.sin_addr));
        FILE *dev;
        dev = fopen( argv[1] ,"wb");

        while(1){
            n_bytes=recv(new_fd, buf, BUFSIZE, 0);
            /*printf("nbytes=%d buf=%s\n", n_bytes, buf);*/
            if(n_bytes>0){
                n_bytes=recv(new_fd, buf, n_bytes, 0);
                fwrite(buf, n_bytes, 1, dev);
            }

            /*if (n_bytes == 1 ) {
                printf("catch EOF\n");
                break;
            }*/
            if(n_bytes==0) {
                printf("print_server: connection closed\n");
                break;
            }
            if(n_bytes==-1) printf("print_server error in recv: %d => %s",errno, strerror(errno));
        }
        fclose(dev);
        close(new_fd);
	}

	return 0;
}

