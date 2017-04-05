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
	else{
		fprintf(stderr, "variable MONITOR_OUTPUT is not specify\n");
		exit(1);
	}
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
#define GEN_CODE_0(type, fName, fmt, ...){	\
			type oldRes = old_##fName(__VA_ARGS__);	\
			char* result;   \
			if(strcmp(#type, "uid_t") == 0) result = getUserNmaeFromUid(oldRes);	\
			else if(strcmp(#type, "char*") == 0) result = oldRes;	\
			fprintf((outputFile ? outputFile : stderr), "[monitor] "#fName"(");	\	
			fprintf((outputFile ? outputFile : stderr), ") = '%s'\n",result);    \
			return oldRes;  \
}
char* getUserNmaeFromUid(uid_t uid){
	struct passwd *pwd;
	if ((pwd = getpwuid(uid)) != NULL)
		return pwd->pw_name;
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
	GEN_CODE_0(uid_t, getuid,"");
}
char* getenv(const char *name){
	char *result = old_getenv(name);
	GEN_CODE(getenv, "%s", result, "'%s'", name);
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












