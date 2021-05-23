void removeDupWord(std::string str, int new_socket)
{
    std::istringstream ss(str);
  
    std::string word; 
    std::vector<std::vector<std::string>> results;

    while (ss >> word) 
    {

        int flagFound = 0;
        std::cout << word << "\n";

        auto found = table.mainMap.find(word);

        if (found == table.mainMap.end()){

            flagFound = 0;

        }
        else{

            flagFound = 1;

        }

        if (flagFound){

            std::vector<std::string>::iterator it;

            it = std::unique ((found->second).begin(), (found->second).end());

            (found->second).resize( std::distance((found->second).begin(),it) );

            results.push_back(found->second);
        }

    }
    
    std::cout<<"_____________________________________________________________________________"<<std::endl;

    

    if (results.size() > 1){

        std::vector<std::string>::iterator it, end; 

        std::vector<std::string> resultVector;

        std::vector<std::string> startVector = results[0];

        for (std::vector<std::vector<std::string>>::iterator ik = results.begin(); ik!=results.end(); ik++){

            resultVector = intersection(startVector, (*ik));

            startVector.clear();

            startVector = resultVector;

        }

        for (std::vector<std::string>::iterator ik = resultVector.begin(); ik!=resultVector.end(); ++ik){

            char buf[1024] = {0};

            int len = (*ik).length();

            strcpy(buf, (*ik).c_str());

            send(new_socket, &len, sizeof(int), 0);

            send(new_socket, buf, strlen(buf), 0);

        }

    }
    else{

        std::vector<std::string> reply = results[0];
        for (std::vector<std::string>::iterator ik = reply.begin(); ik!=reply.end(); ++ik){

            char buf[1024] = {0};

            int len = (*ik).length();

            strcpy(buf, (*ik).c_str());

            send(new_socket, &len, sizeof(int), 0);

            send(new_socket, buf, strlen(buf), 0);

        }

    }
    

}
