#define _GNU_SOURCE
#include <iostream>
#include <stdio.h>
#include <vector>
#include <stdlib.h>
#include <dirent.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define PORT 8080
std::vector<std::string> getFiles(char* path, std::string startIndex, std::string endIndex){

	
	DIR *dir;
	struct dirent *ent;


	int fileCounter = 0;
	int flagStart = 0;
	struct dirent **namelist;
    int n,i;

    n = scandir(path, &namelist, 0, versionsort);

    std::vector<std::string> filesNeeded; 
    if (n < 0)
        perror("scandir");
    else
    {
        for(i = 0 ; i < n; ++i)
        {
        	std::string filename(namelist[i]->d_name);

        	if (filename.find(startIndex) != std::string::npos) {

        		flagStart = 1;


			}
			 if (filename.find(endIndex) != std::string::npos) {
        		flagStart = 0;

			}
			if (flagStart){
//	            printf("%s\n", namelist[i]->d_name);
	            filesNeeded.push_back(filename);
	            fileCounter++;
			}

            free(namelist[i]);
        
        }
        free(namelist);
    }
    
//	std::cout<<"There are "<<fileCounter<<" files here ."<<std::endl;  
	return filesNeeded;

}

int main(){

	char* path = "./aclImdb_v1/aclImdb/train/neg";
	std::string startIndex = "7750";
	std::string endIndex = "8000";
	std::vector<std::string> v;
	v = getFiles(path, startIndex, endIndex);
	std::cout<<"Looping through a vector "<<"\n";
	std::cout<<"My length : "<<v.size()<<"\n"; 



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
	

    int fileCounter = 0;
	for (std::vector<std::string>::iterator it = v.begin() ; it != v.end(); ++it){

		char* buf = new char[v.size()];
		for (int i =0; i < (*it).length(); i++){
			buf[i] = (*it)[i];
		}
		
		send(sock , buf , strlen(buf), 0 );
		fileCounter++;
		delete [] buf;

	}
	printf("\nFiles sended %d\n", fileCounter);

    return 0;


}
