
#ifndef _UDP_RECEIVER_THREAD_H_
#define _UDP_RECEIVER_THREAD_H_

#include "ofMain.h"
#include "ofxUDPManager.h"

#define UDP_PACKET_BUF_SIZE	(1024 * 10)

extern char* tokenize(char *buf, char **argv, int &argc);


class testApp;

class UDPReceiverThread : public ofThread {
	testApp *app;
	int port;
	char buf[UDP_PACKET_BUF_SIZE];
	char address[1024];
	ofxUDPManager receiver;

public:

	UDPReceiverThread();

	void setup(testApp *app, int port);
	
	void threadedFunction();
};

#endif

