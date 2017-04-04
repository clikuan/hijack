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
#define GEN_CODE(type, fName, fmt, ...){	\
			type oldRes = old_##fName(__VA_ARGS__);	\
			char* result;   \
			if(strcmp(#type, "uid_t") == 0) result = getUserNmaeFromUid(oldRes);	\
			else if(strcmp(#type, "char*") == 0) result = oldRes;	\
			fprintf((outputFile ? outputFile : stderr), "[monitor] "#fName"(");	\	
			fprintf((outputFile ? outputFile : stderr), fmt, __VA_ARGS__);	\
			fprintf((outputFile ? outputFile : stderr), ") = '%s'\n",result);    \
			return oldRes;  \
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
uid_t getuid(void){
	GEN_CODE_0(uid_t, getuid,"");
}
char* getenv(const char *name){
	GEN_CODE(char*, getenv, "'%s'", name);
}
int closedir(DIR *dirp){
	GEN_CODE(int, closedir, "%d", dirp);
}
int close(int fd){
	GEN_CODE(int, close, "%d", fd);
}


