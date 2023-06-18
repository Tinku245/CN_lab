//client

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#define MAX 1024
#define SIZE 1024
void recv_file(int sockfd, struct sockaddr_in addr)
{

    int n;
    char buffer[SIZE];
    socklen_t addr_size;
    while (1)
    {
        addr_size = sizeof(addr);
        n = recvfrom(sockfd, buffer, SIZE, 0, (struct sockaddr *)&addr, &addr_size);

        if (strcmp(buffer, "END") == 0)
        {
            break;
        }
        printf("%s", buffer);
        bzero(buffer, SIZE);
    }
}
int main()
{
    int serverDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
    char buffer[MAX], message[MAX];
    struct sockaddr_in cliaddr, serverAddress;
    socklen_t serverLength = sizeof(serverAddress);

    bzero(&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr("127.0.0.1");
    serverAddress.sin_port = htons(9976);

    bind(serverDescriptor, (struct sockaddr *)&serverAddress, sizeof(serverAddress));

    while (1)
    {
        printf("\nCOMMAND FOR EXECUTION ... ");
        fgets(buffer, sizeof(buffer), stdin);
        sendto(serverDescriptor, buffer, sizeof(buffer), 0, (struct sockaddr *)&serverAddress, serverLength);
        if (strcmp(buffer, "end\n") == 0)
        {
            close(serverDescriptor);
            exit(0);
        }
        recv_file(serverDescriptor, serverAddress);
    }
    return 0;
}