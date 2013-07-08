#include <json/json.h>
#include <jsoncpp/json/json.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

const char * test(){
	Json::Value val;
	val["teste2"] = 128;
	return val.toStyledString().c_str();
}

int main(int argc,char ** argv){
	Json::Value root;   // will contains the root value after parsing.
	Json::Reader reader;
	bool parsingSuccessful = reader.parse( "{\"encoding\" : \"UTF-8\", \"plug-ins\" : [\"python\",\"c++\",\"ruby\"],\"indent\" : { \"length\" : 3, \"use_space\": true }}", root );
	std::cout << root["encoding"].asString() << std::endl;
	std::cout << root["plug-ins"].size() << std::endl;

	Json::Value json;
	json["teste"] = 1;

	std::cout << json.toStyledString() << std::endl;

	std::cout << test() << std::endl;
}