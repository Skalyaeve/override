#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

void log_wrapper(FILE *logFile, const char *msg, const char *filename)
{
    char dest[264];

    strcpy(dest, msg);
    snprintf(&dest[strlen(dest)], 254 - strlen(dest), filename);
    dest[strcspn(dest, "\n")] = 0;
    fprintf(logFile, "LOG: %s\n", dest);
    return;
}

int main(int argc, const char **argv)
{
    FILE *logFile;
    FILE *fileToBackup;
    int backupFd;
    char buf;
    char dest[104];

    if (argc != 2)
        printf("Usage: %s filename\n", *argv);

    logFile = fopen("./backups/.log", "w");
    if (!logFile)
    {
        printf("ERROR: Failed to open %s\n", "./backups/.log");
        exit(1);
    }

    log_wrapper(logFile, "Starting back up: ", argv[1]);

    fileToBackup = fopen(argv[1], "r");
    if (!fileToBackup)
    {
        printf("ERROR: Failed to open %s\n", argv[1]);
        exit(1);
    }

    strcpy(dest, "./backups/");
    strncat(dest, argv[1], 99 - strlen(dest));
    backupFd = open(dest, O_WRONLY | O_CREAT | O_EXCL, 00660);
    if (backupFd < 0)
    {
        printf("ERROR: Failed to open %s%s\n", "./backups/", argv[1]);
        exit(1);
    }

    buf = -1;
    while (1)
    {
        buf = fgetc(fileToBackup);
        if (buf == -1)
            break;
        write(backupFd, &buf, 1uLL);
    }

    log_wrapper(logFile, "Finished back up ", argv[1]);

    fclose(fileToBackup);
    close(backupFd);
    return 0;
}
