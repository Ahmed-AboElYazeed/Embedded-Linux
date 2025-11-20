//the code commented and add some checks by using ai

// client.c — Sends 2 integers to the server and receives the sum
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int sock;
    struct sockaddr_in server_addr;

    // 1. Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) { perror("socket"); exit(1); }

    // 2. Connect to localhost:8080
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        exit(1);
    }

    // 3. Input numbers from user
    int nums[2];
    printf("Enter first number: ");
    scanf("%d", &nums[0]);
    printf("Enter second number: ");
    scanf("%d", &nums[1]);

    // 4. Send numbers
    write(sock, nums, sizeof(nums));

    // 5. Receive sum
    int sum;
    read(sock, &sum, sizeof(sum));

    printf("Server returned sum = %d\n", sum);

    // 6. Cleanup
    close(sock);

    return 0;
}


