#include <stdio.h> 
#include <string.h> 
#include <sys/socket.h> //socket
#include <arpa/inet.h> //inet_addr
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>

int readFile(char data[20][100], char* fileName);
bool IsUserAuthentic(char data[20][100], int voterCount, char* userInfo);
bool IsCandidateValid(char data[20][100], int candidateCount, char* candidate);
bool hasUserVoted(char data[20][100], int voteCount, char* userInfo);
void writeVotetoFile(char* userInfo, char* candidate);
void printAuthenticationMessage(char* userInfo, bool status, char* server_message);


void* processVote(void* para){
        char voter[20][100], candidates[20][100], votes[20][100];
        char server_message[500], client_message[500], userInfo[500];
        
        int socket_desc, voterCount = readFile(voter, "VoterName_List.txt");
        int candidateCount = readFile(candidates, "Candidates_List.txt");
        int voteCount  = readFile(votes, "Votes_List.txt");

        int client_sock = *((int*)para);

        //Cleaning the Buffers   
        memset(server_message,'\0',sizeof(server_message));
        memset(client_message,'\0',sizeof(client_message));     
               
        
        //Receive the message from the client
        if (recv(client_sock, client_message, sizeof(client_message),0) < 0)
        {
                perror("Receive Failed. Error!!!!!\n");
                exit(-1);
        }
        
        printf("Client Message: %s\n",client_message);
        strcpy(userInfo, client_message);
        //authenticate
        bool status = IsUserAuthentic(voter, voterCount, client_message);
        printAuthenticationMessage(client_message, status, server_message);

        
        //Send the message back to client        
        if (send(client_sock, server_message, strlen(server_message),0)<0)
        {
                perror("Send Failed. Error!!!!!\n");
                exit(-1);
        }

        if(!status)
                pthread_exit(NULL);

        memset(server_message,'\0',sizeof(server_message));     
        memset(client_message,'\0',sizeof(client_message)); 
        if (recv(client_sock, client_message, sizeof(client_message),0) < 0)
        {
                perror("Receive Failed. Error!!!!!\n");
                exit(-1);
        }

        
        if (IsCandidateValid(candidates, candidateCount, client_message) && !hasUserVoted(votes, voteCount, userInfo)){
                writeVotetoFile(userInfo, client_message);
                printf("A vote successfully added\n");
        }
        else
                printf("Vote Rejected\n");
        memset(server_message,'\0',sizeof(server_message));
        memset(client_message,'\0',sizeof(client_message));
        
        //Closing the Socket
        
        close(client_sock);
}

int main(void)
{
        pthread_t thread[15];
        int socket_desc, client_size; 
        struct sockaddr_in server_addr, client_addr;         //SERVER ADDR will have all the server address
        
        //Creating Socket
        
        socket_desc = socket(AF_INET, SOCK_STREAM, 0);
        
        if(socket_desc < 0)
        {
                printf("[-]Could Not Create Socket. Error!!!!!\n");
                return -1;
        }
        
        printf("[+]Socket Created\n");
        
        //Binding IP and Port to socket
        
        server_addr.sin_family = AF_INET;               /* Address family = Internet */
        server_addr.sin_port = htons(2000);               // Set port number, using htons function to use proper byte order */
        server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");    /* Set IP address to localhost */
		
		
		
		// BINDING FUNCTION
        
        if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr))<0)    // Bind the address struct to the socket.  /
	                            	//bind() passes file descriptor, the address structure,and the length of the address structure
        {
                printf("[-]Bind Failed. Error!!!!!\n");
                return -1;
        }        
        
        printf("[+]Bind Done\n");
        
        //Put the socket into Listening State
        int i=0;
        while(i<15){
                if(listen(socket_desc, 1) < 0)
                {
                        printf("[-]Listening Failed. Error!!!!!\n");
                        return -1;
                }
                
                printf("[+]Listening for Incoming Connections.....\n");

                //Accept the incoming Connections
                client_size = sizeof(client_addr);
                int client_sock = accept(socket_desc, (struct sockaddr*)&client_addr, &client_size);
                printf("[+]Client Connected with IP: %s and Port No: %i\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
 
                if (client_sock < 0)
                {
                        printf("[-]Accept Failed. Error!!!!!!\n");
                        return -1;
                }

                pthread_create(&(thread[i++]), NULL, processVote, (void*)&client_sock);
                
        }
        close(socket_desc);
        return 0;       
}
int readFile(char data[20][100], char filename[]){
        FILE *fp = fopen( filename, "r");
        if (fp == NULL) {
        printf("Error opening file\n");
        exit(1);
        }
        int i = 0;
        while (fgets(data[i++],200,fp));                    
        for(int j=0;j<i-2;j++){
                int len = strlen(data[j]);
                data[j][len-2]='\000';
        }

        fclose(fp);
        return i;
}
bool IsUserAuthentic(char voterName[20][100], int voterCount, char currentUser[]){
    for(int i = 0; i< voterCount; i++){
        if(strcmp( currentUser, voterName[i] ) == 0)
            return true;                                
    }
    return false;
}
bool hasUserVoted(char data[20][100], int voteCount, char* username){
        for(int i = 0; i < voteCount-1 ;i++){
                char* token;
                token = strtok(data[i],"\t");
                if(strcmp(username, token) == 0){               
                        return true;
                }
        }
        return false;
}
bool IsCandidateValid(char data[20][100], int candidateCount, char* candidate){
        
        for(int i = 0; i<candidateCount-1 ;i++){
                char* token;
                token = strtok(data[i],"\t");
                token = strtok(NULL, "\t");
                if(strcmp(candidate, token) == 0){          
                        return true;
                }
        }
        return false;
}
void printAuthenticationMessage(char currentUser[], bool passed, char retMsg[]){
        printf("Name of Voter : ");
        int i = 0;
        for(i = 0; currentUser[i] != '/'; i++)
                printf("%c",currentUser[i]);
        printf(" CNIC : ");
        for(i = i+1; currentUser[i]!='\0';i++)
                printf("%c",currentUser[i]);                
        if(passed){
                printf(") Verified \n");
                strcpy(retMsg, "True");
        }
        else{
                printf(") Authentication Failed ! ! !\n");
                strcpy(retMsg, "False");
        }
}
void writeVotetoFile(char userInfo[], char candidate[]){
        FILE *fp = fopen( "Votes_List.txt", "a+");
        if (fprintf(fp,"\n%s\t%s",userInfo, candidate) < 0)     
                printf("[-]Error Writing vote to file\n");
        fclose(fp);
}