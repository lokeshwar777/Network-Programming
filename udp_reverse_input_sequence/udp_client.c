#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

# define PORT 8080
# define BUFFER_SIZE 1024

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // creating socket file descriptor
    if ((client_fd = socket(AF_INET, SOCK_DGRAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    while (1) {
        printf("Enter a sentence (Type 'exit' to quit) : ");
        fgets(buffer, BUFFER_SIZE, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        
        if(strncmp(buffer,"exit",4) == 0) {
            printf("Exiting client...");
            break;
        }

        // send data to server
        sendto(client_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&server_addr, sizeof(server_addr));


        // Receive the reversed string from server
        int recv_len = recvfrom(client_fd, buffer, BUFFER_SIZE,0,NULL, NULL);
        buffer[recv_len] = '\0';
         
        printf("Reversed string received from server : %s\n",buffer);
    }

    close(client_fd);

    return 0;
}
