#include <fstream>
#include <iostream>
#include <unistd.h>
#include <string>
#include <Poco/StreamCopier.h>
#include <gammu.h>
#include "api.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/HTMLForm.h"

#include "Poco/DateTime.h"
#include "Poco/DateTimeParser.h"
#include "Poco/DateTimeFormatter.h"
#include "Poco/DateTimeFormat.h"

using namespace std;
using namespace Poco::Net;
const std::string SMSSend::PATH("/api/sms/send");

namespace
{
  GSM_StateMachine *s;
  INI_Section *cfg;
  GSM_Error error;
  volatile GSM_Error sms_send_status;
  volatile gboolean gshutdown = FALSE;

  /* Handler for SMS send reply */
  void send_sms_callback (GSM_StateMachine *sm, int status, int MessageReference, void * user_data)
  {
    printf("Sent SMS on device: \"%s\"\n", GSM_GetConfig(sm, -1)->Device);
    if (status==0) {
      printf("..OK");
      sms_send_status = ERR_NONE;
    } else {
      printf("..error %i", status);
      sms_send_status = ERR_UNKNOWN;
    }
    printf(", message reference=%d\n", MessageReference);
  }

  /* Function to handle errors */
  int error_handler(void)
  {
    if (error != ERR_NONE) {
      printf("ERROR: %s\n", GSM_ErrorString(error));
      if (GSM_IsConnected(s))
        GSM_TerminateConnection(s);
    }
    return error;
  }

  int send_sms(const string& rc_path, const string& phone, const string& text)
  {
    int return_value = 0;

    GSM_SMSMessage sms;
    GSM_SMSC PhoneSMSC;
    GSM_Debug_Info *debug_info;

    /*
     *   * We don't need gettext, but need to set locales so that
     *     * charset conversion works.
     *       */
    GSM_InitLocales(NULL);

    /* Enable global debugging to stderr */
    debug_info = GSM_GetGlobalDebug();
    GSM_SetDebugFileDescriptor(stderr, TRUE, debug_info);
    GSM_SetDebugLevel("textall", debug_info);

    /* Prepare message */
    /* Cleanup the structure */
    memset(&sms, 0, sizeof(sms));
    EncodeUnicode(sms.Text, text.data(), text.length());
    /* Encode recipient number */
    EncodeUnicode(sms.Number, phone.data(), phone.length());
    /* Encode message text */
    /* Encode recipient number */
    /* We want to submit message */
    sms.PDU = SMS_Submit;
    /* No UDH, just a plain message */
    sms.UDH.Type = UDH_NoUDH;
    /* We used default coding for text */
    sms.Coding = SMS_Coding_Unicode_No_Compression;
    /* Class 1 message (normal) */
    sms.Class = 1;

    /* Allocates state machine */
    s = GSM_AllocStateMachine();
    if (s == NULL)
      return 3;

    /*
     *   * Enable state machine debugging to stderr
     *     * Same could be achieved by just using global debug config.
     *       */
    debug_info = GSM_GetDebug(s);
    GSM_SetDebugGlobal(FALSE, debug_info);
    GSM_SetDebugFileDescriptor(stderr, TRUE, debug_info);
    GSM_SetDebugLevel("textall", debug_info);

    /*
     *   * Find configuration file (first command line parameter or
     *     * defaults)
     *       */

    error = GSM_FindGammuRC(&cfg, rc_path.data());
    if (error_handler() != ERR_NONE) {
      return error;
    };

    /* Read it */
    error = GSM_ReadConfig(cfg, GSM_GetConfig(s, 0), 0);
    if (error_handler() != ERR_NONE) {
      return error;
    };

    /* Free config file structures */
    INI_Free(cfg);

    /* We have one valid configuration */
    GSM_SetConfigNum(s, 1);

    /* Connect to phone */
    /* 1 means number of replies you want to wait for */
    error = GSM_InitConnection(s, 1);
    if (error_handler() != ERR_NONE) {
      return error;
    };

    /* Set callback for message sending */
    /* This needs to be done after initiating connection */
    GSM_SetSendSMSStatusCallback(s, send_sms_callback, NULL);

    /* We need to know SMSC number */
    PhoneSMSC.Location = 1;
    error = GSM_GetSMSC(s, &PhoneSMSC);
    if (error_handler() != ERR_NONE) {
      return error;
    };

    /* Set SMSC number in message */
    CopyUnicodeString(sms.SMSC.Number, PhoneSMSC.Number);

    /*
     *   * Set flag before callind SendSMS, some phones might give
     *     * instant response
     *       */
    sms_send_status = ERR_TIMEOUT;

    /* Send message */
    error = GSM_SendSMS(s, &sms);
    if (error_handler() != ERR_NONE) {
      return error;
    };

    /* Wait for network reply */
    while (!gshutdown) {
      GSM_ReadDevice(s, TRUE);
      if (sms_send_status == ERR_NONE) {
        /* Message sent OK */
        return_value = 0;
        break;
      }
      if (sms_send_status != ERR_TIMEOUT) {
        /* Message sending failed */
        return_value = 100;
        break;
      }
    }

    /* Terminate connection */
    error = GSM_TerminateConnection(s);
    if (error_handler() != ERR_NONE) {
      return error;
    };

    /* Free up used memory */
    GSM_FreeStateMachine(s);

    return return_value;
  }

}

void SMSSend::handleRequest(Poco::Net::HTTPServerRequest& request, Poco::Net::HTTPServerResponse& response)
{
  HTMLForm form(request, request.stream());
  response.setChunkedTransferEncoding(true);
  response.setContentType("text/html");

  std::ostream& ostr = response.send();
  NameValueCollection::ConstIterator it = request.begin();
  NameValueCollection::ConstIterator end = request.end();

#if 0
  for (; it != end; ++it)
  {
    ostr << it->first << ": " << it->second << "<br>\n";
  }
  ostr << "</p>";
#endif

  string phone;
  string sms_text;

  if (!form.empty())
  {
    //ostr << "<h2>Form</h2><p>\n";
    it = form.begin();
    end = form.end();
    for (; it != end; ++it)
    {
      //ostr << it->first << ": " << it->second << "<br>\n";
      if (it->first == "phone") {
        phone= it->second;
      }
      if (it->first == "sms") {
        sms_text = it->second;
      }
    }
    //ostr << "</p>";
    if (phone.size() && sms_text.size()) {
      int ret = send_sms("/etc/gammurc", phone, sms_text);
      ostr << ret;
    } else {
      ostr << "params not found!";
    }
  }

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
