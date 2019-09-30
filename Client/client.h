#ifndef CLIENT_H
#define CLIENT_H
#include <arpa/inet.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include<string>
#include<iostream>
#include "stringMan.h"
#include "Message.h"
#define SIZE 60000
using namespace std;

class client{
  private:
    int port;
    char* hostname;

    struct sockaddr_in address;
    int sock, valread;
    struct sockaddr_in serv_addr;
    char buffer[SIZE];


    void clean();
    int connectToServer();
    bool sendPacket(string str);
    string recievePacket();
    Message getReply();
    void handleReply(Message rep);
  public:

  	client(string _listen_hostname, int _listen_port);

  	void sendRequest(Message request);

  	~client();
};
//#include "Server.cpp"
#endif // SERVER_H
