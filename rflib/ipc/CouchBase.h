#ifndef __COUCHBASE_H__
#define __COUCHBASE_H__

#include "CouchConfig.h"
#include <libcouchbase/couchbase.h>

#include <stdlib.h>

class CouchBase{
private:
	lcb_t instance;    /* our libcouchbase instance */
    lcb_error_t error;  /* for checking various responses */
public:

	/**
	* Inicia a conexao
	* start conncetion
	*/
	CouchBase(const CouchConfig& config);

	/**
	* Finaliza conexao
	* end connection
	*/
	virtual ~CouchBase();


	/**
	* 
	*
	*/
	void store(const string& key,const string& value);

	/**
	*
	*/
	string * load(const string& key);


	/**
	*
	*/
	void remove(const string& key);
	
};

#endif /* __COUCHBASE_H__ */