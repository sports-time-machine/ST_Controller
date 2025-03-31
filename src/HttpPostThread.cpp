#include "HttpPostThread.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/URI.h"

#include <istream>

using namespace std;
using namespace Poco::Net;
using namespace Poco;

HttpPostThread::HttpPostThread() = default;

HttpPostThread::~HttpPostThread()
{
    if (isThreadRunning())
    {
        stopThread();
    }
}

int HttpPostThread::post(const std::string& url, const std::string& body)
{
    this->url = url;
    this->postbody = body;
    this->done = false;
    startThread();
    printf("post thread started\n");
    return 0;
}

void HttpPostThread::threadedFunction()
{
    try
    {
        URI uri(url);
        std::string path(uri.getPathAndQuery());
        cout << "post: " << uri.getPathAndQuery() << "\n---\n" << postbody << endl;
        if (path.empty()) path = "/";

        HTTPClientSession session(uri.getHost(), uri.getPort());
        HTTPRequest req(HTTPRequest::HTTP_POST, path, HTTPMessage::HTTP_1_1);

        // JSONコンテンツタイプを設定
        req.setContentType("application/json");

        // コンテンツ長を設定
        req.setContentLength(postbody.length());

        // リクエストボディを直接書き込む
        std::ostream& os = session.sendRequest(req);
        os << postbody;

        static std::string response;
        HTTPResponse res;
        std::istream& rs = session.receiveResponse(res);

        res_status = res.getStatus();
        ofBuffer body(rs);
        response = body.getText();

        printf_s("DB RESPONSE:%d | %s\n", res_status, response.c_str());

        response_body = response;
    }
    catch (Exception& exc)
    {
        std::cerr << "HttpPost error--\n";
        std::cerr << exc.displayText() << std::endl;
    }

    done = true;
}