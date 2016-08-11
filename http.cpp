#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Poco/Net/ServerSocket.h>
#include <Poco/Net/HTTPServerParams.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPCookie.h>
#include <Poco/Net/NameValueCollection.h>
#include <Poco/AccessExpireCache.h>
#include <Poco/Util/Application.h>
#include <Poco/URI.h>
#include <glog/logging.h>
#include <memory>
#include "http.h"
#include "api.h"

using namespace std;
using namespace Poco;
using namespace Poco::Util;
using namespace Poco::Net;

class RequestHandlerFactory: public HTTPRequestHandlerFactory {
  public:
    RequestHandlerFactory(Http* http)
      : _http(http) {
      }

    HTTPRequestHandler* createRequestHandler(HTTPServerRequest const& request) {

      try {

        URI uri(request.getURI());
        NameValueCollection cookies;

        if (uri.getPath() == "/api/sms/send") {
          return new SMSSend();
        }

      } catch (const std::exception& e) {
        LOG(INFO) << e.what() << endl;
      }

      return new DefaultHandler();
    }

  private:
    Http* _http;
};

Http::Http(): _httpServer(0) {

}

Http::~Http() {

}

const char* Http::name() const {
  return "Http";
}

void Http::initialize()
{
  RequestHandlerFactory* factory = new RequestHandlerFactory(this);
  ServerSocket socket(8000);
  HTTPServerParams* params = new HTTPServerParams;
  _httpServer = new HTTPServer(factory, socket, params);
  _httpServer->start();
}

void Http::uninitialize()
{
  _httpServer->stop();
  delete _httpServer;
  _httpServer = 0;
}

