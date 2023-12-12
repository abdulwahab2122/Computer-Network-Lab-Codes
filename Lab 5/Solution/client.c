#include <stdio.h>
#include <string.h>
#include <sys/socket.h> //socket
#include <arpa/inet.h> //inet_addr

void printCandidate(char data[10][100], int candidateCount);
int readFile(char data[10][100]);

int main(void)
{
        char data[10][100];
        int socket_desc, candidateCount = readFile(data);
        struct sockaddr_in server_addr;
        char server_message[2000], client_message[2000];
        
        //Cleaning the Buffers
        
        memset(server_message,'\0',sizeof(server_message));
        memset(client_message,'\0',sizeof(client_message));
        
        //Creating Socket
        
        socket_desc = socket(AF_INET, SOCK_STREAM, 0);
        
        if(socket_desc < 0)
        {
                printf("[-]Could Not Create Socket. Error!!!!!\n");
                return -1;
        }
        
        
        //Specifying the IP and Port of the server to connect
        
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(2000);
        server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        
        //Now connecting to the server accept() using connect() from client side
        
        if(connect(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
        {
                printf("[-]Connection Failed. Error!!!!!");
                return -1;
        }
        
        printf("* * * * *    ONLINE VOTING SYSTEM     * * * * * \n\n");
        
        //Get Input from the User
        char name[100], cnic[100];
        printf("Please enter your name : ");
        gets(name);
        printf("Enter your CNIC in the given format (XXXXX-YYYYYYY-Z) : ");
        gets(cnic);                                     
        
        //Sending name and CNIC to Server
        sprintf(client_message, "%s/%s", name, cnic);
        if(send(socket_desc, client_message, strlen(client_message),0) < 0)
        {
                printf("Send Failed. Error!!!!\n");
                return -1;
        }
        
        //Receive the authentication message back from the server
        if(recv(socket_desc, server_message, sizeof(server_message),0) < 0)
        {
                printf("Receive Failed. Error!!!!!\n");
                return -1;
        }

        if(strcmp(server_message, "False") == 0){
                printf("Sorry! Your name is not on the registered voter list.\n");
                close(socket_desc);
                return 0;
        }
        
        printCandidate(data, candidateCount);
        printf("\nEnter your desired candidate's poll symbol: ");
        gets(name); 
        sprintf(client_message, "%s", name); 
        if(send(socket_desc, client_message, strlen(client_message),0) < 0)
        {
                printf("Send Failed. Error!!!!\n");
                return -1;
        }
        
        printf("Thank you for casting your vote.\n");
        
        memset(server_message,'\0',sizeof(server_message));
        memset(client_message,'\0',sizeof(client_message));
        
        //Closing the Socket
        
        close(socket_desc);
        
        return 0;
}


void printCandidate(char data[10][100], int candidateCount){
        printf("* * * * *    List of Candidates    * * * * * \n\n");
        for(int i = 0; i< candidateCount; i++)
                printf("%s",data[i]);
        printf("\n\n");

}

int readFile(char data[10][100]){
        FILE *fp = fopen( "Candidates_List.txt", "r");
        if (fp == NULL) {
        printf("Error opening file\n");
        exit(1);
        }
        int i = 0;
        while (fgets data[i],200,fp)){
            i++;
            if (i == 10) {
            printf("Reached maximum lines limit, exiting...\n");
            break;
            }
        }
        fclose(fp);
        return i;
}