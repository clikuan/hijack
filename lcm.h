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
#include <grp.h>
#include <sys/mman.h>
#include <errno.h>
#include <sys/param.h>


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

int (*old_setvbuf)(FILE *, char *, int ,size_t);
int setvbuf(FILE *, char *, int ,size_t);

char* (*old_tempnam)(const char *, const char *);
char* tempnam(const char *, const char *);

FILE* (*old_tmpfile)(void);
FILE* tmpfile(void);

char* (*old_tmpnam)(char *);
char* tmpnam(char *);

void (*old_exit)(int);
void exit(int);

char* (*old_mkdtemp)(char *);
char* mkdtemp(char *);

int (*old_mkstemp)(char *);
int mkstemp(char *);

int (*old_putenv)(char *);
int putenv(char *);

int (*old_rand)(void);
int rand(void);

int (*old_rand_r)(unsigned int *);
int rand_r(unsigned int *);

int (*old_setenv)(const char *, const char *, int);
int setenv(const char *, const char *, int);

void (*old_srand)(unsigned int);
void srand(unsigned int);

int (*old_system)(const char *);
int system(const char *);

int (*old_chdir)(const char *);
int chdir(const char *);

int (*old_chown)(const char *, uid_t, gid_t);
int chown(const char *, uid_t, gid_t);

int (*old_dup)(int);
int dup(int);

int (*old_dup2)(int, int);
int dup2(int, int);

void (*old__exit)(int);
void _exit(int);

int (*old_execl)(const char *, const char *, ...);
int execl(const char *, const char *, ...);

int (*old_execle)(const char *, const char *, ...);
int execle(const char *, const char *, ...);

int (*old_execlp)(const char *, const char *, ...);
int execlp(const char *, const char *, ...);

int (*old_execv)(const char *, char *const []);
int execv(const char *, char *const []);




