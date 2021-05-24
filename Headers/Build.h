
#include <sstream>
#include <dirent.h>
#include <fstream>
#include "Tokenizer.h"
#include "FilesSearcher.h"
extern std::vector<std::string> ignorlist;
extern std::vector<std::string> ignorWords;
extern safeMap table;

void buildIndex(std::vector<std::string> temp){


    for (std::vector<std::string>::iterator ik=temp.begin(); ik != temp.end(); ++ik){


        std::string filename = (*ik);
        std::fstream file;
        file.open(filename);

        if (!file) {

            std::cout << "No such file"<<std::endl;

        }

        else {

            std::string word;

            while (1) {

                file >> word;

                std::for_each(word.begin(), word.end(), [](char & c){
                    c = ::tolower(c);
                });

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
                
                

                if ( table.mainMap.find(word) == table.mainMap.end()) {

                    std::vector<std::string> key;
                  
                    key.push_back(filename);
                        
                    table.insert(word, key);
                    
                } 

                else{

                    table.findVector(word, filename);   

                }

                if (file.eof()){

                    file.close();
                    break;


                }

            }
        }

    }


   
}
