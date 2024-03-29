#include "k_s_definitions.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printError("Klienta je nutne spustit s nasledujucimi argumentmi: adresa port.");
    }
    
    //ziskanie adresy a portu servera <netdb.h>
    struct hostent *server = gethostbyname(argv[1]);
    if (server == NULL) {
        printError("Server neexistuje.");
    }
    int port = atoi(argv[2]);
	if (port <= 0) {
		printError("Port musi byt cele cislo vacsie ako 0.");
	}
    
    //vytvorenie socketu <sys/socket.h>
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        printError("Chyba - socket.");        
    }
    
    //definovanie adresy servera <arpa/inet.h>
    struct sockaddr_in serverAddress;
    bzero((char *)&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serverAddress.sin_addr.s_addr, server->h_length);
    serverAddress.sin_port = htons(port);

    if (connect(sock,(struct sockaddr *) &serverAddress, sizeof(serverAddress)) < 0) {
        printError("Chyba - connect.");        
    }
    
    printf("Spojenie so serverom bolo nadviazane.\n");
    char buffer[BUFFER_LENGTH + 1];
    buffer[BUFFER_LENGTH] = '\0';
    int koniec = 0;
    while (!koniec) {
        fgets(buffer, BUFFER_LENGTH, stdin);
        char* pos = strchr(buffer, '\n');
        if (pos != NULL) {
            *pos = '\0';
        }
        //zapis dat do socketu <unistd.h>
		write(sock, buffer, strlen(buffer) + 1);
        if (strcmp(buffer, endMsg) != 0) {
            //citanie dat zo socketu <unistd.h>
			read(sock, buffer, BUFFER_LENGTH);
            printf("Server poslal nasledujuce data:\n%s\n", buffer);
        }
        else {
            koniec = 1;
        }
    }
    //uzavretie socketu <unistd.h>
    close(sock);
    printf("Spojenie so serverom bolo ukoncene.\n");
    
    return (EXIT_SUCCESS);
}