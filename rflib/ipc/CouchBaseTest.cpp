#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "CouchIPC.h"
#include "CouchConfig.h"
#include "CouchBase.h"
#include "RFProtocol.h"



using namespace std;

int main(int argc,char ** argv){
	if(argc != 6){
		printf("error - falta de argumentos\n");
		printf("%s host username password key data\n",argv[0]);
		return -1;
	}
	/*CouchBase cb(config);
	cb.store(std::string(argv[4]),std::string(argv[5]));
	printf("%s\n",str->c_str());*/
	CouchConfig config(argv[1],argv[2],argv[3]);
	string strId("icp_id");
	CouchIPCMessageService * ipc = new CouchIPCMessageService(config,strId);
	IPCMessage * message= new PortRegister();
	cout << message->toJSON() << endl;
	ipc->send(strId,strId,*message);
	delete message;

	message = new PortConfig();
	cout << message->toJSON() << endl;
	ipc->send(strId+'1',strId,*message);
	delete message;

	message = new DatapathPortRegister();
	cout << message->toJSON() << endl;
	ipc->send(strId+'2',strId,*message);
	delete message;

	message = new DatapathDown();
	cout << message->toJSON() << endl;
	ipc->send(strId+'3',strId,*message);
	delete message;

	message = new VirtualPlaneMap();
	cout << message->toJSON() << endl;
	ipc->send(strId+'4',strId,*message);
	delete message;

	message = new DataPlaneMap();
	ipc->send(strId+'5',strId,*message);
	cout << message->toJSON() << endl;
	delete message;

	message = new RouteMod();
	ipc->send(strId+'6',strId,*message);
	cout << message->toJSON() << endl;
	delete message;
	
}

