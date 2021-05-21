
extern std::vector<std::string> ignorlist;
extern std::vector<std::string> ignorWords;
extern safeMap table;


std::vector<std::string> getFiles(char* path, std::string startIndex, std::string endIndex){

    std::vector<std::string> filesNeeded; 
    
    DIR *dir;
    struct dirent *ent;

    int fileCounter = 0;
    int flagStart = 0;
    struct dirent **namelist;
    int n, i;

    n = scandir(path, &namelist, 0, versionsort);

    
    if (n < 0)
        perror("scandir");
    else
    {
        for(i = 0 ; i < 2000; ++i)
        {
            std::string filename(namelist[i]->d_name);

            filesNeeded.push_back(filename);
        }
        free(namelist);
    }
    

    return filesNeeded;

}



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
                
                

                if ( table.mainMap.find(word) == table.mainMap.end()) {

                    std::vector<std::string> key;
                  
                    key.push_back(f);
                        
                    table.insert(word, key);
                    
                } 

                else{

                    table.findVector(word, f);
                }

                if (file.eof()){

                    file.close();
                    break;


                }

            }
        }

    }


   
}
