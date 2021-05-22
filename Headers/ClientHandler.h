void clientListener(int new_socket){


    int flagFound = 0;

    char buffer[1024] = {0};

    int valread = read(new_socket, buffer, 1024);

    std::string input(buffer);

    auto found = table.mainMap.find(input);

    if (found == table.mainMap.end()){
        flagFound = 0;
    }
    else
    {
        flagFound = 1;
    }

    std::cout<<input<<std::endl;

    if (flagFound){
    
        std::vector<std::string>::iterator it;

        it = std::unique ((found->second).begin(), (found->second).end());

        (found->second).resize( std::distance((found->second).begin(),it) );

        for (std::vector<std::string>::iterator ik = (found->second).begin(); ik!=(found->second).end(); ++ik){

            char buf[1024] = {0};

            int len = (*ik).length();

            strcpy(buf, (*ik).c_str());

            send(new_socket, &len, sizeof(int), 0);

            send(new_socket, buf, strlen(buf), 0);

        }
    }

    
}