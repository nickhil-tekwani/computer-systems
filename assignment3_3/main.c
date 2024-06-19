#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include "caesar.h" 

int main(int argc, char* argv[]) {
    int mypid = fork();
	
	if (argc != 4) {
		printf("please make sure you give the 3 required inputs in order: '-e' or '-d' flag, the string for encryption, and a key.");
	} else {
		if (0 == mypid) {
			// spawning child process
			if (strncmp (argv[1], "-e", 2147483647) == 0) {
				caesarEncode(argv[2], (int)(argv[3]));
			} else if (strncmp (argv[1], "-d", 2147483647) == 0) {
				caesarDecode(argv[2], (int)(argv[3]));
			}
		}
	}

    return( 0 );
}