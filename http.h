#ifndef Web_Http_INCLUDED
#define Web_Http_INCLUDED

#include <Poco/Net/HTTPServer.h>


class Http{
public:
    Http();

    ~Http();

    const char* name() const;

    void initialize();

    void uninitialize();

private:
    Poco::Net::HTTPServer* _httpServer;
};
#endif  // Web_Http_INCLUDED
