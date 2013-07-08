#ifndef __COUCHCONFIG_H__
#define __COUCHCONFIG_H__

#include <libcouchbase/couchbase.h>
#include <string>

using namespace std;


class CouchConfig{
private:
	string host;
	string user;
	string password;
	string bucket;
public:
	CouchConfig();
	CouchConfig(const string& host,const string& username,const string& password);
	CouchConfig(const string& host,const string& username,const string& password, const string& bucket);
	virtual ~CouchConfig();
	void setHost(const string&);
	void setUser(const string&);
	void setPassword(const string&);
	void setBucket(const string&);
	const string& getHost() const;
	const string& getUser() const;
	const string& getPassword() const;
	const string& getBucket() const;
	struct lcb_create_st * makeOptions() const;

};

#endif /* __COUCHCONFIG_H__ */