#include "CouchBaseIPC.h"
#include <boost/thread.hpp>

CouchBaseIPCMessageService::CouchBaseIPCMessageService(const string &address, const string db, const string id) {
    this->set_id(id);
    this->db = db;
    this->address = address;
    this->connect(producerConnecion, this->address);
}

/*void CouchBaseIPCMessageService::createChannel(mongo::DBClientConnection &con, const string &ns) {
    con.createCollection(ns, CC_SIZE, true);
    con.ensureIndex(ns, BSON("_id" << 1));
    con.ensureIndex(ns, BSON(TO_FIELD << 1));
}*/

void CouchBaseMessageService::connect(Couchbase::connect &connection, const string &address) {
    //No CouchBase nao e possivel detectar se o servidor esta caido. Isso nao chega a ser um problema porque, como o CouchBase funciona em modo cluster, um nodo caido pode ser substituido por outro, alem do servidor se recuperar automaticamente e rapidamente de erros.
    conection.connect(address);
}
void CouchBaseIPCMessageService::listenWorker(const string &channelId, IPCMessageFactory *factory, IPCMessageProcessor *processor) {
    string ns = this->db + "." + channelId;

    Couchbase::connect connection;
    this->connect(connection, this->address);

//    this->createChannel(connection, ns);
    string query = "QUERY(TO_FIELD << this->get_id() << READ_FIELD << false).sort('$natural');"
    while (true) {
        auto_ptr<mongo::DBClientCursor> cur = connection.query(ns, query,
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
        }
        usleep(50000); // 50ms
    }
}

void MongoIPCMessageService::listen(const string &channelId, IPCMessageFactory *factory, IPCMessageProcessor *processor, bool block) {
    boost::thread t(&MongoIPCMessageService::listenWorker, this, channelId, factory, processor);
    if (block)
        t.join();
    else
        t.detach();
}

bool MongoIPCMessageService::send(const string &channelId, const string &to, IPCMessage& msg) {
    boost::lock_guard<boost::mutex> lock(ipcMutex);
    string ns = this->db + "." + channelId;

    this->createChannel(producerConnection, ns);
    this->producerConnection.insert(ns, putInEnvelope(this->get_id(), to, msg));

    return true;
}

/*mongo::BSONObj putInEnvelope(const string &from, const string &to, IPCMessage &msg) {
    mongo::BSONObjBuilder envelope;

    envelope.genOID();
    envelope.append(FROM_FIELD, from);
    envelope.append(TO_FIELD, to);
    envelope.append(TYPE_FIELD, msg.get_type());
    envelope.append(READ_FIELD, false);

    const char* data = msg.to_BSON();
    envelope.append(CONTENT_FIELD, mongo::BSONObj(data));
    delete data;

    return envelope.obj();
}

IPCMessage* takeFromEnvelope(mongo::BSONObj envelope, IPCMessageFactory *factory) {
   IPCMessage* msg = factory->buildForType(envelope[TYPE_FIELD].Int());
   msg->from_BSON(envelope[CONTENT_FIELD].Obj().objdata());
   return msg;
}*/
