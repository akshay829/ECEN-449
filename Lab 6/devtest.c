#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
	unsigned int result, read_i, read_j;
	int fd; //file descriptor
	int i, j; //loop variables
	char input = 0;
	unsigned int* multiplyValues = (unsigned int*)malloc(2 * sizeof(int));
	char* outputBuffer = (char*)malloc(3 * sizeof(int));
	int* intBuffer; //used to convert from char* to int* buffer
	//open device file for reading and writing
	//user open to open 'dev/multiplier'/
	fd = open("/dev/multiplier", O_RDWR);
	
	//handle error opening file
	if (fd == -1) {
		printf("Failed to open device file!\n");
		return -1;
	}
	
	while (input != 'q') { //continue until user decides to quit
		for (i = 0; i <= 16; i++) {
			for (j = 0; j <= 16; j++) {
				multiplyValues[0] = i;
				multiplyValues[1] = j;
				char* buffer = (char*)multiplyValues; //switching to char* just in case the write function goes up by the type 
				
				write(fd, buffer, 2 * sizeof(int));
				read(fd, outputBuffer, 3 * sizeof(int));
				
				intBuffer = (int*)outputBuffer;
				read_i = intBuffer[0];
				read_j = intBuffer[1];
				result = intBuffer[2];
				
				printf("%u * %u = %u\n", read_i, read_j, result);
				if (result == (i * j)) {
					printf("Result Correct!\n");
				}
				else {
					printf("Result Incorrect!\n");
				}
				//read from terminal
				input = getchar();
			}
		}
	}
	close(fd);
	free(multiplyValues);
	free(outputBuffer);
	return 0;
}