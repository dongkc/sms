#include <Poco/Util/ServerApplication.h>
#include <Poco/Util/Option.h>
#include <Poco/Util/OptionSet.h>
#include <Poco/Util/HelpFormatter.h>
#include <glog/logging.h>
#include <gflags/gflags.h>

#include "http.h"
#include "sms_send.h"

using Poco::Util::ServerApplication;
using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::HelpFormatter;

using namespace std;

int main_(int argc, char *argv[])
{
  google::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();

  SmsSend sms("/root/.gammurc");

  sms.send("13488845771", "动测试");
  return 0;
}

class RTUServer: public Poco::Util::ServerApplication
{
  public:
    RTUServer()
    {
    }

    ~RTUServer()
    {
    }

  protected:
    void initialize(Application& self)
    {
      loadConfiguration(); // load default configuration files, if present
      ServerApplication::initialize(self);
    }

    void uninitialize()
    {
      ServerApplication::uninitialize();
    }

    int main(const std::vector<std::string>& args)
    {
      Http http_server;
      http_server.initialize();

      waitForTerminationRequest();

      http_server.uninitialize();

      return Application::EXIT_OK;
    }
};

int main(int argc, char** argv)
{
  google::ParseCommandLineFlags(&argc, &argv, true);
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();

  RTUServer server;

  server.run(argc, argv);
}
