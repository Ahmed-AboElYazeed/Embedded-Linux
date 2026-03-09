//the code commented and add some checks by using ai

// server.c — Local server that receives 2 integers and returns their sum
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // 1. Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { perror("socket"); exit(1); }

    // 2. Bind to port 8080 on localhost
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        exit(1);
    }

    // 3. Listen for incoming connections
    listen(server_fd, 1);
    printf("Server running... waiting for client.\n");

    // 4. Accept a client
    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
    if (client_fd < 0) { perror("accept"); exit(1); }

    // 5. Receive two integers
    int nums[2];
    read(client_fd, nums, sizeof(nums));

    printf("Server received numbers: %d and %d\n", nums[0], nums[1]);

    // 6. Compute sum
    int sum = nums[0] + nums[1];
    printf("Sending sum: %d back to client.\n", sum);

    // 7. Send the result
    write(client_fd, &sum, sizeof(sum));

    // 8. Cleanup
    close(client_fd);
    close(server_fd);

    return 0;
}

