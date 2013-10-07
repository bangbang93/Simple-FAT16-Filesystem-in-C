//Fares Almotlag

#include <stdio.h>
#include <string.h>
#include "readImg.h"

void getRoot(FILE *fp){
	char *label = malloc(sizeof(char)*11);
	char *label2 = malloc(sizeof(char)*8);
	char *name = malloc(sizeof(char)*8);	
	int base = 9728;
        int cur = base;   // point to the first byte of the current entry
	int offset = 32;  // Each entry has 32 bytes in root directory
	int counter,i = 0;
   	int nameLength;
	unsigned char tmp;
	char tmp2;
   	fseek(fp,0x2B,SEEK_SET);
	fread(label,1,11,fp);
	fseek(fp, base, SEEK_SET);
	fread(&tmp,1,1,fp);
	// Read "notes on directory entries" for the correct conditions of how we identify a file
	while(tmp != 0x00)  
	{
        //search for files
		if (tmp != 0xE5)
		{
            // Locate the byte for the current entry's attribute
		file[i].type = 'D';
		file[i].name = malloc(sizeof(char)*8);
		file[i].ext = malloc(sizeof(char)*3);
		file[i].fullFileName = (char *) malloc(sizeof(file[i].name) + sizeof(file[i].ext) + 2);
    		fseek(fp,-1,SEEK_CUR);			
		fread(file[i].name,1,8,fp);
		fread(file[i].ext,1,3,fp);
            	for (nameLength = 0; nameLength < 8; nameLength++) {
				if (file[i].name[nameLength] != 0x20)
					file[i].fullFileName[nameLength] = file[i].name[nameLength];
				else
					break;
		}
            
		strncat(file[i].fullFileName, ".", 1);
		strncat(file[i].fullFileName, file[i].ext, 3);
		fseek(fp,cur+14,SEEK_SET);
            	fread(&file[i].ctime,1,2,fp);
        	fread(&file[i].cdate,1,2,fp);
		fseek(fp,cur+28,SEEK_SET);
		fread(&file[i].size,1,4,fp);
		fseek(fp, cur+11, SEEK_SET);
		fread(&tmp2,1,1,fp);
            
			// What is the attribute of a file ?
			// not 0x0F(not part of a long file name), not suddirectory, not volume label
		if( tmp2!=0x0F && (tmp2 & 0x10) != 0x10 && (tmp2 & 0x08) != 0x08)
		{
			file[i].type = 'F';				
			counter ++;
		}
        		
        	else if(tmp2 == 0x10){
		        file[i].type = 'D';
        	}
            
		else if(tmp2 == 0x08){
			fseek(fp,cur,SEEK_SET);				
			fread(label2,1,8,fp);
                	cur = cur + offset;
        		fseek(fp, cur, SEEK_SET);
        		fread(&tmp,1,1,fp);
                	continue;
		}
            	else{
        		cur = cur + offset;
                	fseek(fp, cur, SEEK_SET);
                	fread(&tmp,1,1,fp);
                	continue;
            	}
            
		}
		
		// Go to next entry in Root Directory
		cur = cur + offset;
		fseek(fp, cur, SEEK_SET);
		fread(&tmp,1,1,fp);
		i++;
	}
	
	file[i].name = NULL;
	if(strcmp(label,label2) != 0){
        label = label2;
	}
	
	info.labelName = label;
	info.dirFiles = counter;
}
void getOSName(FILE *fp)
{
    	char *osname = malloc(sizeof(char)*8);	
	fseek(fp,3L,SEEK_SET);
    	fread(osname,1,8,fp);
	info.osname = osname;
	//free(osname);
}

