#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main()
{
FILE *fp;
char buff[255];
int check=0;
fp=fopen("Input_File.txt","r");

if(fp==NULL)
{
printf("Cannot open file. \n");
}
else
{
FILE* fp2;
fp2=fopen("Output4.txt","w+");
do
{
check=0;
for(int i=0;i<strlen(buff);i++)
{
if(buff[i]=='a'|| buff[i]=='A' ||buff[i]=='u'|| buff[i]=='o' || buff[i]=='i'|| buff[i]=='e' || buff[i]=='E'|| buff[i]=='I' || buff[i]=='O'|| buff[i]=='U' )
{
check++;
break;
}
}
if(check==0)
{
fputs(buff, fp2;
fputs("\n",fp2);
}
else
{
for(int i=0;i<strlen(buff)/2;i++)
{
char f=buff[i];
buff[i]=buff[strlen(buff)-i-1];
buff[strlen(buff)-i-1]=fp;
}
fputs(buff, fp2);
fputs("\n",fp2);

}
}

while(fscanf(fp, "%s", buff)!=EOF);
fclose(fp2);
}
fclose(fp);
return 0;
}
