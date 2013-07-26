#ifndef _ST_CLIENT_H_
#define _ST_CLIENT_H_

#include "ofMain.h"

class STClient {
public:
	std::string address;
	std::string name;
	std::string number;
	std::string status;

	STClient(std::string name, std::string address, std::string number);

	virtual void message(char *argv[], int argc);

};

#endif

