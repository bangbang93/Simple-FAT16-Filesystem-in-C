//Fares Almotlag
//V00686361

#include <stdio.h>
#include<stdlib.h>
#include "readImg.h"

// This will use readImg.c to get the rootinfo and print the files info
int main(int argc, char* argv[])
{
	FILE *fp;
	if ((fp=fopen(argv[1],"r"))){
		printf("Successfully open the image file.\n");
		getRoot(fp);		
		printFiles();
	}
	else
		printf("Fail to open the image file.\n");
    
	//freeBuffers();
	fclose(fp);
	return 0;
}
