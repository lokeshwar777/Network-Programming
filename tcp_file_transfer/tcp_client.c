#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

# define PORT 8080
# define BUFFER_SIZE 1024

void receiveFile(FILE *file, int socket) {
    char buffer[BUFFER_SIZE];
    size_t bytesRead;

    while ((bytesRead = recv(socket, buffer, sizeof(buffer), 0)) > 0){
        fwrite(buffer, 1, bytesRead, file);
    } 
}

int main() {
    int sock = 0;
    struct sockaddr_in serv_addr;
    
    // creating a socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\nSocket creation failed\n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary
    if (inet_pton(AF_INET, "127.0.0.1",&serv_addr.sin_addr) <= 0) {
        printf("\nInvalid Address or Address not supported\n");
        return -1;
    }

    // Connect to server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection failed\n");
        return -1;
    }

    FILE *file = fopen("received_file.txt","wb");

    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    receiveFile(file, sock);
    printf("File received successfully");
    fclose(file);
    close(sock);
    return 0;
}
