#define _GNU_SOURCE
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <dirent.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define PORT 8080

int main(){

	int sock = 0, valread;
    struct sockaddr_in serv_addr;
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
   
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
       
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
   
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
	
	std::string word;
//	std::cin >>word;
    getline(std::cin, word);

    std::for_each(word.begin(), word.end(), [](char & c){
                    c = ::tolower(c);
                });

	char buff[1024];
	strcpy(buff, word.c_str());
	send(sock, buff, strlen(buff), 0);


    while(1){

        char buff[1024] = {0};

        int len = 0;

        valread = read(sock, &len, sizeof(int));

        valread = read(sock, buff, len*sizeof(char));

        if (buff[0]!=0){

            std::cout<<buff<<std::endl;

        }
    }

    return 0;


}
