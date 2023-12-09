#include<stdio.h>

int main(){
    unsigned int num = 0x3412;
   
    char* p1 = (char*)&num;

    if(*p1==0x10)
        printf("Your system uses Big Endian.");
    else
    printf("Your system uses Little Endian.");

}