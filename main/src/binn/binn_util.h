#include <memory>
#include <functional>
#include <binn.h>
#include <wsdebug.h>
inline std::unique_ptr<binn, std::function<void(binn *)>> safeBinnCreate(void *obj) {
  return std::unique_ptr<binn, std::function<void(binn *)>>(
      static_cast<binn *>(obj), [&](binn *ptr) {
        debugV("About to delete binn resource");
        binn_free(ptr);
        debugV("Deleted binn resource");
      });
}