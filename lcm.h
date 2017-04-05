#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#include <dirent.h>
#include <stdarg.h>
#include <fcntl.h>

uid_t (*old_getuid)(void);
uid_t getuid(void);

char* (*old_getenv)(const char*);
char* getenv(const char *name);

int (*old_closedir)(DIR *);
int closedir(DIR *);

int (*old_close)(int);
int close(int);

DIR* (*old_opendir)(const char*);
DIR* opendir(const char *);

DIR* (*old_fdopendir)(int);
DIR* fdopendir(int);

struct dirent* (*old_readdir)(DIR *);
struct dirent *readdir(DIR *);

int (*old_readdir_r)(DIR *, struct dirent *, struct dirent **);
int readdir_r(DIR *, struct dirent *, struct dirent **);

void (*old_rewinddir)(DIR *);
void rewinddir(DIR *);

void (*old_seekdir)(DIR *, long);
void seekdir(DIR *, long);

long (*old_telldir)(DIR *);
long telldir(DIR *);

int (*old_creat)(const char *, mode_t);
int creat(const char *, mode_t);

int (*old_open)(const char *, int, ...);
int open(const char *, int, ...);      

int (*old_remove)(const char *);
int remove(const char *);

int (*old_rename)(const char *, const char *);
int rename(const char *, const char *);

void (*old_setbuf)(FILE *, char *);
void setbuf(FILE *, char *);




