#include <container.h>
#include "api.h"
#include <arpa/inet.h>
#include "easymongoose.h"
#include "wsdebug.h"
void Api::setup() {
   if (!em::isListening()){
    debugE(logtags::network,"EasyMongoose is not listening");
  }

  em::registerRoute("/container", em::method::Value::Get, [=](auto &&...args) { return addContainer(args...); });
  em::registerRoute("/reset", em::method::Value::Get, [=](const em::Request&, em::Response res) {
    storedSettings.reset();
    res.sendText(em::Ok, "Reset stored settings");
  });
  em::registerRoute("/container/:id", em::method::Value::Get, [=](auto &&...args) { return getContainer(args...); });

}