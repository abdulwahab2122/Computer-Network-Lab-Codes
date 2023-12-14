/*
        TCP_Server. This Program will will create the Server side for TCP_Socket Programming.
        It will receive the data from the client and then send the same data back to client.
*/

#include <stdio.h> 
#include <string.h> 
#include <sys/socket.h> //socket
#include <arpa/inet.h> //inet_addr
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
 char server_message[2000], client_message[2000]; // buffers for storing server and client messages
int total=0;
sem_t mutex;
void *communication(void *arg)
{
    int s=*(int*)arg;
     //Receive the message from the client
        
        if (recv(s, client_message, sizeof(client_message),0) < 0)
        {
                printf("Receive Failed. Error!!!!!\n");
                return NULL; 
        }
        
        printf("Client Message: %s\n",client_message);
        
        
        //Send the message back to client
        
        strcpy(server_message, client_message);
        
        if (send(s, server_message, strlen(client_message),0)<0)
        {
                printf("Send Failed. Error!!!!!\n");
                return NULL;
        }
        if(strcmp("DISCONNECT",client_message)==0)
        {
            close(s);
            exit(0);
        }
        
        sem_wait(&mutex);
        total--;
        sem_post(&mutex);
       
}

int main(void)
{
    sem_init(&mutex,0,1);
    pthread_t capacity[4];
    int socket_desc, client_sock, client_size; 
    struct sockaddr_in server_addr, client_addr;         //SERVER ADDR will have all the server address
       
        //Cleaning the Buffers
  
        memset(server_message,'\0',sizeof(server_message));
        memset(client_message,'\0',sizeof(client_message));     // Set all bits of the padding field//
        
        //Creating Socket
        
        socket_desc = socket(AF_INET, SOCK_STREAM, 0);
        
        if(socket_desc < 0)
        {
                printf("Could Not Create Socket. Error!!!!!\n");
                return -1;
        }
        
        printf("Socket Created\n");
        
        //Binding IP and Port to socket
        
        server_addr.sin_family = AF_INET;               /* Address family = Internet */
        server_addr.sin_port = htons(2000);               // Set port number, using htons function to use proper byte order */
        server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");    /* Set IP address to localhost */
		
		
		
		// BINDING FUNCTION
        
        if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr))<0)    // Bind the address struct to the socket.  /
	                            	//bind() passes file descriptor, the address structure,and the length of the address structure
        {
                printf("Bind Failed. Error!!!!!\n");
                return -1;
        }        
        
        printf("Bind Done\n");
        
        //Put the socket into Listening State
        while(1)
        {

        if(listen(socket_desc, 1) < 0)                               //This listen() call tells the socket to listen to the incoming connections.
     // The listen() function places all incoming connection into a "backlog queue" until accept() call accepts the connection.
        {
                printf("Listening Failed. Error!!!!!\n");
                return -1;
        }
        
        printf("Listening for Incoming Connections.....\n");
        //int i=0;
        
        //Accept the incoming Connections
        
        client_size = sizeof(client_addr);
		
		
		
        client_sock = accept(socket_desc, (struct sockaddr*)&client_addr, &client_size);          // Creating new socket for a particular client
        
        if (client_sock < 0)
        {
                printf("Accept Failed. Error!!!!!!\n");
                return -1;
        }
        
        printf("Client Connected with IP: %s and Port No: %i\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));
		       //inet_ntoa() function converts the Internet host address in, given in network byte order, to a string in IPv4 dotted-decimal notation
        
        //printf("Total is: %d\n",total);
        if(total<3)
        {

            if(pthread_create(&capacity[total],NULL,communication,(void*)&client_sock)!=0)
                printf("Thread 1 Creation Failed\n");
            
            total++;
        }
        else
        {
                char m[12]="SERVER FULL";
                if (send(client_sock, m ,strlen(m),0)<0)
                {
                    printf("Send Failed. Error!!!!!\n");
                    return -1;
                }
        }

       /// pthread_join(capacity[i-1],NULL);
        memset(server_message,'\0',sizeof(server_message));
        memset(client_message,'\0',sizeof(client_message));
         
        //Closing the Socket
        }
        close(client_sock);
        close(socket_desc);
        sem_destroy(&mutex);
        return 0;       
}