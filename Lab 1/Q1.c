#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>

int main(){
    
    FILE *fp;
    char buff[255];
    fp = fopen("Input_File.txt", "r");
    while(fgets(buff, 255, (FILE*)fp)){
        printf("%s", buff);
    }
    fclose(fp);
    return 0;

}