
#include "STClient.h"

STClient::STClient(std::string name, std::string address, std::string number) {
	this->name = name;
	this->address = address;
	this->number = number;
	this->status = "none";
};

void STClient::message(char *argv[], int argc) {

};

