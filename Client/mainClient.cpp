#include "client.h"
using namespace std;
void help();
pair<int,vector<string>> getRequest();
void doOperation(pair<int, string> req);


int main()
{
  string ip;
  int portN;

  pair<int, vector<string>> request;
  int rpcID =0;

  vector<string> tmps;

  int quit = 0;

  printf("Kindly insert the IP of the server:\n");
  cin >> ip;
  printf("Kindly insert the Port Number of the server:\n");
  cin >> portN;

  client Z(ip, portN);
  help();
  while(!quit)
  {
    request = getRequest();


    if(request.first == 1)
    {
      help();
    }
    else if(request.first)
    {
      Message msg(request.first, Request, request.second, rpcID);
      tmps = msg.getMessage();
      for(auto it:tmps)
        cout << it << endl;
      Z.sendRequest(msg);
      rpcID++;
    }
    else
      quit = 1;
  }

  cout << "I'm quitting\n";
  return 0;
}

void help()
{
  printf("The available operation in which you can use the FTP Server are:\nhelp\nget <file name>\nupload <file name>\nbye\n\n\n\n");
}

pair <int,vector<string>> getRequest()
{
  pair<int, vector<string>> ret;
  string op = "";
  string arg;

  do{
  if(op == "1")
    cout << "invalid instructions\n";

  printf ("ftp>> ");
  do {
    getline(cin,op);
  }while(op == "");

  cout << op << endl;

  if(op.substr(0,op.find(' ')) == "help")
  {
    ret.first = 1;

  }
  else if(op.substr(0,op.find(' ')) == "get")
  {
    if(op.find(' ') == std::string::npos)
      op = "1";
    else{
      arg = op.substr(op.find(' ')+1);
      ret.first = 2;
      ret.second.push_back(arg);
      if(arg == " ")
        op = "1";
    }
  }
  else if(op.substr(0,op.find(' ')) == "upload")
  {
    if(op.find(' ') == std::string::npos)
      op = "1";
    else{
    arg = op.substr(op.find(' ')+1);
    cout << arg << endl;
    ret.first = 3;
    ret.second.push_back(arg);
    if(arg == " ")
      op = "1";
    else{
      ret.second.push_back(readDataFromFile(arg));

      if(ret.second[1] == "")
          op = "1";
      }
    }
  }
  else if(op.substr(0,op.find(' ')) == "bye")
  {
    ret.first =0;
  }
  else{
    op = "1";
  }

}while(op == "1");

  return ret;
}
