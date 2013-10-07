//Fares Almotlag
#include<stdio.h>
#include<stdlib.h>
#include "readImg.h"
	
//This will use readImg.c to get the image info
int main(int argc, char* argv[])
{
	FILE *fp;
	if ((fp=fopen(argv[1],"r"))){
		printf("Successfully open the image file.\n");
		getOSName(fp); 
		getRoot(fp);		
		getSize(fp);
		getFatCopies(fp);
		getFatSector(fp);
		getFreeSpace(fp);	
		printf("OS Name: %s\n", info.osname);		
		printf("Total Size: %dKB\n", info.totalSize);	
		printf("Total Free Space: %d Bytes\n",info.freeSize);
		printf("Label name: %s\n",info.labelName);				
		printf("No. of directory enteries: %d\n",info.dirFiles);
		printf("No. of FAT: %d\n",info.fatCopies);
		printf("No. of sectors per FAT: %d\n",info.fatSectors);
	}
	else
		printf("Fail to open the image file.\n");

	free(info.labelName);
	free(info.osname);
	fclose(fp);
	return 0;
}
