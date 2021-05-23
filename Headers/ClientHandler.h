#include "Intersection.h"
#include "RemoveDupWord.h"

void clientListener(int new_socket){


    int flagFound = 0;

    char buffer[1024] = {0};

    int valread = read(new_socket, buffer, 1024);

    std::string input(buffer);

    removeDupWord(input, new_socket);

    
}