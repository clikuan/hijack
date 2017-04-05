#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main( void ) {
			char *argv[3] = {"Command-line", ".", NULL};

				int pid = fork();

					if ( pid == 0 ) {
								execl("usr/bin/find","/usr/bin/find",  ".", "-maxdepth", "1", "-perm", "0644", (char*)NULL);
										}

						/* Put the parent to sleep for 2 seconds--let the child finished executing */
						//wait( 2 );

							printf( "Finished executing the parent process\n"
												        " - the child won't get here--you will only see this once\n" );

								return 0;
}
