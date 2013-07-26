
#include "HttpPostThread.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTMLForm.h"
#include "Poco/URI.h"

#include <istream>

using namespace std;
using namespace Poco::Net;
using namespace Poco;

HttpPostThread::HttpPostThread()
{
}

HttpPostThread::~HttpPostThread()
{
	if (isThreadRunning()) {
		stopThread();
	}
}

int HttpPostThread::post(std::string url, std::string body)
{
	this->url = url;
	this->postbody = body;
	this->done = false;
   	startThread(true, false);
    printf("post thread started\n");
	return 0;
}

void HttpPostThread::threadedFunction()
{

	try{
        URI uri(url);
        std::string path(uri.getPathAndQuery());
        cout << "post: "<< uri.getPathAndQuery() << "\n---\n" << postbody << endl;
        if (path.empty()) path = "/";

        HTTPClientSession session(uri.getHost(), uri.getPort());
        HTTPRequest req(HTTPRequest::HTTP_POST, path, HTTPMessage::HTTP_1_1);

#if 0
		HTTPBasicCredentials auth;
		auth.setUsername(username);
		auth.setPassword(password);
		auth.authenticate(req);
#endif

		HTMLForm pocoForm;
        // create the form data to send
       	pocoForm.setEncoding(HTMLForm::ENCODING_URL);

		pocoForm.set(std::string("json"), postbody);

        pocoForm.prepareSubmit(req);
        pocoForm.write(session.sendRequest(req));

		static std::string response;
        HTTPResponse res;
        std::istream& rs = session.receiveResponse(res);

		res_status = res.getStatus();
		ofBuffer body(rs);
		response = body.getText();

		printf_s("DB RESPONSE:%d | %s\n", res_status, response.c_str());

		response_body = response;

		/*
		if (res_status != 200) {
			error_content = "O.K.";
		} else {
			error_contents = response_body;
		}
		*/


	} catch (Exception& exc) {

    	std::cerr << "HttpPost error--\n";

        std::cerr << exc.displayText() << std::endl;
	}

	done = true;
}

