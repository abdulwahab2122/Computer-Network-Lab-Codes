/*
        TCP_Client. This Program will implement the Client Side for TCP_Socket Programming.
        It will get some data from user and will send to the server and as a reply from the
        server, it will get its data back.
*/

#include <stdio.h>
#include <string.h>
#include <sys/socket.h> //socket
#include <arpa/inet.h> //inet_addr
#include <stdbool.h>


void reverseWord (char word[]){
    int wordLength = strlen(word);
    for(int i = 0, j=wordLength-1; i<wordLength/2 ;i++,j--){
        char temp = word[i];
        word[i] = word[j];
        word[j] = temp;
    }
}

bool hasVowel(char word[]){

    int wordLength = strlen(word);
    for(int i = 0; i<wordLength ;i++){
        if(word[i] == 'a' || word[i] == 'e' || word[i] == 'i' || word[i] == 'o' || word[i] == 'u' || word[i] == 'A' || word[i] == 'E' || word[i] == 'I' || word[i] == 'O' || word[i] == 'U'){
            return true;
        }
    }
    return false;
}
void outputReverse(char str[], char retVal[]){
        memset(retVal,'\0',sizeof(retVal));

        char* word = strtok(str," ");
        while(word != NULL){
        if(!hasVowel(word)){
                reverseWord(word);
        }
        strcat(retVal, word);
        strcat(retVal, " ");
        word = strtok(NULL," ");
        }
}

int main(void)
{
        int socket_desc;
        struct sockaddr_in server_addr;
        char server_message[2000], client_message[2000];
        
        //Cleaning the Buffers
        
        memset(server_message,'\0',sizeof(server_message));
        memset(client_message,'\0',sizeof(client_message));
        
        //Creating Socket
        
        socket_desc = socket(AF_INET, SOCK_STREAM, 0);
        
        if(socket_desc < 0)
        {
                printf("Could Not Create Socket. Error!!!!!\n");
                return -1;
        }
        
        printf("Socket Created\n");
        
        //Specifying the IP and Port of the server to connect
        
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(2000);
        server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        
        //Now connecting to the server accept() using connect() from client side
        
        if(connect(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
        {
                printf("Connection Failed. Error!!!!!");
                return -1;
        }
        
        printf("Connected\n");
        
        //Get Input from the User
        
        printf("Enter Message: ");
        gets(client_message);                                     //One is that gets() will only get character string data. 
                                                       //		will get only one variable at a time.
																//  reads characters from stdin and loads them into str
        //Send the message to Server
        
        if(send(socket_desc, client_message, strlen(client_message),0) < 0)
        {
                printf("Send Failed. Error!!!!\n");
                return -1;
        }
        
        //Receive the message back from the server
        
        if(recv(socket_desc, server_message, sizeof(server_message),0) < 0)
        {
                printf("Receive Failed. Error!!!!!\n");
                return -1;
        }
        
        printf("Server Message: %s\n",server_message);
        
        char temp[2000];
        outputReverse(server_message,temp);
        printf("After processing: %s\n",temp);

        memset(server_message,'\0',sizeof(server_message));
        memset(client_message,'\0',sizeof(client_message));
        
        //Closing the Socket
        
        close(socket_desc);
        
        return 0;
}