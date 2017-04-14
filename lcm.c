#include "lcm.h"

void *dlHandle;
char *outputFileName;
FILE *outputFile;

static  __attribute__((constructor)) void setup(void){
	dlHandle = dlopen("libc.so.6", RTLD_LAZY);
	
	old_getenv = dlsym(dlHandle, "getenv");
	old_getuid = dlsym(dlHandle, "getuid");
	old_closedir = dlsym(dlHandle, "closedir");	
	old_close = dlsym(dlHandle, "close");
	old_fdopendir = dlsym(dlHandle, "fdopendir");
	old_opendir = dlsym(dlHandle, "opendir");
	old_readdir = dlsym(dlHandle, "readdir");
	old_readdir_r = dlsym(dlHandle, "readdir_r");
	old_rewinddir = dlsym(dlHandle, "rewinddir");
	old_seekdir = dlsym(dlHandle, "seekdir");	
	old_telldir = dlsym(dlHandle, "telldir");
	old_creat = dlsym(dlHandle, "creat");
	old_open = dlsym(dlHandle, "open");
	old_remove = dlsym(dlHandle, "remove");
	old_rename = dlsym(dlHandle, "rename");
	old_setbuf = dlsym(dlHandle, "setbuf");
	old_setvbuf = dlsym(dlHandle, "setvbuf");
	old_tempnam = dlsym(dlHandle, "tempnam");
	old_tmpfile = dlsym(dlHandle, "tmpfile");
	old_tmpnam = dlsym(dlHandle, "tmpnam");
	old_exit = dlsym(dlHandle, "exit");
	old_mkdtemp = dlsym(dlHandle, "mkdtemp");
	old_mkstemp = dlsym(dlHandle, "mkstemp");	
	old_putenv = dlsym(dlHandle, "putenv");
	old_rand = dlsym(dlHandle, "rand");
	old_rand_r = dlsym(dlHandle, "rand_r");
	old_setenv = dlsym(dlHandle, "setenv");
	old_srand = dlsym(dlHandle, "srand");
	old_system = dlsym(dlHandle, "system");
	old_chdir = dlsym(dlHandle, "chdir");
	old_chown = dlsym(dlHandle, "chown");
	old_dup = dlsym(dlHandle, "dup");
	old_dup2 = dlsym(dlHandle, "dup2");
	old__exit = dlsym(dlHandle, "_exit");
	old_execl = dlsym(dlHandle, "execl");
	old_execle = dlsym(dlHandle, "execle");
	old_execlp = dlsym(dlHandle, "execlp");
	old_execv = dlsym(dlHandle, "execv");
	old_execve = dlsym(dlHandle, "execve");
	old_execvp = dlsym(dlHandle, "execvp");
	old_fchdir = dlsym(dlHandle, "fchdir");
	old_fchown = dlsym(dlHandle, "fchown");
	old_fork = dlsym(dlHandle, "fork");
	old_fsync = dlsym(dlHandle, "fsync");
	old_ftruncate = dlsym(dlHandle, "ftruncate");
	old_getcwd = dlsym(dlHandle, "getcwd");
	old_getegid = dlsym(dlHandle, "getegid");
	old_geteuid = dlsym(dlHandle, "geteuid");
	old_getgid = dlsym(dlHandle, "getgid");
	old_link = dlsym(dlHandle, "link");
	old_pipe = dlsym(dlHandle, "pipe");
	old_pread = dlsym(dlHandle, "pread");
	old_pwrite = dlsym(dlHandle, "pwrite");
	old_read = dlsym(dlHandle, "read");
	old_readlink = dlsym(dlHandle, "readlink");
	old_rmdir = dlsym(dlHandle, "rmdir");	
	old_setegid = dlsym(dlHandle, "setegid");
	old_seteuid = dlsym(dlHandle, "seteuid");
	old_setgid = dlsym(dlHandle, "setgid");
	old_setuid = dlsym(dlHandle, "setuid");
	old_sleep = dlsym(dlHandle, "sleep");
	old_symlink = dlsym(dlHandle, "symlink");
	old_unlink = dlsym(dlHandle, "unlink");
	old_write = dlsym(dlHandle, "write");
	old_chmod = dlsym(dlHandle, "chmod");
	old_fchmod = dlsym(dlHandle, "fchmod");
	old___fxstat = dlsym(dlHandle, "__fxstat");
	old___xstat = dlsym(dlHandle, "__xstat");
	old___lxstat = dlsym(dlHandle, "__lxstat");
	old_mkdir = dlsym(dlHandle, "mkdir");
	old_mkfifo = dlsym(dlHandle, "mkfifo");
	old_umask = dlsym(dlHandle, "umask");
	old_popen = dlsym(dlHandle, "popen");	
	old_pclose = dlsym(dlHandle, "pclose");
	old_realloc = dlsym(dlHandle, "realloc");
	old_puts = dlsym(dlHandle, "puts");
	old_feof = dlsym(dlHandle, "feof");

	outputFileName = old_getenv("MONITOR_OUTPUT");
	outputFile = NULL;
	if(dlHandle == NULL){
		fprintf(stderr, "open libc.so.6 failed\n");
	}
	if(outputFileName != NULL){
		if(strcmp(outputFileName, "stderr")){
			if((outputFile = fopen(outputFileName, "w")) == NULL){
				fprintf(stderr, "can not open file:%s\n", outputFileName);
				exit(1);
			}
		}
	}
	/*else{
		fprintf(stderr, "variable MONITOR_OUTPUT is not specify\n");
		exit(1);
	}*/
}
static __attribute__((destructor)) void release(void){
	if(outputFile)
		old_close(outputFile);
	dlclose(dlHandle);
}
#define GEN_CODE(fName, fmt_r, result, fmt_p, ...){	\
			fprintf((outputFile ? outputFile : stderr), "[monitor] "#fName"(");	\	
			fprintf((outputFile ? outputFile : stderr), fmt_p, __VA_ARGS__);	\
			fprintf((outputFile ? outputFile : stderr), ") = "fmt_r"\n",result);    \
}
#define GEN_CODE_0(type, fName){	\
			type oldRes = old_##fName();	\
			fprintf((outputFile ? outputFile : stderr), "[monitor] "#fName"(");	\	
			if(strcmp(#type, "char*") == 0 || strcmp(#type, "FILE*") == 0){	\	
				fprintf((outputFile ? outputFile : stderr), ") = %p\n", oldRes);    \
			} \
			else if(strcmp(#type, "int") == 0 || strcmp(#type, "pid_t") == 0 || strcmp(#type, "uid_t") == 0 || \ 
						strcmp(#type, "gid_t") == 0	){	\
				fprintf((outputFile ? outputFile : stderr), ") = %d\n", oldRes);    \
			}	\
			return oldRes;  \
}
char* getUserNameFromUid(uid_t uid){
	struct passwd *pwd;
	if ((pwd = getpwuid(uid)) != NULL)
		return pwd->pw_name;
	else
		return "NULL";
}
char* getGroupNameFromGid(gid_t gid){
	struct group *g;
	if((g = getgrgid(gid)) != NULL)
		return g->gr_name;
	else
		return "NULL";
}
char *fd2Name(int fd){
	char fd_path[255];
	char * filename = malloc(255);
	ssize_t n;
	sprintf(fd_path, "/proc/self/fd/%d", fd);
	n = old_readlink(fd_path, filename, 255);
	if (n < 0)
		return "NULL";
	filename[n] = '\0';
	return filename;
}
char* getDirectoryName(DIR *dir){
	int fd = dirfd(dir);
	return fd2Name(fd);
}
char *getFileName(FILE * fptr){
	int fd = fileno(fptr);
	return fd2Name(fd);
}
char* getFileType(mode_t m){
	char *type = malloc(20);
	if(S_ISBLK(m)){
		strcpy(type, "Block");
	}
	else if(S_ISCHR(m)){
		strcpy(type, "Character");
	}
	else if(S_ISDIR(m)){
		strcpy(type, "Directory");
	}
	else if(S_ISFIFO(m)){
		strcpy(type, "FIFO");
	}
	else if(S_ISREG(m)){
		strcpy(type, "Regular");
	}
	else if(S_ISLNK(m)){
		strcpy(type, "Symbolic");
	}
	else if(S_ISSOCK(m)){
		strcpy(type, "Socket");
	}
	else{
		strcpy(type, "Unknow");
	}
	return type;
}
uid_t getuid(void){
	GEN_CODE_0(uid_t, getuid);
}
char* getenv(const char *name){
	char *result = old_getenv(name);
	GEN_CODE(getenv, "'%s'", result, "'%s'", name);
	return result;
}
int closedir(DIR *dirp){
	char *dirName = getDirectoryName(dirp);
	int result = old_closedir(dirp);
	GEN_CODE(closedir, "%d", result, "'%s'", dirName);
	return result;
}
int close(int fd){
	char *name = fd2Name(fd);
	int result = old_close(fd);
	GEN_CODE(close, "%d", result, "'%s'", name);
	return result;
}
DIR* opendir(const char *name){
	DIR *result = old_opendir(name);
	GEN_CODE(opendir, "%p", result, "'%s'", name);
	return result;
}
DIR* fdopendir(int fd){
	char *name = fd2Name(fd);
	DIR* result = old_fdopendir(fd);
	GEN_CODE(fdopendir, "%p", result, "%s", name);
	return result;
}
struct dirent* readdir(DIR *dirp){
	char *dirName = getDirectoryName(dirp);
	struct dirent* result = old_readdir(dirp);
	GEN_CODE(readdir, "%p", result, "'%s'", dirName);
	return result;
}
int readdir_r(DIR *dirp, struct dirent *entry, struct dirent **result){
	char *dirName = getDirectoryName(dirp);
	int res = old_readdir_r(dirp, entry, result);
	GEN_CODE(readdir_r, "%d", res, "'%s', %p, %p", dirName, entry, result);
	return res;
}
void rewinddir(DIR *dirp){
	char *dirName = getDirectoryName(dirp);
	old_rewinddir(dirp);
	GEN_CODE(rewinddir, "", NULL, "'%s'", dirName);
}
void seekdir(DIR *dirp, long loc){
	char *dirName = getDirectoryName(dirp);
	old_seekdir(dirp, loc);
	GEN_CODE(seekdir, "", NULL, "'%s', %ld", dirName, loc);
}
long telldir(DIR *dirp){
	char *dirName = getDirectoryName(dirp);
	long result =  old_telldir(dirp);
	GEN_CODE(telldir, "%ld", result, "'%s'", dirName);
	return result;
}
int creat(const char *pathname, mode_t mode){
	int result = old_creat(pathname, mode);
	GEN_CODE(creat, "%d", result, "'%s', %u", pathname, mode);
	return result;
}
int open(const char *pathname, int flags, ...){
	int result;
	mode_t mode;
	if (__OPEN_NEEDS_MODE(flags)){
		va_list arg;
		va_start(arg, flags);
		mode = va_arg(arg, mode_t);
		va_end(arg);
		result = old_open(pathname, flags, mode);
		GEN_CODE(open, "%d", result, "'%s', %d, %u", pathname, flags, mode);
	}
	else{
		result = old_open(pathname, flags);
		 GEN_CODE(open, "%d", result, "'%s', %d", pathname, flags);
	}
	return result;
}
int remove(const char *pathname){
	int result = old_remove(pathname);
	GEN_CODE(remove, "%d", result, "'%s'", pathname);
	return result;
}
int rename(const char *oldpath, const char *newpath){
	int result = old_rename(oldpath, newpath);
	GEN_CODE(rename, "%d", result, "'%s', '%s'", oldpath, newpath);
	return result;
}
void setbuf(FILE *stream, char *buf){
	char *filename = getFileName(stream); 
	old_setbuf(stream, buf);
	GEN_CODE(setbuf, "", NULL, "'%s', '%s'", filename, buf);
}
int setvbuf(FILE *stream, char *buf, int mode, size_t size){
	char *filename = getFileName(stream);
	int result = old_setvbuf(stream, buf, mode, size);
	GEN_CODE(setvbuf, "%d", result, "'%s', '%s', %d, %lu", filename, buf, mode, size);
	return result;
}
char* tempnam(const char *dir, const char *pfx){
	char *result = old_tempnam(dir, pfx);
	GEN_CODE(tempnam, "'%s'", result, "'%s', '%s'", dir, pfx);
	return result;
}
FILE* tmpfile(void){
	GEN_CODE_0(FILE*, tmpfile);
}
char *tmpnam(char *s){
	char *result = old_tmpnam(s);
	GEN_CODE(tmpnam, "'%s'", result, "'%s'", s);
	return result;
}
void exit(int status){
	GEN_CODE(exit, "", NULL, "%d", status);
	old_exit(status);
}
char* mkdtemp(char *template){
	char *result = old_mkdtemp(template);
	GEN_CODE(mkdtemp, "'%s'", result, "'%s'", template);
	return result;
}
int mkstemp(char *template){
	int result = old_mkstemp(template);
	GEN_CODE(mkstemp, "'%d'", result, "'%s'", template);
	return result;
}
int putenv(char *string){
	int result = old_putenv(string);
	GEN_CODE(putenv, "%d", result, "'%s'", string);
	return result;
}
int rand(void){
	GEN_CODE_0(int, rand);
}
int rand_r(unsigned int *seedp){
	int result = old_rand_r(seedp);
	GEN_CODE(rand_r, "%d", result, "%p", seedp);
	return result;
}
int setenv(const char *name, const char *value, int overwrite){
	int result = old_setenv(name, value, overwrite);
	GEN_CODE(setenv, "%d", result, "'%s', '%s', %d",name, value, overwrite);
	return result;
}
void srand(unsigned int seed){
	old_srand(seed);
	GEN_CODE(srand, "", NULL, "%u", seed);
}
int system(const char *command){
	int result = old_system(command);
	GEN_CODE(system, "%d", result, "'%s'", command);
	return result;
}
int chdir(const char *path){
	int result = old_chdir(path);
	GEN_CODE(chdir, "%d", result, "'%s'", path);
	return result;
}
int chown(const char *pathname, uid_t owner, gid_t group){
	char *ownerName = getUserNameFromUid(owner);
	char *groupName = getGroupNameFromGid(group);
	int result = old_chown(pathname, owner, group);
	GEN_CODE(chown, "%d", result, "'%s', '%s', '%s'", pathname, ownerName, groupName);
	return result;	
}
int dup(int oldfd){
	char *oldFileName = fd2Name(oldfd);
	int result = old_dup(oldfd);
	GEN_CODE(dup, "%d", result, "'%s'", oldFileName);
	return result;
}
int dup2(int oldfd, int newfd){
	char *oldFileName = fd2Name(oldfd);
	char *newFileName = fd2Name(newfd);
	int result = old_dup2(oldfd, newfd);
	GEN_CODE(dup2, "%d", result, "'%s', '%s'", oldFileName, newFileName);
	return result;
}
void _exit(int status){
	old__exit(status);
	GEN_CODE(_exit, "", NULL, "%d", status);
}
int execl(const char *path, const char *arg, ...){	
	char *argv[2560];
	argv[0] = arg;
	int i = 1;
	va_list argl;
	va_start(argl, arg);
	char *a; 
	while((a = va_arg(argl, char*)) != NULL){
		argv[i++] = a;
	}
	argv[i++] = NULL;
	va_end(argl);

	int result = execv(path, argv);
	fprintf((outputFile ? outputFile : stderr), "[monitor] execl(");	
	int j;
	fprintf((outputFile ? outputFile : stderr), "'%s', ", path);
	for(j = 0; j < i; j++)
		fprintf((outputFile ? outputFile : stderr),(j == i-1) ? "NULL" : "'%s', ", argv[j]);
	fprintf((outputFile ? outputFile : stderr), ") = %d\n", result);
	return result;
}
int execle(const char *path, const char *arg, ...){
	va_list ap;
	va_start (ap, arg);
	char *argv[2560];
	char *a;
	argv[0] = arg;
	int i = 1;

	va_start (ap, arg);	
	while((a = va_arg(ap, char*)) != NULL){
		argv[i++] = a;
	}
	argv[i++] = NULL;
	char **envp = va_arg(ap, char **);
	va_end(ap);
	int result = execve(path, argv, envp);	
	fprintf((outputFile ? outputFile : stderr), "[monitor] execle(");
	fprintf((outputFile ? outputFile : stderr), "'%s', ", path);
	int j;
	for(j = 0; j < i; j++)
		fprintf((outputFile ? outputFile : stderr), (j == i-1) ? "NULL, [ " : "'%s', ", argv[j]);
	for( ;(*envp) != NULL; envp++)
		fprintf((outputFile ? outputFile : stderr), (*(envp+1) == NULL) ? "'%s', NULL ]" : "'%s', ", *envp);
	fprintf((outputFile ? outputFile : stderr), ") = %d\n", result);
	return result;
}
int execlp(const char *file, const char *arg, ...){
	va_list ap;
	va_start(ap, arg);
	char *argv[2560];
	char *a;
	argv[0] = arg;
	int i = 1;
	va_start(ap, arg);
	while((a = va_arg(ap, char*)) != NULL){
		argv[i++]  = a;
	}
	argv[i++] = NULL;
	int result = execvp(file, argv);	
	fprintf((outputFile ? outputFile : stderr), "[monitor] execlp(");
	fprintf((outputFile ? outputFile : stderr), "'%s', ", file);
	int j;
	for(j = 0; j < i; j++)
		fprintf((outputFile ? outputFile : stderr), (j == i-1) ? "NULL" : "'%s', ", argv[j]);
	fprintf((outputFile ? outputFile : stderr), ") = %d\n", result);
	return result;
}
int execv(const char *path, char *const argv[]){
	int result = old_execv(path, argv);
	char **a = argv;
	fprintf((outputFile ? outputFile : stderr), "[monitor] execv(");
	fprintf((outputFile ? outputFile : stderr), "'%s', [", path);
	for( ;(*a) != NULL; a++)
		fprintf((outputFile ? outputFile : stderr), (*(a+1) == NULL) ? "'%s', NULL ]" : "'%s', ", *a);
	fprintf((outputFile ? outputFile : stderr), ") = %d\n", result);
	return result;
}
int execve(const char *filename, char *const argv[], char *const envp[]){
	int result = old_execve(filename, argv, envp);
	char **a = argv;
	fprintf((outputFile ? outputFile : stderr), "[monitor] execve(");
	fprintf((outputFile ? outputFile : stderr), "'%s', [", filename);
	for(;(*a) != NULL; a++)
		fprintf((outputFile ? outputFile : stderr), (*(a+1) == NULL) ? "'%s', NULL ], [ " : "'%s', ", *a);
	a = envp;
	for(;(*a) != NULL; a++)
		fprintf((outputFile ? outputFile : stderr), (*(a+1) == NULL) ? "'%s', NULL ]" : "'%s', ", *a);	
	fprintf((outputFile ? outputFile : stderr), ") = %d\n", result);
	return result;
}
int execvp(const char *file, char *const argv[]){
	int result = old_execvp(file, argv);
	char **a = argv;
	fprintf((outputFile ? outputFile : stderr), "[monitor] execvp(");
	fprintf((outputFile ? outputFile : stderr), "'%s', [", file);
	for(;(*a) != NULL; a++)
		fprintf((outputFile ? outputFile : stderr), (*(a+1) == NULL) ? "'%s', NULL ]" : "'%s', ", *a);
	fprintf((outputFile ? outputFile : stderr), ") = %d\n", result);
	return result;
}
int fchdir(int fd){
	char *fileName = fd2Name(fd);
	int result = old_fchdir(fd);
	GEN_CODE(fchdir, "%d", result, "'%s'", fileName);
	return result;
}
int fchown(int fd, uid_t owner, gid_t group){
	char *fileName = fd2Name(fd);
	char *ownerName = getUserNameFromUid(owner);
	char *groupName = getGroupNameFromGid(group);	
	int result = old_fchown(fd, owner, group);
	GEN_CODE(fchown, "%d", result, "'%s', '%s', '%s'", fileName, ownerName, groupName);
	return result;
}
pid_t fork(void){
	GEN_CODE_0(pid_t, fork);
}
int fsync(int fd){
	char *fileName = fd2Name(fd);
	int result = old_fsync(fd);
	GEN_CODE(fsync, "%d", result, "'%s'", fileName);
	return result;
}
int ftruncate(int fd, off_t length){
	char *fileName = fd2Name(fd);
	int result = old_ftruncate(fd, length);
	GEN_CODE(ftruncate, "%d", result, "'%s', %ld", fileName, length);
	return result;	
}
char *getcwd(char *buf, size_t size){
	char *result = old_getcwd(buf, size);
	GEN_CODE(getcwd, "'%s'", result, "'%s', %lu", buf, size);
	return result;
}
gid_t getegid(void){
	GEN_CODE_0(gid_t, getegid);
}
uid_t geteuid(void){
	GEN_CODE_0(uid_t, geteuid);
}
gid_t getgid(void){
	GEN_CODE_0(gid_t, getgid);
}
int link(const char *oldpath, const char *newpath){
	int result = old_link(oldpath, newpath);
	GEN_CODE(link, "%d", result, "'%s', '%s'", oldpath, newpath);
	return result;
}
int pipe(int pipefd[]){
	char *fileName[2];
	fileName[0] = fd2Name(pipefd[0]);
	fileName[1] = fd2Name(pipefd[1]);
	int result = old_pipe(pipefd);
	fprintf((outputFile ? outputFile : stderr), "[monitor] pipe([ ");
	fprintf((outputFile ? outputFile : stderr), "'%s', '%s' ]", fileName[0], fileName[1]);
	fprintf((outputFile ? outputFile : stderr), ") = %d\n", result);
	return result;
}
ssize_t pread(int fd, void *buf, size_t count, off_t offset){
	char *fileName = fd2Name(fd);
	ssize_t result = old_pread(fd, buf, count, offset);
	GEN_CODE(pread, "%d", result, "'%s', %p, %lu, %ld", fileName, buf, count, offset);
	return result;
}
ssize_t pwrite(int fd, const void *buf, size_t count, off_t offset){
	char *fileName = fd2Name(fd);
	ssize_t result = old_pwrite(fd, buf, count, offset);
	GEN_CODE(pwrite, "%d", result, "'%s', %p, %lu, %ld", fileName, buf, count, offset);
	return result;
}
ssize_t read(int fd, void *buf, size_t count){
	char * fileName = fd2Name(fd);
	ssize_t result = old_read(fd, buf, count);
	GEN_CODE(read, "%d", result, "'%s', %p, %lu", fileName, buf, count);
	return result;
}
ssize_t readlink(const char *pathname, char *buf, size_t bufsiz){
	ssize_t result = old_readlink(pathname, buf, bufsiz);
	GEN_CODE(readlink, "%d", result, "'%s', '%s', %lu", pathname, buf, bufsiz);
	return result;
}
int rmdir(const char *pathname){
	int result = old_rmdir(pathname);
	GEN_CODE(rmdir, "%d", result, "'%s'", pathname);
	return result;
}
int setegid(gid_t egid){
	char *groupName = getGroupNameFromGid(egid);
	int result = old_setegid(egid);
	GEN_CODE(setegid, "%d", result, "'%s'", groupName);
	return result;
}
int seteuid(uid_t euid){
	char *userName = getUserNameFromUid(euid);
	int result = old_seteuid(euid);
	GEN_CODE(seteuid, "%d", result, "'%s'", userName);
	return result;
}
int setgid(gid_t gid){
	char *groupName = getGroupNameFromGid(gid);
	int result = old_setgid(gid);
	GEN_CODE(setgid, "%d", result, "'%s'", groupName);
	return result;
}
int setuid(uid_t uid){
	char *userName = getUserNameFromUid(uid);
	int result = old_setuid(uid);
	GEN_CODE(setuid, "%d", result, "'%s'", userName);
	return result;
}
unsigned int sleep(unsigned int seconds){
	unsigned int result = old_sleep(seconds);
	GEN_CODE(sleep, "%u", result, "%u", seconds);
	return result;
}
int symlink(const char *target, const char *linkpath){
	int result = old_symlink(target, linkpath);
	GEN_CODE(symlink, "%d", result, "'%s', '%s'", target, linkpath);
	return result;
}
int unlink(const char *pathname){
	int result = old_unlink(pathname);
	GEN_CODE(unlink, "%d", result, "'%s'", pathname);
	return result;
}
ssize_t write(int fd, const void *buf, size_t count){
	char *fileName = fd2Name(fd);
	int result = old_write(fd, buf, count);
	GEN_CODE(write, "%d", result, "'%s', %p, %lu", fileName, buf, count);
	return result;
}
int chmod(const char *pathname, mode_t mode){
	int result = old_chmod(pathname, mode);	
	GEN_CODE(chmod, "%d", result, "'%s', %u", pathname, mode);
	return result;
}
int fchmod(int fd, mode_t mode){
	char *fileName = fd2Name(fd);
	int result = old_fchmod(fd, mode);
	GEN_CODE(fchmod, "%d", result, "'%s', %u", fileName, mode);
	return result;
}
int __fxstat(int ver, int fd, struct stat *buf){
	char *fileName = fd2Name(fd);
	int result = old___fxstat(ver, fd, buf);
	fprintf((outputFile ? outputFile : stderr), "[monitor] __fxstat");
	fprintf((outputFile ? outputFile : stderr), "('%s', 'type:%s permission:%o size:%llu')", fileName, getFileType(buf->st_mode),
					(buf->st_mode & (S_IRWXU | S_IRWXG | S_IRWXO)), buf->st_size); 
	fprintf((outputFile ? outputFile : stderr), " = %d\n", result);
	return result;
}
int __xstat(int ver, const char *pathname, struct stat *buf){
	int result = old___xstat(ver, pathname, buf);	
	fprintf((outputFile ? outputFile : stderr), "[monitor] __xstat");	
	fprintf((outputFile ? outputFile : stderr), "('%s', 'type:%s permission:%o size:%llu')", pathname, getFileType(buf->st_mode),
					(buf->st_mode & (S_IRWXU | S_IRWXG | S_IRWXO)), buf->st_size);
	fprintf((outputFile ? outputFile : stderr), " = %d\n", result);
	return result;
}
int __lxstat(int ver, const char *pathname, struct stat *buf){
	int result = old___lxstat(ver, pathname, buf);	
	fprintf((outputFile ? outputFile : stderr), "[monitor] __lxstat");	
	fprintf((outputFile ? outputFile : stderr), "('%s', 'type:%s permission:%o size:%llu')", pathname, getFileType(buf->st_mode),
					(buf->st_mode & (S_IRWXU | S_IRWXG | S_IRWXO)), buf->st_size);
	fprintf((outputFile ? outputFile : stderr), " = %d\n", result);
	return result;
}
int mkdir(const char *pathname, mode_t mode){
	int result = old_mkdir(pathname, mode);
	GEN_CODE(mkdir, "%d", result, "'%s', %u", pathname, mode);
	return result;	
}
int mkfifo(const char *pathname, mode_t mode){
	int result = old_mkfifo(pathname, mode);
	GEN_CODE(mkfifo, "%d", result, "'%s', %u", pathname, mode);
	return result;
}
mode_t umask(mode_t mask){
	mode_t result = old_umask(mask);
	GEN_CODE(umask, "%u", result, "%u", mask);
	return result;
}
FILE *popen(const char *command, const char *type){
	FILE * result = old_popen(command, type);
	GEN_CODE(popen, "%p", result, "'%s', '%s'", command, type);
	return result;
}
int pclose(FILE *stream){
	char *fileName = getFileName(stream); 
	int result = old_pclose(stream);
	GEN_CODE(pclose, "%d", result, "'%s'", fileName);
	return result;
}
void *realloc(void *ptr, size_t size){
	void *result = old_realloc(ptr, size);
	GEN_CODE(realloc, "%p", result, "%p %lu", ptr, size);
	return result;
}
int puts(const char *s){
	int result = old_puts(s);
	GEN_CODE(puts, "%d", result, "'%s'", s);
	return result;
}
int feof(FILE *stream){
	char *fileName = getFileName(stream);
	int result = old_feof(stream);
	GEN_CODE(feof, "%d", result, "'%s'",fileName);
	return result;
}












