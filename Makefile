CPPFLAGS = -lpthread
HEADERS = ./Headers
 
Server:	./Source/Server.cpp 
	g++ -o Server.out ./Source/Server.cpp $(CPPFLAGS) -I $(HEADERS)

Client:	./Source/Course.cpp
	g++ -o Client.out ./Source/Course.cpp
