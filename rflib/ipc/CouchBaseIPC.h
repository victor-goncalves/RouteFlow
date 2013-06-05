#ifndef __MONGOIPC_H__
#define __MONGOIPC_H__

#include <libcouchbase/couchbase.h>
#include "IPC.h"

#define FROM_FIELD "from"
#define TO_FIELD "to"
#define TYPE_FIELD "type"
#define READ_FIELD "read"
#define CONTENT_FIELD "content"

// 1 MB for the capped collection
//CouchBase nao suporta capped collection
//#define CC_SIZE 1048576

// Handle a maximum of 10 messages at a time
#define PENDINGLIMIT 10

//CouchBase nao suporta BSON
//mongo::BSONObj putInEnvelope(const string &from, const string &to, IPCMessage &msg);
//IPCMessage* takeFromEnvelope(mongo::BSONObj envelope, IPCMessageFactory *factory);

/** An IPC message service that uses CouchBase as its backend. */
class CouchBaseIPCMessageService : public IPCMessageService {
    public:
        /** Creates and starts an IPC message service using CouchBase.
        @param address the address and port of the mongo server in the format 
                       address:port
        @param db the name of the database to use
        @param id the ID of this IPC service user */
        CouchBaseIPCMessageService(const string &address, const string db, const string id);
        virtual void listen(const string &channelId, IPCMessageFactory *factory, IPCMessageProcessor *processor, bool block=true);
        virtual bool send(const string &channelId, const string &to, IPCMessage& msg);
        
    private:
        string db;
        string address;
        CouchBase::connect producerConnection;
        boost::muwtex ipcMutex;
        void listenWorker(const string &channelId, IPCMessageFactory *factory, IPCMessageProcessor *processor);
//        void createChannel(mongo::DBClientConnection &con, const string &ns);
        void connect(CouchBase::connect &connection, const string &address);

};

#endif /* __COUCHBASEIPC_H__ */
