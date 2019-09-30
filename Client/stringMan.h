#ifndef STRINGMAN_H
#define STRINGMAN_H
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstring>
#include "stdio.h"
#include <thread>
#include <map>
#include <utility>
#include <algorithm>
#include <sys/stat.h>
using namespace std;



char* copyStr(string);
int getPacketNumber(string message, bool& end);
string ShrinkInt(int x);
int UnShrinkInt(string tmp);
bool existFile(string fileName);
string readDataFromFile(string fileName);
void writeDataToFile(string fileName, string data);
//#include "Server.cpp"
#endif // SERVER_H
