
// Client side C/C++ program to demonstrate Socket programming
#include "client.h"

client::client(string _listen_hostname, int _listen_port)
{
  port = _listen_port;
  hostname = copyStr(_listen_hostname);
  sock = 0;

}


void client::clean()
{
  memset(buffer, 0,sizeof(buffer));
}

int client::connectToServer()
{

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
  // Convert IPv4 and IPv6 addresses from text to binary form
  if(inet_pton(AF_INET, hostname, &serv_addr.sin_addr)<=0)
  {
      printf("\nInvalid address/ Address not supported \n");
      return -1;
  }

  if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
  {
      printf("\nConnection Failed \n");
      return -1;
  }
return 1;
}


bool client::sendPacket(string str)
{
  char * msg = copyStr(str);
  cout << str << endl;
  return send(sock , msg , str.size() , 0 );
  //printf("Hello message sent\n");
}

string client::recievePacket()
{
  valread = read( sock , buffer, SIZE);
  string rec = "";
  for(int i = 0; i < SIZE; i++)
  {
    rec.push_back(buffer[i]);
  }
  return rec;
}


Message client::getReply()
{
  map<int,string> recieved;
  vector<string> recVec;
  string tmp;
  bool end = true;

  do{
    tmp = recievePacket();
    recieved[getPacketNumber(tmp,end)] = tmp;
  }while(end);
  for(auto it:recieved)
  {
    cout << it.first << endl;
    recVec.push_back(it.second);
  }
  Message ret(recVec);
  return ret;
}

void client::handleReply(Message rep)
{
  int op = rep.getOperation();
  vector<string> info = rep.getMessage();
  if(info[0] == "true")
    printf("command success\n");
  else
    printf("command failure\n");

  if(op == 2)
  {
    if(info[0] == "true")
      writeDataToFile(info[1],info[2]);
  }

}

void client::sendRequest(Message request)
{
  vector<string> sent = request.marshal();
  if(connectToServer()!= -1){
    clean();
    cout << "sending packets...\n";
    for(auto it:sent)
      do{cout << "packet sending...\n";}while(!sendPacket(it));
    cout << "getting reps...\n";
    Message rep = getReply();
    handleReply(rep);
    close(sock);
  }
}

client::~client()
{
  close(sock);
}
