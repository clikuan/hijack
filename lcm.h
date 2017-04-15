#include <stdio.h>
#include <fcntl.h> 
#include <stdlib.h>
#include <dlfcn.h>
#include <sys/types.h>
#include <string.h>
#include <pwd.h>
#include <dirent.h>
#include <stdarg.h>
#include <grp.h>
#include <sys/mman.h>
#include <errno.h>
#include <sys/param.h>
#include <sys/stat.h>
#include <unistd.h>

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

int (*old_execve)(const char *, char *const[], char *const[]);
int execve(const char *, char *const[], char *const[]);

int (*old_execvp)(const char *, char *const[]);
int execvp(const char *, char *const[]);

int (*old_fchdir)(int);
int fchdir(int);

int (*old_fchown)(int, uid_t, gid_t);
int fchown(int, uid_t, gid_t);

pid_t (*old_fork)(void);
pid_t fork(void);

int (*old_fsync)(int);
int fsync(int);

int (*old_ftruncate)(int, off_t);
int ftruncate(int, off_t);

char* getcwd(char *, size_t);
char* (*old_getcwd)(char *, size_t);

gid_t (*old_getegid)(void);
gid_t getegid(void);

uid_t (*old_geteuid)(void);
uid_t geteuid(void);

gid_t (*old_getgid)(void);
gid_t getgid(void);

int (*old_link)(const char *, const char *);
int link(const char *, const char *);

int (*old_pipe)(int[]);
int pipe(int[]);

ssize_t (*old_pread)(int, void *, size_t, off_t);
ssize_t pread(int, void *, size_t, off_t);

ssize_t (*old_pwrite)(int, const void *, size_t, off_t);
ssize_t pwrite(int, const void *, size_t, off_t);

ssize_t (*old_read)(int, void *, size_t);
ssize_t read(int, void *, size_t);

ssize_t (*old_readlink)(const char *, char *, size_t);
ssize_t readlink(const char *, char *, size_t);

int (*old_rmdir)(const char *);
int rmdir(const char *);

int (*old_setegid)(gid_t);
int setegid(gid_t);

int (*old_seteuid)(uid_t);
int seteuid(uid_t);

int (*old_setgid)(gid_t);
int setgid(gid_t);

int (*old_setuid)(uid_t);
int setuid(uid_t);

unsigned int (*old_sleep)(unsigned int);
unsigned int sleep(unsigned int);

int (*old_symlink)(const char *, const char *);
int symlink(const char *, const char *);

int (*old_unlink)(const char *);
int unlink(const char *);

ssize_t (*old_write)(int, const void *, size_t);
ssize_t write(int, const void *, size_t);

int (*old_chmod)(const char *, mode_t);
int chmod(const char *, mode_t);

int (*old_fchmod)(int, mode_t);
int fchmod(int, mode_t);

int (*old___fxstat)(int, int, struct stat *);
int __fxstat(int, int, struct stat *);

int (*old___xstat)(int, const char *, struct stat *);
int __xstat(int ,const char *, struct stat *);

int (*old___lxstat)(int, const char *, struct stat *);
int __lxstat(int, const char *, struct stat *);

int (*old_mkdir)(const char *, mode_t);
int mkdir(const char *, mode_t);

int (*old_mkfifo)(const char *, mode_t);
int mkfifo(const char *, mode_t);

mode_t (*old_umask)(mode_t);
mode_t umask(mode_t);

FILE *(*old_popen)(const char *, const char *);
FILE *popen(const char *, const char *);

int (*old_pclose)(FILE *);
int pclose(FILE *);

void* (*old_realloc)(void *, size_t);
void* realloc(void *, size_t);

int (*old_puts)(const char *);
int puts(const char *);

int (*old_feof)(FILE *);
int feof(FILE *);

void (*old_rewind)(FILE *);
void rewind(FILE *);



