#include <stdlib.h>
#include "CouchConfig.h"


CouchConfig::CouchConfig():
host("localhost:8091"),
user(""),
password(""),
bucket("default"){

}

CouchConfig::CouchConfig(const string& host,const string& username,const string& password):
host(host),
user(username),
password(password),
bucket("default"){

}

CouchConfig::CouchConfig(const string& host,const string& username,const string& password, const string& bucket):
host(host),
user(username),
password(password),
bucket(bucket){

}

CouchConfig::~CouchConfig(){
	
}

void CouchConfig::setHost(const string& host){
	this->host.assign(host);
}

void CouchConfig::setUser(const string& username){
	this->user.assign(username);
}

void CouchConfig::setPassword(const string& password){
	this->password.assign(password);
}
	
void CouchConfig::setBucket(const string& bucket){
	this->bucket.assign(bucket);
}

const string& CouchConfig::getHost() const {
	return this->host;
}

const string& CouchConfig::getUser() const {
	return this->user;
}

const string& CouchConfig::getPassword() const {
	return this->password;
}

const string& CouchConfig::getBucket() const {
	return this->bucket;
}

struct lcb_create_st * CouchConfig::makeOptions() const{
	struct lcb_create_st * options;
	options = (struct lcb_create_st *)malloc(sizeof(struct lcb_create_st));
    memset(options, 0, sizeof(*options));
    options->v.v0.host = this->host.c_str();
    options->v.v0.user = this->user.c_str();
    options->v.v0.passwd = this->password.c_str();
    options->v.v0.bucket = this->bucket.c_str();
    return options;
}
