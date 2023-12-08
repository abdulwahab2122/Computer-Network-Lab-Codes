#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>

int main(){
    FILE *fp;
	FILE *fpp;
	int c;
	
	fp = fopen("Input_File.txt", "r");
	fpp = fopen("Output.txt", "w+");
	
	while (!feof(fp))
	{
		int c = fgetc(fp);
		if (c>=48 && c<=57)
		{	
			fputc(c, fpp);
		}
	}
	
	fclose (fp);
	fclose (fpp);
	
	return 0;
}