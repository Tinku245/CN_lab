#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#define SERV_TCP_PORT 5035
#define MAX 60
int main(int arg, char *argv[])
{
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char send[MAX], recvline[MAX], s[MAX], name[MAX];
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(SERV_TCP_PORT);
    connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    printf("Enter the file name : ");
    scanf("%s", name);
    write(sockfd, name, MAX);
    printf("File contents are : \n");
    while (read(sockfd, recvline, MAX) > 0)
    {
        printf("%s", recvline);
    }
    printf("\n");
    close(sockfd);
    return 0;
}