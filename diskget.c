//Fares Almotlag
//V00686361
#include <stdio.h>
#include<stdlib.h>
#include "readImg.h"

//This function will get the info of the file from the root dir
files *getFile(FILE *fp, char *fileName){
    int base = 9728;
    int cur = base;
    int offset = 32;
    int entries = (512 *13)/offset;
    char *tmp_name,*tmp_ext,*tmp_fullName;
    int i,tmp_size,tmp_attr,nameLength;
    files *buffer = malloc(entries * sizeof(files));
    fseek(fp,cur,SEEK_SET); // go to the first entry
    for(i = 0;i < entries;i++){
        tmp_name = malloc(sizeof(char)*8);  //This part of the program will iterate through the root dir
        tmp_ext = malloc(sizeof(char)*3);        // And find the correct file
        fread(tmp_name,1,8,fp);
        fread(tmp_ext,1,3,fp);
        fread(&tmp_attr,1,1,fp);
        fseek(fp,cur+28,SEEK_SET);
        fread(&tmp_size,1,4,fp);
        if(strcmp(tmp_name,"") != 0 && tmp_attr != 0x0F && (tmp_attr & 0x08) != 0x08 && (tmp_attr & 0x10) != 0x10){
            tmp_fullName = malloc(sizeof(fileName) + sizeof(char)*3 + 2);
            for (nameLength = 0; nameLength < 8; nameLength++) {
                if (tmp_name[nameLength] != 0x20){
                    tmp_fullName[nameLength] = tmp_name[nameLength];
				} else{
					break;
                }
			}
            strncat(tmp_fullName, ".", 1);   // THis will separate the name from the ext
			strncat(tmp_fullName, tmp_ext, 3);
        }
        if(strcmp(tmp_fullName,fileName) == 0){

            buffer->name = tmp_fullName;
            buffer->size = tmp_size;
            fseek(fp,cur+26,SEEK_SET);
            fread(&buffer->fatIndex,1,2,fp);
            break;
        }
        cur = cur + offset;
        fseek(fp, cur, SEEK_SET);
    }
    return buffer;         
}
//This function will get the fat phsyical address
int getFatEntry(FILE *fp, int entry){ 
    long start = 512 + 1 + (3 * entry) /2;
    long end = 512 + (3 * entry) /2;
    uint8_t endbits;
    uint16_t startbits,entrybits;   //Use the provided formula 
    fseek(fp,start,SEEK_SET);       //Change from little endian and return value
    fread(&startbits,1,1,fp);
    fseek(fp,end,SEEK_SET);
    fread(&endbits,1,1,fp);
    if(entry%2 == 0){
        startbits<<=12;
        startbits>>=4;
    } else{
        startbits<<=8;
        startbits>>=4;
        endbits>>=4;
        }
    entrybits = startbits + endbits;
    return entrybits;
}
//This function will copy file from the image to the current linux dir
void copyFile(FILE *input, FILE *output,files *entry){
    int fatEntry = entry->fatIndex;
    int base = 512;
    int i,dataIndex,dataStart = 0;
    char readByte;
    
    while(fatEntry<4088){ //Iterate though every sector and copy the data
        dataIndex = 33 + fatEntry - 2;
        dataStart = dataIndex * 512;
        for(i = 0;i < 512;i++){
            fseek(input,dataStart+i,SEEK_SET);
            fread(&readByte,1,1,input);
            fwrite(&readByte,1,1,output);
        }
        
        fatEntry = getFatEntry(input,fatEntry);
    }
}

int main(int argc, char* argv[])
{
	FILE *fp;
    char *fileName = argv[2]; //get the file name
    if (argc < 3) { //check if its there
		printf("Enter a disk image and a file name");
		return 1;
	} else if((fp=fopen(argv[1],"r"))){
		printf("Successfully open the image file.\n");
		int i,c;
		for (i = 0; i < strlen(fileName); i++) { //This will convert the name to uppercase
			c = fileName[i];
			fileName[i] = toupper(c);
		}
        
        files *fileEntry = getFile(fp,fileName); //get the file info from the root dir
		if(fileEntry->name == NULL){
            printf("File not found\n");
        } else{
            FILE *output;
            if((output = fopen(fileName, "ab")) == NULL){ //open the file on the current dir
                printf("Error openning the file\n");
                return 1;
            }
            copyFile(fp,output,fileEntry); //copy to linux
            fclose(output);
        }
	}
	else
		printf("Fail to open the image file.\n");
    
	//freeBuffers();
	fclose(fp);
	return 0;
}
