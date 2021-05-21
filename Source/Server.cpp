#include <iostream>
#include <chrono>
#include <vector>
#include <map>
#include <mutex>
#include <thread>
#include <fstream>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <dirent.h>
#include <algorithm>
#include "safeMap.h"
#include "build.h"

#define PORT 8080
#define THREADS_NUMBER 10

safeMap table;

std::vector<std::string> ignorlist{".", ",","!","@","#","$","%","&","*","(",")", "_","+","=","?","`","~", "|","/", ":", ";", "<", ">", "{", "}"};
std::vector<std::string> ignorWords{"br", "</br>", "<h>", "</h>"};

int main(int argc, char const *argv[])
{
    char* trainNegPath = "./aclImdb/train/neg";
    char* trainPosPath = "./aclImdb/train/pos";
    char* testNegPath = "./aclImdb/test/neg";
    char* testPosPath = "./aclImdb/test/pos";
    char* trainUnsupPath = "./aclImdb/train/unsup";
    std::string startIndex = "7750"; //7750
    std::string endIndex = "8000";

    std::string unsupStartIndex = "31000";
    std::string unsupEndIndex = "32000";

    std::vector<std::string> v;
    std::vector<std::thread> threads;

    v = getFiles(trainNegPath, startIndex, endIndex, v);

    v = getFiles(trainPosPath, startIndex, endIndex, v);

    v = getFiles(testNegPath, startIndex, endIndex, v);

    v = getFiles(testPosPath, startIndex, endIndex, v);

    v = getFiles(trainUnsupPath, unsupStartIndex, unsupEndIndex, v);
    

    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    
    char symbol = '"';
    std::string s(1, symbol); 
    ignorlist.push_back(s);
    
    int i = 0;
    



 //   for (std::vector<std::string>::iterator it = v.begin(); it!= v.end(); ++it){
 //       std::cout<<(*it)<<std::endl;
 //   }

    std::cout<<"v size "<<v.size()<<std::endl;


    
    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    for (int i = 0; i < THREADS_NUMBER; i++){

        std::vector<std::string> temp;
        int start = (v.size() / THREADS_NUMBER ) * i;
        int end = (v.size()  / THREADS_NUMBER ) * (i + 1);

        for (int k = start; k < end; k++ ){
            temp.push_back(v[k]);
        }

        threads.push_back(std::thread(buildIndex, temp));

        temp.clear();

    }


    for (auto& th : threads) th.join();

    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cout<<"Index built in "<<std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count()<<" milliseconds "<<std::endl;

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
       

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                  &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
       

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

    std::string input(buffer);
    auto found = table.mainMap.find(input);

    std::cout<<input<<std::endl;

    for (std::vector<std::string>::iterator ig = (found->second).begin(); ig!=(found->second).end(); ++ig){

        std::cout<<(*ig)<<"  "<<std::endl;

    }

    std::cout<<std::endl;
    

    return 0;
}