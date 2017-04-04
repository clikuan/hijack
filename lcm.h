#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#include <dirent.h>

uid_t (*old_getuid)(void) = NULL;
uid_t getuid(void);

char* (*old_getenv)(const char*) = NULL;
char* getenv(const char *name);

int (*old_closedir)(DIR *) = NULL;
int closedir(DIR *);

int (*old_close)(int);
int close(int);
