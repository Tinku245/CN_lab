//server

#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <netinet/in.h>
#include <string.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#define MAX 1024
char buffer[MAX];
void send_file_data(FILE *fp, int sockfd, struct sockaddr_in addr)
{
    int n;
    while (fgets(buffer, MAX, fp) != NULL)
    {
        n = sendto(sockfd, buffer, MAX, 0, (struct sockaddr *)&addr, sizeof(addr));
        if (n == -1)
        {
            perror("[ERROR] sending data to the server.");
            exit(1);
        }
        bzero(buffer, MAX);
    }
    strcpy(buffer, "END");
    sendto(sockfd, buffer, MAX, 0, (struct sockaddr *)&addr, sizeof(addr));

    fclose(fp);
}

int main()
{

    int serverDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
    char error[] = "end\n", add[] = " >> output.txt";
    struct sockaddr_in clientAddress, serverAddress;

    socklen_t clientLength = sizeof(clientAddress);

    bzero(&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(9976);

    bind(serverDescriptor, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    while (1)
    {
        bzero(buffer, sizeof(buffer));
        recvfrom(serverDescriptor, buffer, sizeof(buffer), 0, (struct sockaddr *)&clientAddress, &clientLength);
        if (strcmp(buffer, error) == 0)
        {
            close(serverDescriptor);
            exit(0);
        };
        int n = 0;
        for (int i = 0; i < MAX; i++)
        {
            if (buffer[i] == '\n')
                break;
            n++;
        }
        char command[n + 17];
        for (int i = 0; i < n; i++)
        {
            command[i] = buffer[i];
        }
        command[n] = '\0';
        strcat(command, add);
        remove("output.txt");
        system(command);
        FILE *fp = fopen("output.txt", "r");
        send_file_data(fp, serverDescriptor, clientAddress);
        printf("Command Executed ...");
    }
    close(serverDescriptor);
    return 0;
}