int getSize(FILE *fp)
{
	int *tmp1 = malloc(sizeof(int));
	int *tmp2 = malloc(sizeof(int));
	int retVal;
	fseek(fp,19L,SEEK_SET);
	fread(tmp1,1,1,fp);
	fread(tmp2,1,1,fp);
	retVal = *tmp1+((*tmp2)<<8);
	free(tmp1);
	free(tmp2);
	retVal = (retVal * 512) / 1024;
	info.totalSize = retVal;	
    
}
char *getDate(int ctime,int cdate){
    	char *result = (char*)malloc(sizeof(char));
    	// For the day
    	int day = 0;
    	day = cdate & 0x1F;
    	// For the month
    	int month = 0;
   	month = cdate & 0x1E0;
    	month >>= 5;
    	// For the year
    	int year = 0;
    	year = cdate & 0xFE00;
    	year >>= 9;
    	year += 1980;
    	// For time
    	// For seconds
    	int sec = 0;
    	sec = ctime & 0x3F;
    	// For minutes
    	int min = 0;
    	min = ctime & 0x7E0;
    	min >>= 5;
    	// For hours
    	int hour = 0;
    	hour = ctime & 0xFC00;
    	hour >>= 11;
    
    	// Conc and return
    	char *buf = (char *) malloc(sizeof(char));
    	sprintf(buf, "%.2d ", day);
	strncat(result, buf, 3);
	sprintf(buf, "%.2d ", month);
    	strncat(result, buf, 3);
    	sprintf(buf, " %4d ", year);
	strncat(result, buf, 6);
	sprintf(buf, "%.2d:%.2d:%.2d", hour, min, sec * 2);
	strncat(result, buf, 8);
    	return result;
}
void printFiles(){
	int i = 0;
	int len = 0;
	while(file[i].name != NULL){
        	if(file[i].type == 'F'){
            		printf("%c %10d bytes %20s %s\n",file[i].type,file[i].size,file[i].fullFileName,getDate(file[i].ctime,file[i].cdate));
		}
		else{
			printf("%c %s\n",file[i].type,file[i].name);
		}
		i++;
	}
}
// loop through the FAT table
// Each entry has 12 bits in FAT table
int getFreeSpace(FILE* fp)
{
	int n = 2;  // logical number of the first sector in Data Area
	int base = 512; // the first byte of the FAT table 
	unsigned int tmp1;
	unsigned int tmp2;
    	unsigned int result;
	int counter;
	// The logical number for all the sectors in Data Area is from 2 to 2848
	 for (n = 2; n <= 2848; n++) 
	{
		// given logical no. of sector in data area
		// where is the corresponding entry in FAT table
		// For the algorithm, refer to the last page of FAT Description: 
		// http://webhome.csc.uvic.ca/~wkui/Courses/CSC360/FAT12Description.pdf
		// if the logical number is even
        
		if (n % 2 == 0)
		{
			fseek(fp, base + 3*n/2, SEEK_SET);
			fread(&tmp1, 1, 1, fp);  // get all 8 bits 
			fread(&tmp2,1 ,1, fp);
			tmp2 = tmp2 & 0x0F;   // get the low 4 bits 
            
			// Then apply "Little Endian": (4 bits)**** + (8 bits)********
			result = (tmp2 << 8) + tmp1;  
		}
        
		// if the logical number is odd
		else
		{			
			fseek(fp, base + 3*n/2, SEEK_SET);
			fread(&tmp1, 1, 1, fp);  
			tmp1 =   tmp1 & 0xF0;   // get the high 4 bits 
			fread(&tmp2,1 ,1, fp);  // get all 8 bits
			// Then apply "Little Endian": (8 bits)**** + (4 bits)********
			result = (tmp2 << 1) + tmp1; 
		}
		
		if (result == 0x00)  // if the value is 0x00, that sector is free
		{
			counter ++;
		}
        
	}
	info.freeSize = (counter * 512);
    
}
//Get how many fat copies are there
void getFatCopies(FILE *fp){
	int fatCopies = 0;
	fseek(fp,16L,SEEK_SET);
	fread(&fatCopies,1,1,fp);
	info.fatCopies = fatCopies;	
}

//Get how many fat sectors
void getFatSector(FILE *fp){
	char *fatSector = malloc(sizeof(char));
	char *fatSector2 = malloc(sizeof(char));
	int retVal = 0;	
	fseek(fp,22L,SEEK_SET);
	fread(fatSector,1,1,fp);
	fread(fatSector2,1,1,fp);
	retVal = *fatSector + ((*fatSector2)<<8);
	free(fatSector);	
	free(fatSector2);	
	info.fatSectors = retVal;	
    
}
void freeBuffers(){
    int i = 0;	
	while(file[i].name != NULL){
        free(file[i].name);
        free(file[i].ext);

    }
}
