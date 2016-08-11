#include <fstream>
#include <iostream>
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
  response.setChunkedTransferEncoding(true);
  response.setContentType("text/html");

  std::ostream& ostr = response.send();
  ostr << "dongkechang";
}

const std::string DefaultHandler::PATH("/default");


void DefaultHandler::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
  response.setChunkedTransferEncoding(true);
  response.setContentType("text/html");

  response.setStatusAndReason(HTTPServerResponse::HTTP_NOT_FOUND, HTTPServerResponse::HTTP_REASON_FORBIDDEN);
  std::ostream& ostr = response.send();
  ostr << "ACCESS DENIED!";
}
