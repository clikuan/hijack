#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	char *env[] = {"PATH=C:\\TEST", "2131", NULL};
	//execlp("lssdas","ls", "-l", "/bin/??", (char *)NULL);
	execle("child.exe", "child", "arg1", NULL,env);
	DIR *d = opendir("./");
//	execl("sr/bin/find","/usr/bin/find",  ".", "-maxdepth", "1", "-perm", "0644",(char *)NULL);
	rewinddir(d);
	seekdir(d,100);
	open("./lcm.so",O_WRONLY|O_CREAT,640);
	FILE *f = tmpfile();
	int fd = open("./test.c", O_APPEND|O_WRONLY);
	int c;
	dup2(fd,c);
	//rand_r(123);
	//rand();
	//srand(12312);
	//exit(-1);
	//return 0;
}
