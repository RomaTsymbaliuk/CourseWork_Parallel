#include <iostream>
#include <chrono>
#include <vector>
#include <mutex>
#include <thread>
#include <fstream>
#include <unordered_map>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <dirent.h>
#include <algorithm>
#define PORT 8080
#define THREADS_NUMBER 3
std::unordered_map<std::string, std::vector<std::string>> table;
std::vector<std::string> ignorlist{".", ",","!","@","#","$","%","&","*","(",")", "_","+","=","?","`","~", "|","/", ":", ";", "<", ">", "{", "}"};
std::vector<std::string> ignorWords{"br", "</br>", "<h>", "</h>"};

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

std::mutex mtx;

void buildIndex(std::vector<std::string> temp,  char* path){


    

    for (std::vector<std::string>::iterator ik=temp.begin(); ik != temp.end(); ++ik){



        std::string filename(path);
        filename.append("/");
        filename.append((*ik));
        std::string f = (*ik);

        std::fstream file;
        file.open(filename);

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
                
//                std::mutex mtx;

                if ( table.find(word) == table.end() ) {

                    std::lock_guard<std::mutex> guard(mtx);

                    std::vector<std::string> key;                    
                    key.push_back(f);
                    std::thread::id this_id = std::this_thread::get_id();
                    
//                    std::cout<<"Word : "<<word<<"  found  "<<" by thread : "<<this_id<<std::endl;
                    
                    table.insert({word, key});
                    

                } 

                else {

                    int exists = 0;

                    std::lock_guard<std::mutex> guard(mtx);
                    

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


   
    }


int main(int argc, char const *argv[])
{
    char* path = "./aclImdb_v1/aclImdb/train/neg";

    std::string startIndex = "6000"; //7750
    std::string endIndex = "8000";
    std::vector<std::string> v;
    std::vector<std::thread> threads;

    v = getFiles(path, startIndex, endIndex);

    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    
    char symbol = '"';
    std::string s(1, symbol); 
    ignorlist.push_back(s);
    
    int i = 0;
std::cout<<"v size "<<v.size()<<std::endl;
std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();

    for (int i = 0; i < THREADS_NUMBER; i++){



        std::vector<std::string> temp;
        int start = (v.size() / THREADS_NUMBER ) * i;
        int end = (v.size()  / THREADS_NUMBER ) * (i + 1);

        for (int k = start; k < end; k++ ){
            temp.push_back(v[k]);
        }

        threads.push_back(std::thread(buildIndex, temp, path));

        temp.clear();

    }


    for (auto& th : threads) th.join();

std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

std::cout<<"Index built in "<<std::chrono::duration_cast<std::chrono::seconds>(end - begin).count()<<" seconds "<<std::endl;

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
    auto found = table.find(input);

    std::cout<<input<<std::endl;

    for (std::vector<std::string>::iterator ig = (found->second).begin(); ig!=(found->second).end(); ++ig){

        std::cout<<(*ig)<<"  ";

    }

    std::cout<<std::endl;
    
    return 0;
}