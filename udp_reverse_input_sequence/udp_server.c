#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

# define PORT 8080
# define BUFFER_SIZE 1024

void reverseString(char *str) {
    int n = strlen(str);
    for (int i = 0 ; i <n/2 ; ++i) {
        char temp = str[i];
        str[i] = str[n-i-1];
        str[n-i-1] = temp;
    }
}

int main() {
    int server_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addrlen = sizeof(server_addr);
    char buffer[BUFFER_SIZE] = {0};

    // creating a socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // binding the socket to the specific port
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    
    printf("UDP Server listening on port %d...\n",PORT);

    while(1) {
        int recv_len = recvfrom(server_fd, buffer, BUFFER_SIZE,0,(struct sockaddr*)&client_addr, &addrlen);
        if (recv_len == -1) {
            perror("Recive failed");
            exit(EXIT_FAILURE);
        }
        buffer[recv_len] = '\0';
        printf("Received from client: %s\n",buffer);

        // Revese the string
        reverseString(buffer);

        // send the reverse string back to the client
        sendto(server_fd, buffer, strlen(buffer), 0, (struct sockaddr*)&client_addr, addrlen);
        printf("Reversed string sent to client : %s\n",buffer);

        // clear buffer for next iteration
        memset(buffer, 0, BUFFER_SIZE);
    }

    close(server_fd);
    return 0;
}
