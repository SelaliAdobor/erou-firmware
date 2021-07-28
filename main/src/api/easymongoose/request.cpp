#include "request.h"
#include "ArduinoJson.h"

namespace em {
DynamicJsonDocument Request::json = DynamicJsonDocument(1024);
}