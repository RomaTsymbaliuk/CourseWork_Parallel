#include <iostream>
#include <vector>
#include <thread>
#include <fstream>
#include <map>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <dirent.h>
#include <algorithm>
#define PORT 8080
#define THREADS_NUMBER 4;
std::vector<std::string> getFiles(char* path, std::string startIndex, std::string endIndex){

    
    DIR *dir;
    struct dirent *ent;

    int fileCounter = 0;
    int flagStart = 0;
    struct dirent **namelist;
    int n, i;

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

                filesNeeded.push_back(filename);
                fileCounter++;
            }

            free(namelist[i]);
        
        }
        free(namelist);
    }
    

    return filesNeeded;

}

int main(int argc, char const *argv[])
{
    char* path = "./aclImdb_v1/aclImdb/train/neg";
    std::string startIndex = "7750";
    std::string endIndex = "8000";
    std::vector<std::string> v;

    v = getFiles(path, startIndex, endIndex);

    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    

    std::map<std::string, std::vector<std::string>> table;
    
    std::vector<std::string> ignorlist{".", ",","!","@","#","$","%","&","*","(",")", "_","+","=","?","`","~", "|","/", ":", ";", "<", ">", "{", "}"};
    std::vector<std::string> ignorWords{"br", "</br>", "<h>", "</h>"};

    for (std::vector<std::string>::iterator it = v.begin(); it!=v.end(); ++it){

        std::fstream file;
        std::string filename(path);
        filename.append("/");
        filename.append((*it));
        std::string f = (*it);
        file.open(filename, std::ios::in);
        if (!file) {
            std::cout << "No such file"<<std::endl;
        }
        else {

            std::string word;

        while (1) {

            file >> word;

            

            for (std::vector<std::string>::iterator it = ignorWords.begin(); it!=ignorWords.end(); ++it){


                std::string::size_type i = word.find((*it));

                if (i != std::string::npos)
                    word.erase(i, word.length());
            
            }

            for (std::vector<std::string>::iterator it = ignorlist.begin(); it!=ignorlist.end(); ++it){

                if (word.find((*it)) != std::string::npos) {

                    char a = (*it)[0];

                    word.erase(std::remove(word.begin(), word.end(), a), word.end());

                }
                

            }
            


            if ( table.find(word) == table.end() ) {

                std::vector<std::string> key;
                
                
                key.push_back(f);

                table.insert({word, key});
  
            } else {

                int exists = 0;
                for (std::vector<std::string>::iterator it = (table.find(word)->second).begin(); it!= (table.find(word)->second).end(); ++it){
                    if ((*it) == f){
                        exists = 1;
                        break;
                    }
                }
                if (!exists){
                    (table.find(word)->second).push_back(f);
                }

            }
            


            if (file.eof()){

                file.close();
                break;
            }

            }
        }
    }

    std::map<std::string, std::vector<std::string>>::iterator it;
//    for (it = table.begin(); it != table.end(); it++)
//    {

//    std::cout << it->first<<':';
//        for (std::vector<std::string>::iterator ig = (it->second).begin(); ig!=(it->second).end(); ++ig){

//       std::cout<<(*ig)<<"  ";

//        }
//    printf("\n");

//    }

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
       
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
       
    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, 
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    char buffer[1024] = {0};
    valread = read(new_socket, buffer, 1024);


//    printf("\nGot from client : %s\n", buffer);  
//    printf("\n\n Printing our map \n\n");
    
    std::string input(buffer);
    auto found = table.find(input);

    std::cout<<input<<std::endl;

    for (std::vector<std::string>::iterator ig = (found->second).begin(); ig!=(found->second).end(); ++ig){

        std::cout<<(*ig)<<"  ";

    }

    std::cout<<std::endl;


    
    return 0;
}