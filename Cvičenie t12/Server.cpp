#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int sockfd, newsockfd;
    socklen_t cli_len;
    struct sockaddr_in serv_addr, cli_addr;
    int n;
    char buffer[256];

    if (argc < 2) //1
    {
        fprintf(stderr,"usage %s port\n", argv[0]);
        return 1;
    }

    bzero((char*)&serv_addr, sizeof(serv_addr)); //2
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(argv[1]));

    sockfd = socket(AF_INET, SOCK_STREAM, 0); //3
    if (sockfd < 0)
    {
        perror("Error creating socket");
        return 1;
    }

    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) //4
    {
        perror("Error binding socket address");
        return 2;
    }

    listen(sockfd, 5); //5
    cli_len = sizeof(cli_addr);

    newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &cli_len); //6
    if (newsockfd < 0)
    {
        perror("ERROR on accept");
        return 3;
    }

    bzero(buffer,256);
    n = read(newsockfd, buffer, 255); //7
    if (n < 0)
    {
        perror("Error reading from socket");
        return 4;
    }
    printf("Here is the message: %s\n", buffer); //8

    const char* msg = "I got your message";
    n = write(newsockfd, msg, strlen(msg)+1); //9
    if (n < 0)
    {
        perror("Error writing to socket");
        return 5;
    }

    close(newsockfd);
    close(sockfd);

    return 0;
}

