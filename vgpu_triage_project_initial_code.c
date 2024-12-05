/*

Author: Shivkumar Ople (NVIDIA Inc.)
Program: To extract the VMware Support Bundle to Analyze logs
Status: Work in Progress

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

int file_exist(const char *filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

void create_directory(const char *dir) {
    struct stat st = {0};
    if (stat(dir, &st) == -1) {
        if (mkdir(dir, 0755) != 0) {
            perror("Failed to create directory");
            exit(1);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s support_bundle_file target_directory\n", argv[0]);
        return 1;
    }

    const char *support_bundle = argv[1];
    const char *target_dir = argv[2];

    if (!file_exist(support_bundle)) {
        fprintf(stderr, "Error: Support bundle file '%s' not found.\n", support_bundle);
        return 1;
    }

    create_directory(target_dir);

    char command[1024];
    snprintf(command, sizeof(command),
             "tar -xzf %s --wildcards --no-anchored '*/var/core/*' --strip-components=2 -C %s",
             support_bundle, target_dir);

    printf("Extracting /var/core directory from the support bundle...\n");
    int ret = system(command);

    if (ret == 0) {
        printf("Extraction complete. Files are in '%s'.\n", target_dir);
    } else {
        fprintf(stderr, "Error during extraction. Command returned %d.\n", ret);
        return 1;
    }

    return 0;
}
