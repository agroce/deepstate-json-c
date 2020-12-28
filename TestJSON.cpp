#include <deepstate/DeepState.hpp>
using namespace deepstate;

#include <json.h>

#define MAX_LEN 255

TEST(JSON, RoundTrip) {
  size_t size1 = DeepState_UIntInRange(0, MAX_LEN);
  char* str1 = DeepState_CStr(size1, ",\":{}[]abcdef0123456789E.-\t\n ");
  LOG(TRACE) << "String 1:" << str1;
  LOG(TRACE) << "Length 1:" << size1;
  json_tokener *tok1 = json_tokener_new();
  json_object *obj1 = json_tokener_parse_ex(tok1, str1, size1 + 1);
  LOG(TRACE) << "Type 1:" << json_object_get_type(obj1);
  if (obj1 != NULL) {
    const char* str2 = json_object_to_json_string(obj1);
    size_t size2 = strlen(str2);;
    LOG(TRACE) << "String 2:" << str2;
    LOG(TRACE) << "Length 2:" << size2;
    json_tokener *tok2 = json_tokener_new();    
    json_object *obj2 = json_tokener_parse_ex(tok2, str2, size2 + 1);
    ASSERT (obj2 != NULL) << "Object 2 should not be null!";
    LOG(TRACE) << "Type 2:" << json_object_get_type(obj2);
    ASSERT (json_object_equal(obj1, obj2)) << "NOT EQUAL:\nSTRING 1:\n" << str1 << "\nSTRING 2:\n" << str2;
    json_object_put(obj2);
    json_tokener_free(tok2);    
  }
  json_object_put(obj1);
  json_tokener_free(tok1);
}
