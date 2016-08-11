#ifndef EXPORT_LOG_INCLUDED
#define EXPORT_LOG_INCLUDED


#include "Poco/Net/HTTPRequestHandler.h"


class SMSSend: public Poco::Net::HTTPRequestHandler
{
  public:
    void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);

    static const std::string PATH;
};

class DefaultHandler: public Poco::Net::HTTPRequestHandler
{
  public:
    void handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response);

    static const std::string PATH;
};

#endif // images_button_gif_INCLUDED
