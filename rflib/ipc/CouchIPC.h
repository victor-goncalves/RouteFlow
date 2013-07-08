#ifndef __COUCH_H__
#define __COUCH_H__


#include "IPC.h"
#include "CouchBase.h"
#include "CouchConfig.h"
#include <libcouchbase/couchbase.h>
#include <list>
#include <json/json.h>
#include <jsoncpp/json/json.h>
#include <boost/thread.hpp>


class CouchIPCMessageService : public IPCMessageService {
private:
	CouchBase * couch;
	boost::mutex ipcMutex;
	std::list<CouchConfig> confs;
	Json::Value putInEnvelope(const std::string& from,const std::string& to,IPCMessage& msg);
	IPCMessage *takeFromEnvelope(Json::Value envelope,IPCMessageFactory * factory);
	void openConnection();
	void closeConnection();
	void listenWorker(const string &channelId, IPCMessageFactory *factory, IPCMessageProcessor *processor);
public:
	CouchIPCMessageService(const std::list<CouchConfig>& configs, const string& id);
	CouchIPCMessageService(const CouchConfig& config, const string& id);
	void listen(const string& channelId, IPCMessageFactory *factory, IPCMessageProcessor *processor, bool block=true);
	bool send(const string& channelId, const string& to, IPCMessage& msg);
};


#endif /* __COUCH_H__ */