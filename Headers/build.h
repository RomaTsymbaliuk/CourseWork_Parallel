
#include <sstream>

extern std::vector<std::string> ignorlist;
extern std::vector<std::string> ignorWords;
extern safeMap table;


void tokenize(std::string const &str, const char delim, std::vector<std::string> &out)
{
 
    std::stringstream ss(str);
 
    std::string s;
    while (std::getline(ss, s, delim)) {
        out.push_back(s);
    }
}

std::vector<std::string> getFiles(char* path, std::string startIndex, std::string endIndex, std::vector<std::string> filesNeeded){

    DIR *dir;
    struct dirent *ent;
    int fileCounter = 0;
    int flagStart = 0;
    struct dirent **namelist;
    int n, i;
    n = scandir(path, &namelist, 0, versionsort);
    const char delim = '_';
    
    if (n < 0)
        perror("scandir");

    else
    {
        for(i = 0 ; i < n; ++i)
        {

            std::string stringPath(path);
            stringPath.append("/");
            stringPath.append(namelist[i]->d_name);

            std::string filename(namelist[i]->d_name);

            
            if (filename.find(startIndex) != std::string::npos) {

                std::vector<std::string> out;
                tokenize(filename, delim, out);

                std::stringstream ss;
                ss << out[0];

                int num;
                ss >> num;

                int start;

                std::stringstream fs;

                fs << startIndex;

                fs >> start;

                if (num == start){

                    flagStart = 1;

                } 

            }

            if (filename.find(endIndex) != std::string::npos) {


                std::cout<<path<<std::endl;

                std::vector<std::string> out;
                tokenize(filename, delim, out);
 
                std::stringstream as;
                as << out[0];     

                int num;
                as >> num;


                int end;

                std::stringstream bs;

                bs << endIndex;

                bs >> end;

                if (num == end){

                    flagStart = 0;

                } 

            }

            if (flagStart){

                filesNeeded.push_back(stringPath);
                fileCounter++;
            }
            
            free(namelist[i]);
        
        }
        free(namelist);
    }
    
    return filesNeeded;

}



void buildIndex(std::vector<std::string> temp){


    for (std::vector<std::string>::iterator ik=temp.begin(); ik != temp.end(); ++ik){



//        std::string filename(path);
//        filename.append("/");
//        filename.append((*ik));
//        std::string f = (*ik);
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
