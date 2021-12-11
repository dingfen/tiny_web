
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define MAXLINE 127

void unix_error(char *msg) /* Unix-style error */
{
    fprintf(stderr, "%s: %s\n", msg, strerror(errno));
    exit(0);
}

int main(int argc, char const *argv[])
{
    int sockfd, n;
    char recvline[MAXLINE +1];
    struct sockaddr_in servaddr;

    if (argc != 2)
        unix_error("Usage: a.out <IPaddress>");
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        unix_error("socket error");
    }

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(13);

    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
        unix_error("inet_pton error");
    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) < 0)
        unix_error("connect error");
    
    while ((n = read(sockfd, recvline, MAXLINE)) > 0) {
        recvline[n] = 0;
        if (fputs(recvline, stdout) == EOF)
            unix_error("fputs error");
    }

    if (n < 0) {
        unix_error("read error");
    }
    
    return 0;
}
