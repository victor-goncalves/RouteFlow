#include "CouchBase.h"

#include <exception>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>


static void error_callback(lcb_t instance,lcb_error_t err,const char *errinfo){
    fprintf(stderr, "Error %s: %s", lcb_strerror(instance, err),errinfo ? errinfo : "");
    exit(EXIT_FAILURE);
}

static void get_callback(lcb_t instance,const void *cookie,lcb_error_t error,const lcb_get_resp_t *resp){
    if (error != LCB_SUCCESS) {
        fprintf(stderr, "Failed to retrieve \"");
        fwrite(resp->v.v0.key, 1, resp->v.v0.nkey, stderr);
        fprintf(stderr, "\": %s\n", lcb_strerror(instance, error));
        fprintf(stderr, "\n");
    }
    else {
        fprintf(stderr, "Data for key: \"");
        fwrite(resp->v.v0.key, 1, resp->v.v0.nkey, stderr);
        fprintf(stderr, "\" is : ");
        fwrite(resp->v.v0.bytes, 1, resp->v.v0.nbytes, stderr);
        fprintf(stderr, "\n");
    }
}


CouchBase::CouchBase(const CouchConfig& config){
	struct lcb_create_st * options = config.makeOptions();
    this->error = lcb_create(&(this->instance),options);
    if (this->error != LCB_SUCCESS) {
        fprintf(stderr, "Failed to create libcouchbase instance: %s\n",
                lcb_strerror(NULL,this->error));
        throw new exception();
    }


    /* Set up the handler to catch all errors! */
    lcb_set_error_callback(this->instance, error_callback);

    /*
     * Initiate the connect sequence in libcouchbase
     */
    if ((this->error = lcb_connect(this->instance)) != LCB_SUCCESS) {
    	/* exception */
        fprintf(stderr, "Failed to initiate connect: %s\n",
                lcb_strerror(NULL,this->error));
        throw new exception();
    }

    /* Run the event loop and wait until we've connected */
    lcb_wait(this->instance);

    /*
     * Set up a callback for our get requests
     */
    lcb_set_get_callback(this->instance, get_callback);
    free(options);
}

CouchBase::~CouchBase(){
    lcb_wait(instance);
	lcb_destroy(this->instance);
}

void CouchBase::store(const string& key, const string& value){
    lcb_error_t err;
    lcb_store_cmd_t storeCommand;
    const lcb_store_cmd_t * commands[1];
    commands[0] = &storeCommand;
    memset(&storeCommand,0,sizeof(storeCommand));
    storeCommand.v.v0.operation = LCB_SET;

    storeCommand.v.v0.key = key.data();
    storeCommand.v.v0.nkey = key.size();

    storeCommand.v.v0.bytes = value.data();
    storeCommand.v.v0.nbytes = value.size();
    err = lcb_store(this->instance, NULL, 1, commands);
    if (err != LCB_SUCCESS) {
        fprintf(stderr, "Failed to set: %s\n", lcb_strerror(NULL, err));
        throw new exception();
    }
}


void CouchBase::remove(const string& key){
     lcb_remove_cmd_t *remove = (lcb_remove_cmd_t *)calloc(1, sizeof(*remove));
     remove->version = 0;
     remove->v.v0.key = key.data();
     remove->v.v0.nkey = key.size();
     remove->v.v0.cas = 0x000;
     lcb_remove_cmd_t* commands[] = { remove };
     lcb_remove(instance, NULL, 1, commands);
}

string * CouchBase::load(const string& key){
    lcb_error_t err;
    lcb_get_cmd_t cmd;
    const lcb_get_cmd_t * const commands[1] = { &cmd };
    memset(&cmd, 0, sizeof(cmd));

    const char * k = key.c_str();
    cmd.v.v0.key = k;
    cmd.v.v0.nkey = key.size();

    err = lcb_get(this->instance, NULL, 1, commands);
    if (err != LCB_SUCCESS) {
        fprintf(stderr, "Failed to get: %s\n",
                lcb_strerror(NULL, err));
        throw new exception();
    }

    return new string();
}
