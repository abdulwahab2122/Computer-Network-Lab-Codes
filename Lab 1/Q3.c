#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<stdbool.h>

int main(){

    FILE *fp;
    FILE *fp2;
    char buff[255];

    fp = fopen("Input_File.txt", "r");
	fp2 = fopen("Output3.txt", "w+");

    bool alpha=true;
	while (!feof(fp))
	{
		fscanf(fp,"%s",buff);
		int i=0;
		while (buff[i]!='\0')
		{
			if((buff[i]>=65 && buff[i]<=90) || (buff[i]>=97 && buff[i]<=122))
			{
				alpha=true;
			}
			else 
			{
				alpha=false;
			}
			i++;
		}
		
		if (alpha==false)
		{
			fputs(buff, fp2);
			fputc('\n', fp2);
		}
	}
	
	fclose(fp);
	fclose(fp2);
    
    return 0;
}