//Fares Almotlag

#ifndef READIMG_H_
#define READIMG_H_
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
/* This will hold the structures of the files and the img info */

struct{
		char *osname,*labelName;
    int totalSize,freeSize,fatCopies,fatSectors,dirFiles;
}info;

typedef struct{
		char type,*name,*ext,*fullFileName;
		int size,ctime,cdate,fatIndex;
}files;	
files file[223]; 


#endif
