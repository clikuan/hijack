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
#define GEN_CODE_0(type, fName, ...){	\
			type oldRes = old_##fName(__VA_ARGS__);	\
			char* result;   \
			fprintf((outputFile ? outputFile : stderr), "[monitor] "#fName"(");	\	
			if(strcmp(#type, "uid_t") == 0) result = getUserNameFromUid(oldRes);	\
			else if(strcmp(#type, "char*") == 0) result = oldRes;	\
			else if(strcmp(#type, "FILE*") == 0) result = getFileName(oldRes);	\
			else if(strcmp(#type, "int") == 0){	\
				fprintf((outputFile ? outputFile : stderr), ") = %d\n", oldRes);    \
				return oldRes;  \
			}	\
			fprintf((outputFile ? outputFile : stderr), ") = '%s'\n", result);    \
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
	n = readlink(fd_path, filename, 255);
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
	argv[i] = NULL;
	va_end(argl);

	int result = execv(path, argv);
	fprintf((outputFile ? outputFile : stderr), "[monitor] execl(");	
	int j;
	for(j = 0; j < i; j++)
		fprintf((outputFile ? outputFile : stderr),(j == i-1) ? "'%s'" :"'%s', ", argv[j]);
	fprintf((outputFile ? outputFile : stderr), ") = %d\n", result);
	return result;
}
int execle(const char *path, const char *arg, ...){
	
}



