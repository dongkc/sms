#include <fstream>
#include <unistd.h>
#include <string>
#include <Poco/StreamCopier.h>
#include "api.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTMLForm.h"

#include "Poco/DateTime.h"
#include "Poco/DateTimeParser.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"

using namespace Poco::Net;
const std::string SMSSend::PATH("/api/sms/send");


void SMSSend::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
}

const std::string DefaultHandler::PATH("/default");


void DefaultHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
  response.setStatusAndReason(HTTPServerResponse::HTTP_NOT_FOUND, HTTPServerResponse::HTTP_REASON_FORBIDDEN);
}
