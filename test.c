#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){
	DIR *d = opendir("./");
	rewinddir(d);
	seekdir(d,100);
	open("./lcm.so",O_WRONLY|O_CREAT,640);
	return 0;

}
