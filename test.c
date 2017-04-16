#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
	char buf1[256];
	readlink("./123",buf1,255);
	setuid(0);
	setgid(0);
	seteuid(0);
	setegid(0);
	getgid();
	link("./", "../");
	char *env[] = {"PATH=C:\\TEST", "2131", NULL};
	execvp("lsdads",env);
	//execlp("lssdas","ls", "-l", "/bin/??", (char *)NULL);
//	execve("child.exe", "child", "arg1", NULL,env);
	DIR *d = opendir("./");
//	execl("sr/bin/find","/usr/bin/find",  ".", "-maxdepth", "1", "-perm", "0644",(char *)NULL);
	rewinddir(d);
	seekdir(d,100);
	open("./lcm.so",O_WRONLY|O_CREAT,640);
	FILE *f = tmpfile();
	int fd = open("./test.c", O_APPEND|O_WRONLY);
	rewind(f);
	struct stat buf;
	fstat(fd, &buf);
	int c;
	stat("./123", &buf);
	lstat("./123", &buf);
	dup2(fd,c);
	mkdir("./1233",S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	mkfifo("./1wer233",S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	stat("./1233", &buf);
	stat("./1wer233", &buf);	
	umask(0);
	int fdi[2] = {fd,c};
	pipe(fdi);	
	//rand_r(123);
	//rand();
	//srand(12312);
	//exit(-1);
	//return 0;
	system("ls -la");
}
