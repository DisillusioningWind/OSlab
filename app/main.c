#include "types.h"
#include "utils.h"
#include "lib.h"

union DirEntry {
	uint8_t byte[128];
	struct {
		uint32_t inode;
		char name[64];
	};
};

typedef union DirEntry DirEntry;


int ls(char *destFilePath) {
	printf("ls %s\n", destFilePath);
	int i = 0;
	int fd = 0;
	int ret = 0;
	DirEntry *dirEntry = 0;
	uint8_t buffer[512 * 2];
	fd = open(destFilePath, O_READ | O_DIRECTORY);
	if (fd == -1)
		return -1;
	ret = read(fd, buffer, 512 * 2);
	while (ret != 0 && ret != -1) {
		// TODO: ls
		// Hint: 使用 DIrEntry
		dirEntry = (DirEntry *)buffer;
		for ( i = 0; i < 512 * 2 / sizeof(DirEntry); i++)
		{
			if(dirEntry[i].inode == 0)
				continue;
			else
				printf("%s ", dirEntry[i].name);
		}
		ret = read(fd, buffer, 512 * 2);
	}
	printf("\n");
	close(fd);
	return 0;
}

int cat(char *destFilePath) {
	if(destFilePath[0] == ' '){
		int len = stringLen(destFilePath);
		stringCpy(destFilePath + 1, destFilePath, len - 1);
	}
	printf("cat %s\n", destFilePath);
	uint8_t buffer[512 * 2];
	int fd = 0;
	int ret = 0;
	fd = open(destFilePath, O_READ);

	if (fd == -1)
		return -1;
	ret = read(fd, buffer, 512 * 2);

	while (ret != 0 && ret != -1) {

		// TODO: cat
		//把内容读到buffer，输出...
		//buffer[512 * 2] = '\0';
		//code above change fd to 0 so that the file can't be closed
		//???
		printf("%s", buffer);

		ret = read(fd, buffer, 512 * 2);

	}

	close(fd);
	return 0;
}

int uEntry(void) {
	int fd = 0;
	int i = 0;
	char tmp = 0;
	
	ls("/");
	ls("/boot/");
	ls("/dev/");
	ls("/usr/");

	printf("create /usr/test and write alphabets to it\n");
	fd = open("/usr/test", O_WRITE | O_READ | O_CREATE);
	for (i = 0; i < 26; i ++) {
		tmp = (char)(i % 26 + 'A');
		write(fd, (uint8_t*)&tmp, 1);
	}
	close(fd);
	ls("/usr/");
	cat(" /usr/test");
	printf("\n");
	printf("rm /usr/test\n");
	remove(" /usr/test");
	ls("/usr/");
	printf("rmdir /usr/\n");
	remove("/usr");
	ls("/");
	printf("create /usr/\n");
	fd = open("/usr/", O_CREATE | O_DIRECTORY);
	close(fd);
	ls("/");
	
	exit();
	return 0;
}
