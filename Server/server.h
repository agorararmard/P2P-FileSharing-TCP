#ifndef SERVER_H
#define SERVER_H
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

class server
{
private:
  int port;
  char* hostname;
  int server_fd, new_socket, valread;
  struct sockaddr_in address;
  int opt = 1;
  int addrlen = sizeof(address);
  char buffer[SIZE];

  string getPacket();
  Message getRequest();
  void clean();
  Message doOperation(Message req);

  vector<string> download(string);
  vector<string> upload(string,string);

  void sendReply(Message rep);
  void sendPacket(string rep);

  void acceptSession();
	public:
	server(string _listen_hostname, int _listen_port);

	void serveRequest();

	~server();
};
//#include "Server.cpp"
#endif // SERVER_H
