#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

/**
 * main - Print a string using inline assembly and Linux syscalls.
 */
int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s \"<string>\"\n", argv[0]);
        return EXIT_FAILURE;
    }

    const char *msg = argv[1];
    size_t len = strlen(msg);

    // --- print message ---
    __asm__ __volatile__ (
        "mov $1, %%rax\n"      // syscall: write
        "mov $1, %%rdi\n"      // fd = 1 (stdout)
        "syscall\n"
        :
        : "S"(msg), "d"(len)   // S = RSI, d = RDX
        : "rax", "rdi"
    );

    // --- print newline ---
    const char nl = '\n';
    __asm__ __volatile__ (
        "mov $1, %%rax\n"      // syscall: write
        "mov $1, %%rdi\n"      // stdout
        "syscall\n"
        :
        : "S"(&nl), "d"(1)
        : "rax", "rdi"
    );

    return EXIT_SUCCESS;
}

