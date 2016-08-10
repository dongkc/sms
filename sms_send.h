#ifndef SMS_SEND_H
#define SMS_SEND_H
#include <string>
#include <gammu.h>

class SmsSend
{
public:
  SmsSend(const std::string& path);

  uint8_t send(const std::string& phone_num, const std::string& sms);

  virtual ~SmsSend();

private:
  std::string path;
};

#endif /* SMS_SEND_H */
