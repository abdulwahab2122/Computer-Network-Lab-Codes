/*
        UDP_Server. This Program will will create the Server side for UDP_Socket Programming.
        It will receive the data from the client and then send the same data back to client.
*/

#include <stdio.h> 
#include <stdlib.h>
#include <stdbool.h>
#include <string.h> 
#include <sys/socket.h> //// Needed for socket creating and binding
#include <arpa/inet.h> //inet_addr

bool readDb(char students[100][12], int* studentCount){
        char temp[10];
        FILE* fp = fopen("students.txt","r");
        if (!fgets(temp,10,fp)){
                printf("invalid file format\n");
                return false;
        }
        else 
                *studentCount = atoi(temp);

        
        for (int i=0; i < *studentCount; i++){
                if(!fgets(temp,10,fp))
                        break;
                temp[8] = '\0';
                strcpy(students[i],temp);
                printf("%s\n",students[i]);
        }
        fclose(fp);
        return true;
}

void outputToDb(char students[100][12], int* studentCount){
        FILE* fp = fopen("students.txt","w");
        fprintf(fp, "%d\n", *studentCount);
        for(int i=0; i<*studentCount; i++)
                fprintf(fp, "%s\n", students[i]);
        
        fclose(fp);
}

//return true if rollNo is in db
int checkInDb(char rollNo[], char students[100][12], int* studentCount){
        for(int i=0; i<*studentCount; i++){
                if (strcmp(rollNo, students[i]) == 0)
                        return i;
        }
        return -1;
}

//adds roll no to db
bool addToDb(char rollNo[], char students[100][12], int* studentCount){
        if(checkInDb(rollNo,students, studentCount) == -1){
                //we need to add roll no in db
                strcpy(students[*studentCount], rollNo);
                (*studentCount)++;
                outputToDb(students, studentCount);
                return true;
        }
        else
                return false;
}

//remove roll no from db
bool removeFromDb(char rollNo[], char students[100][12], int* studentCount){
        int temp = checkInDb(rollNo,students, studentCount);
        if(temp != -1){
                for(int i=temp ; i< (*studentCount)-1; i++){
                        strcpy(students[i], students[i+1]);
                }
                (*studentCount)--;
                outputToDb(students, studentCount);
                return true;
        }
        else
                return false;

}

//returns 1 if user wants to checkout
//returns 2 id user wants to checkin
//else returns -1
//also formats the msg to contain only roll number
int analyzeMessage(char msg[]){
        int digits[6] = {0,1,4,5,6,7};
        for(int i=0 ; i<6 ; i++){
                if(msg[digits[i]] < '0' || msg[digits[i]] > '9'){         
                        return -1;
                }
        }
        if(msg[2] != 'L' && msg[2] != 'K' && msg[2] != 'I' && msg[2] != 'P' && msg[2] != 'F')
                return -1;
        if(msg[9] == 'C' && msg[10] == 'O'){
                msg[8] = '\0';
                return 1;
        }
        if(msg[9] == 'C' && msg[10] == 'I'){
                msg[8] = '\0';
                return 2;
        }
        return -1;
}

int main(void){
        int studentCount = 0;
        char students[100][12];
        if (!readDb(students, &studentCount)){
                printf("unable to read file correctly");
                return -1;
        }

        
        int socket_desc;
        struct sockaddr_in server_addr, client_addr;
        char server_message[200], client_message[50]; 
        int client_struct_length = sizeof(client_addr);
        
        //Cleaning the Buffers
        
        memset(server_message,'\0',sizeof(server_message));
        memset(client_message,'\0',sizeof(client_message));
        
        //Creating UDP Socket
        
        socket_desc = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        
        if(socket_desc < 0)
        {
                printf("Could Not Create Socket. Error!!!!!\n");
                return -1;
        }
        
        printf("Socket Created\n");
        
        //Binding IP and Port to socket
        
        server_addr.sin_family = AF_INET;
        server_addr.sin_port = htons(2000);
        server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // bind your socket to localhost only, if you want connect any particular ip you should mention it in INET_ADDR.
        
        if(bind(socket_desc, (struct sockaddr*)&server_addr, sizeof(server_addr))<0)
        {
                printf("Bind Failed. Error!!!!!\n");
                return -1;
        }        
        
        printf("Bind Done\n");
        
        printf("Listening for Messages...\n\n");
        
        //Receive the message from the client
        while (true){
                if (recvfrom(socket_desc, client_message, sizeof(client_message), 0, (struct sockaddr*)&client_addr,&client_struct_length) < 0)
                {
                        printf("[-] Receive Failed. Error!!!!!\n");
                        return -1;
                }
        
                printf("[+] Received Message from IP: %s and Port No: %i\n",inet_ntoa(client_addr.sin_addr),ntohs(client_addr.sin_port));

                printf("[+] Client Message: %s\n",client_message);
                
                //Send the message back to client
                int temp = analyzeMessage(client_message);
                if(temp == 2){
                        if(addToDb(client_message, students, &studentCount))
                                sprintf(server_message, "Welcome Student %s", client_message);
                        else
                                sprintf(server_message, "Student %s. You are already here", client_message);
                }
                else if(temp == 1){
                        if(removeFromDb(client_message, students, &studentCount))
                                sprintf(server_message, "Goodbye student %s. Have a nice day", client_message);
                        else
                                sprintf(server_message, "Student %s. You didn\'t check in today. Contact System Administrator", client_message);
                }
                else
                        strcpy(server_message, "Invalid Roll Number format");
                
                if (sendto(socket_desc, server_message, strlen(server_message), 0, (struct sockaddr*)&client_addr,client_struct_length)<0)
                {
                        printf("Send Failed. Error!!!!!\n");
                        return -1;
                }

                printf("Students checked in right now:\n");
                for (int i=0; i<studentCount; i++){
                        printf("%s\n", students[i]);
                }
                memset(server_message,'\0',sizeof(server_message));
                memset(client_message,'\0',sizeof(client_message));
                
        }
        //Closing the Socket
        close(socket_desc);
        return 0;       
}
