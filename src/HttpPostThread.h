#ifndef _HTTP_POST_THREAD_H_
#define _HTTP_POST_THREAD_H_

#include "ofMain.h"

#include "ofThread.h"

class HttpPostThread : public ofThread {

public:

	int res_status;

	std::string url;
	std::string postbody;
	std::string response_body;
	bool done;

	HttpPostThread();
	~HttpPostThread();

	int post(std::string url, std::string body);

	void threadedFunction();

};

#endif
