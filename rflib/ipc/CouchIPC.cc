#include "CouchIPC.h"
#include <stdio.h>
#include <algorithm> 

CouchIPCMessageService::CouchIPCMessageService(const std::list<CouchConfig>& configs, const string& id): confs(configs){
	IPCMessageService::set_id(id);
}

CouchIPCMessageService::CouchIPCMessageService(const CouchConfig& config, const string& id) : confs(1,config){
	IPCMessageService::set_id(id);
}


void CouchIPCMessageService::listenWorker(const string &channelId, IPCMessageFactory *factory, IPCMessageProcessor *processor){
    /*string ns = this->db + "." + channelId;

    mongo::DBClientConnection connection;
    this->connect(connection, this->address);

    this->createChannel(connection, ns);
    mongo::Query query = QUERY(TO_FIELD << this->get_id() << READ_FIELD << false).sort("$natural");*/
    while (true) {
        /*auto_ptr<mongo::DBClientCursor> cur = connection.query(ns, query,
                                                               PENDINGLIMIT);
        while (cur->more()) {
            mongo::BSONObj envelope = cur->nextSafe();
            IPCMessage *msg = takeFromEnvelope(envelope, factory);
            processor->process(envelope["from"].String(), this->get_id(), channelId, *msg);
            delete msg;

            connection.update(ns,
                QUERY("_id" << envelope["_id"]), 
                BSON("$set" << BSON(READ_FIELD << true)),
                false, true);
        }*/
        usleep(50000); // 50ms
    }
}

void CouchIPCMessageService::listen(const string& channelId, IPCMessageFactory *factory, IPCMessageProcessor *processor, bool block){
    boost::thread t(&CouchIPCMessageService::listenWorker, this, channelId, factory, processor);
    if (block)
        t.join();
    else
        t.detach();
}

bool CouchIPCMessageService::send(const string& channelId, const string& to, IPCMessage& msg){
	boost::lock_guard<boost::mutex> lock(this->ipcMutex);
	this->openConnection();
	this->couch->store(channelId,putInEnvelope(this->get_id(),to,msg).toStyledString());
	this->closeConnection();
	return true;
}

void CouchIPCMessageService::closeConnection(){
	delete this->couch;
}

void CouchIPCMessageService::openConnection(){
	
	list<CouchConfig>::iterator it;
	for(it = this->confs.begin();it != this->confs.end();it++){
		try{
			this->couch = new CouchBase(*it);
			break;
		}
		catch(exception * ex){
			delete ex;
			this->couch = NULL;
		}
	}
}

Json::Value CouchIPCMessageService::putInEnvelope(const std::string& from,const std::string& to, IPCMessage& msg){
	Json::Value jsonEnvelope;
	Json::Value jsonMsg;
	Json::Reader reader;
	jsonEnvelope["from"] = from;
	jsonEnvelope["to"] = to;
	jsonEnvelope["type"] = msg.get_type();
	jsonEnvelope["read"] = false;
	string str(msg.toJSON());
	const char * strMsg = str.c_str();
	if(reader.parse(strMsg,jsonMsg)){
		jsonEnvelope["content"] = jsonMsg;
	}
	return jsonEnvelope;
}

IPCMessage * CouchIPCMessageService::takeFromEnvelope(Json::Value envelope,IPCMessageFactory * factory){
   IPCMessage* msg = factory->buildForType(envelope["type"].asInt());
   msg->fromJSON(envelope["content"].toStyledString().c_str());
   return msg;
}