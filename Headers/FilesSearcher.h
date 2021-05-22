
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
