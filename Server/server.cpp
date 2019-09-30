

// Server side C/C++ program to demonstrate Socket programming
#include "server.h"

server::server(string _listen_hostname, int _listen_port)
{
  port = _listen_port;

  hostname = copyStr(_listen_hostname);
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
      perror("socket failed");
      exit(EXIT_FAILURE);
  }

  // Forcefully attaching socket to the port 8080
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                                                &opt, sizeof(opt)))
  {
      perror("setsockopt");
      exit(EXIT_FAILURE);
  }
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(_listen_port);

  // Forcefully attaching socket to the port 8080
  if (bind(server_fd, (struct sockaddr *)&address,
                               sizeof(address))<0)
  {
      perror("bind failed");
      exit(EXIT_FAILURE);
  }

}

void server::clean()
{
  memset(buffer, 0,sizeof(buffer));
}

void server::acceptSession()
{
    if (listen(server_fd, 5) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                       (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    //cout << "connection Established...\n";
}
string server::getPacket()
{
  ////cout << "waiting...\n";
  valread = read( new_socket , buffer, SIZE);
  ////cout << "done...\n";
  string back;

    for(int i = 0; i < SIZE;i++)
      back.push_back(buffer[i]);
  ////cout << back << endl;
  return back;
}

Message server::getRequest()
{

  acceptSession();
  //cout << "getting request...\n";
  map<int,string> recieved;
  vector<string> recVec;
  string tmp;
  bool end = true;

  do{
    tmp = getPacket();
    recieved[getPacketNumber(tmp,end)] = tmp;
    //cout << "got a packet...\n";
  }while(end);

  //cout << "got a request...\n";

  for(auto it:recieved)
  {
    //cout << it.first << endl;
    recVec.push_back(it.second);
  }
  //system("pause");
  Message ret(recVec);
  //system("pause");
  return ret;

}

void server::serveRequest()
{
  clean();
  sendReply(doOperation(getRequest()));
  close(new_socket);
  //close(server_fd);
  //cout << "request served\n";
  return;
}

Message server::doOperation(Message req)
{
  vector<string> info = req.getMessage();
  vector <string> rep;
  switch (req.getOperation()) {
    case 2:
      rep = download(info[0]);
      break;
    case 3:
      rep =  upload(info[0], info[1]);
      break;
  }
  //cout << "reply operation: " << req.getOperation() << endl;
  //cout << "reply type: " << Reply << endl;
  // for(auto it:rep)
  //   //cout << "rep arg: " << it << endl;
  //cout << "reply RpcID: " << req.getRPCId();
//  system("pause");
  Message ret(req.getOperation(),Reply, rep, req.getRPCId());
  //cout << "operation done...\n";
  return ret;
}

vector<string> server::download(string fName)
{
  //cout <<"file name: " <<  fName << endl;
  vector<string> ret;

  string data = readDataFromFile(fName);
  ////cout << "data: " << data << endl;
  if(data == "")
    ret.push_back("false");
  else
  {
    ret.push_back("true");
    ret.push_back(fName);
    ret.push_back(data);
  }
  for(auto it:ret)
    //cout << "ret arg: " << it << endl;
  return ret;
}

vector<string> server::upload(string fName, string data)
{
  writeDataToFile(fName, data);
  vector<string> ret;
  ret.push_back("true");
  //system("pause");
  return ret;
}

void server::sendReply(Message str)
{
  vector<string >sent = str.marshal();
  //cout << "reply marshalled...\n";
  for(auto it:sent)
    sendPacket(it);
}
void server::sendPacket(string str)
{

  char * msg = copyStr(str);
  //cout << str << endl;
  send(new_socket , msg , str.size() , 0 );
}
server::~server()
{
  close(server_fd);
}
