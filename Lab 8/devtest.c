#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
	printf("IR Remote Device Driver by Christopher Padilla\n");
	unsigned int message;
	char* msg_Ptr = (char*)&message; //used to write 1 byte at a time to message
	int fd; //file descriptor
	char input = 0;
	char* outputBuffer = (char*)malloc(200 * sizeof(char)); //enough to read all 100 messages

															//open device file for reading and writing
															//user open to open 'dev/ir_demod'/
	fd = open("/dev/irq_test", O_RDWR);

	//handle error opening file
	if (fd == -1) {
		printf("Failed to open device file!\n");
		return -1;
	}
	int i = 0;
	for (;;) {
		printf("Outputting messages since previous iteration...\n\n");
		int bytesRead = read(fd, outputBuffer, 200); //read up to 100 messages at a time

		for (i = 0; i < bytesRead / 2; i++) { //print all 100 messages
			msg_Ptr[0] = outputBuffer[i * 2];
			msg_Ptr[1] = outputBuffer[i * 2 + 1];
			msg_Ptr[2] = 0;
			msg_Ptr[3] = 0;
			printf("IR Message Recieved: 0x%x\n", message);
		}
		printf("\n");
		sleep(1); //sleep for () seconds
	}
	close(fd);
	free(outputBuffer);
	return 0;
}