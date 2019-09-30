#include "Message.h"



Message::Message(int p_operation,MessageType type, vector<string> p_message, int p_rpc_id){
	message_type = type;
	rpc_id = p_rpc_id;
	operation = p_operation;
	message_size = 0;
	message = p_message;
	for(int i = 0; i < message.size(); i++)
		message_size += message[i].size()+1;
	arg_num = message.size();
	message_size -= 1;
}


Message::Message(vector<string> serialized){
	arg_num = -1;		//if arg_num = -1; then this is a phantom message;
	message_size = 0;
	operation = -1;
	rpc_id = -1;
	string messageSent = "";
	int nowSize;
	for(int i = 0; i < serialized.size();i++)
	{
		//  ////cout <<"to be unmarshalled: " <<serialized[i] << endl;
		int tmpx;
		tmpx = UnShrinkInt(serialized[i].substr(0,4));
		if(i== 0)
			rpc_id = tmpx;
		else if(rpc_id != tmpx)
		{
			////cout << "rpc_id not matching\n";
			arg_num = -1;
			break;
		}

		tmpx = UnShrinkInt(serialized[i].substr(4,4));
		if(i == 0)
			{
				operation = tmpx & 0x7fffffff;
				// ////cout << "operation: " << operation << endl;
				message_type = MessageType((tmpx >> 31) & 1);
				// ////cout << "message_type: " << message_type << endl;

			}
		else if((operation != (tmpx & 0x7fffffff))||(message_type != MessageType((tmpx >> 31)&1)))
		{
			////cout << "op and mt are not matching\n";
			arg_num = -1;
			break;
		}
		tmpx = UnShrinkInt(serialized[i].substr(8,4));
		if(i == 0)
		{
			nowSize =(tmpx &0x00ffffff);
			message_size += (tmpx &0x00ffffff);
			arg_num = (tmpx >> 24) &0xff;
		}
		else if(arg_num != ((tmpx >> 24) &0xff))
		{
			////cout << "num arg not matching\n";
			arg_num = -1;
			break;
		}
		else
		{
			nowSize =(tmpx &0x00ffffff);
			message_size += (tmpx &0x00ffffff);
		}
		 //////cout << "NowSize: " << nowSize << endl;
		messageSent += serialized[i].substr(16,nowSize);
		////cout << messageSent<<endl;

	}

		//  ////cout << "operationID: " << operation << endl;
		//  ////cout << "Message Size: " <<  message_size << endl;
		//  ////cout << "RpcID: " <<  rpc_id << endl;
		//  ////cout << "argNum: " << arg_num << endl;
		//  ////cout << "Message Type: " << message_type << endl;
		//  ////cout << "messageSent: " << messageSent << endl;
		// //////cout << "ar"

		//////cout << "messageSent.size: " << messageSent.size() << endl;

		////cout << messageSent << endl;
		for(int i =0; i < arg_num-1; i++)
		{
			size_t found = messageSent.find(' ');
			////cout << found << endl;
			string t = messageSent.substr(0,found);
			////cout << messageSent.substr(0,found) << endl;
			messageSent = messageSent.substr(found+1);
			//////cout << "arg[" << i << "]: " << t << endl;
			message.push_back(t);
		}
		//////cout << "I'm standing here: " << To << endl;
		//////cout << "messageSent.size: " << messageSent.size() << endl;
		string t = messageSent;

		//////cout << "arg[" << arg_num-1 << "]: " << t << endl;
		message.push_back(t);
}



vector <string> Message::marshal (){
	string messageSent = "";
	for(int i=0; i < message.size();i++)
	{
		if(i != message.size()-1)
			messageSent += message[i] + ' ';
		else
			messageSent += message[i];
	}

	vector<string> serialized;
	int end = ceil(message_size/float(SIZE-16));


	for(int i = 0; i < end;i++)
	{
		string tmp = "";
		tmp += ShrinkInt(rpc_id);


		operation &= 0x7fffffff;
		tmp+= ShrinkInt((int(message_type)<<31) | operation);
		//////cout << operation << " : " <<(UnShrinkInt(tmp.substr(4,4))&0x7fffffff) << " : " << ((UnShrinkInt(tmp.substr(4,4)) >> 31) & 1) << endl;
		if(i!=end-1)
			tmp+= ShrinkInt( (arg_num<<24) | SIZE-16);		//remove this in the near future
		else
			tmp+= ShrinkInt((arg_num<<24) | message_size- ((SIZE-16)*serialized.size()));


		tmp += ShrinkInt(((i!=(end-1))<<31)|(i&0x7fffffff));
		// ////cout << "packet is: " << (UnShrinkInt(tmp.substr(12,4)) & 0x7fffffff) << endl;
		// ////cout << "is it the end?" << (( UnShrinkInt(tmp.substr(12,4)) >>31)&1) << endl;

		tmp += messageSent.substr(i*(SIZE-16),SIZE-16);

		serialized.push_back(tmp);
	}
	// ////cout << "operationID: " << operation << endl;
	// ////cout << "Message Size: " <<  message_size << endl;
	// ////cout << "RpcID: " <<  rpc_id << endl;
	// ////cout << "argNum: " << arg_num << endl;
	// ////cout << "Message Type: " << message_type << endl;

	return serialized;
}

int Message::getOperation (){

	return operation;
}

int Message::getRPCId(){

	return rpc_id;
}

int Message::getArgNum()
{
	return arg_num;
}

vector<string> Message::getMessage(){

	return message;
}
size_t Message::getMessageSize(){

	return message_size;
}
MessageType Message::getMessageType(){

	return message_type;
}
void Message::setOperation (int _operation){

	this->operation = _operation;
}
void Message::setArgNum (int _arg_num)
{
	arg_num = _arg_num;
}
void Message::setMessage (vector<string> message, size_t message_size){

	this->message = message;
	this->message_size = message_size;
}
void Message::setMessageType (MessageType message_type){

	this->message_type = message_type;
}


pair<string, int> Message::getPeerData()
{
	return peerData;
}

void Message::setPeerData(pair<string, int> _peerData)
{
	peerData = _peerData;
}
Message::~Message(){}
