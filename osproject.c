#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SIZE 1024

// =====================================================
// PART 1: FILE BACKUP
// Uses open(), read(), write(), close()
// =====================================================

void copyFile(const char *source, const char *destination) {

    int source_fd;
    int destination_fd;

    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;

    // Open source file
    source_fd = open(source, O_RDONLY);

    if (source_fd == -1) {
        perror("Error opening source file");
        return;
    }

    // Create destination file
    destination_fd = open(
        destination,
        O_WRONLY | O_CREAT | O_TRUNC,
        0644
    );

    if (destination_fd == -1) {
        perror("Error creating backup file");
        close(source_fd);
        return;
    }

    // Read source and write to destination
    while ((bytes_read = read(
                source_fd,
                buffer,
                BUFFER_SIZE)) > 0) {

        write(
            destination_fd,
            buffer,
            bytes_read
        );
    }

    // Close files
    close(source_fd);
    close(destination_fd);

    printf("[BACKUP] %s -> %s\n",
           source,
           destination);
}


// =====================================================
// PART 2: DIRECTORY MANAGEMENT
// Uses opendir(), readdir(), closedir()
// =====================================================

void backupDirectory(
    const char *sourceDir,
    const char *backupDir
) {

    DIR *directory;
    struct dirent *entry;

    // Open source directory
    directory = opendir(sourceDir);

    if (directory == NULL) {
        perror("Error opening directory");
        return;
    }

    // Create backup directory
    mkdir(backupDir, 0755);

    // Read directory entries
    while ((entry = readdir(directory)) != NULL) {

        // Ignore . and ..
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0) {

            continue;
        }

        char sourcePath[500];
        char backupPath[500];

        // Create complete paths
        snprintf(
            sourcePath,
            sizeof(sourcePath),
            "%s/%s",
            sourceDir,
            entry->d_name
        );

        snprintf(
            backupPath,
            sizeof(backupPath),
            "%s/%s",
            backupDir,
            entry->d_name
        );

        struct stat fileInfo;

        // Get information about entry
        if (stat(sourcePath, &fileInfo) == -1) {
            perror("stat");
            continue;
        }

        // If it is a regular file
        if (S_ISREG(fileInfo.st_mode)) {

            // PART 2 calls PART 1
            copyFile(sourcePath, backupPath);
        }

        // If it is a directory
        else if (S_ISDIR(fileInfo.st_mode)) {

            // Recursively backup directory
            backupDirectory(
                sourcePath,
                backupPath
            );
        }
    }

    closedir(directory);
}


// =====================================================
// MAIN PROGRAM
// =====================================================

int main() {

    printf("=====================================\n");
    printf(" Linux File Backup System\n");
    printf("=====================================\n");

    printf("\nStarting backup...\n\n");

    backupDirectory(
        "source",
        "backup"
    );

    printf("\nBackup completed successfully.\n");

    return 0;
}