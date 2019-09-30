#include"server.h"
using namespace std;


int main()
{
  server X("127.0.0.1", 3023);
  while(1)
    X.serveRequest();
return 0;
}
