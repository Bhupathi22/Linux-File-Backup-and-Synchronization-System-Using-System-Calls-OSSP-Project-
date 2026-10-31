#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main() {

    int fd;
    char buffer[100];

    printf("=====================================\n");
    printf("        CO-3 NAMED PIPE / FIFO\n");
    printf("=====================================\n");

    printf("\nBackup Manager waiting for command...\n");

    // Open FIFO for reading
    fd = open("backup_fifo", O_RDONLY);

    if (fd == -1) {

        perror("FIFO open failed");
        return 1;
    }

    // Read command
    read(
        fd,
        buffer,
        sizeof(buffer)
    );

    printf("\nCommand received:\n");
    printf("%s\n", buffer);

    // Close FIFO
    close(fd);

    printf("\nFIFO communication completed.\n");

    return 0;
}