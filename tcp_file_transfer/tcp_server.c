#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

# define PORT 8080
# define BUFFER_SIZE 1024

void sendFile(FILE *file, int socket) {
    char buffer[BUFFER_SIZE];
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0)
    {
        if (send(socket, buffer, bytesRead, 0) == -1) {
            perror("Error sending file");
            exit(EXIT_FAILURE);
        }
    }
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    socklen_t addrlen = sizeof(address);

    // creating a socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // forcefully attaching socket to the port 8080
    // if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
    //     perror("setsockopt");
    //     exit(EXIT_FAILURE);
    // }
    

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd,3) < 0) {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    }
    
    printf("Server listening on port %d...\n",PORT);

    if ((new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen))== -1) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    FILE *file = fopen("sample.txt","rb");

    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    sendFile(file, new_socket);
    printf("File sent successfully");
    fclose(file);
    close(new_socket);
    return 0;
}
