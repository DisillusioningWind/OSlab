#include "boot.h"

#define SECTSIZE 512





void bootMain(void) {
	int i = 0;
	unsigned int elf = 0x150000;
	void (*kMainEntry)(void);

	for (i = 0; i < 200; i++) {
		readSect((void*)(elf + i*512), 1+i);
	}

	// TODO: 填写kMainEntry、phoff、offset...... 然后加载Kernel（可以参考NEMU的某次lab）
    ELFHeader* eh = (ELFHeader*)elf;
    kMainEntry = (void(*)(void))(eh->entry);
	ProgramHeader* ph = (ProgramHeader*)(elf + eh->phoff);
	for (unsigned int i = 0; i < ph->filesz; i++){
		*(unsigned char*)(ph->vaddr + i) =*(unsigned char*)(elf + ph->off + i);		
	}
	kMainEntry();
}

void waitDisk(void) { // waiting for disk
	while((inByte(0x1F7) & 0xC0) != 0x40);
}

void readSect(void *dst, int offset) { // reading a sector of disk
	int i;
	waitDisk();
	outByte(0x1F2, 1);
	outByte(0x1F3, offset);
	outByte(0x1F4, offset >> 8);
	outByte(0x1F5, offset >> 16);
	outByte(0x1F6, (offset >> 24) | 0xE0);
	outByte(0x1F7, 0x20);

	waitDisk();
	for (i = 0; i < SECTSIZE / 4; i ++) {
		((int *)dst)[i] = inLong(0x1F0);
	}
}
