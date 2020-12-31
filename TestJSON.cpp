#include <deepstate/DeepState.hpp>
using namespace deepstate;

#include <math.h>
#include <string.h>
#include <json.h>

#define MAX_LEN 255

bool equal_enough(struct json_object *o1, struct json_object *o2);

bool object_equal_enough(struct json_object *o1, struct json_object *o2) {
  struct json_object_iter iter;
  struct json_object *sub;

  if (json_object_object_length(o1) != json_object_object_length(o2)) {
    return false;
  }

  /* Iterate over jso1 keys and see if they exist and are equal in jso2 */
  json_object_object_foreachC(o1, iter) {
    sub = json_object_object_get(o2, iter.key);
    if (!equal_enough(iter.val, sub)) {
      return false;
    }
  }

  return true;
}

bool equal_enough(struct json_object *o1, struct json_object *o2) {
  json_type t1 = json_object_get_type(o1);
  json_type t2 = json_object_get_type(o2);
  if ((t1 == json_type_double) && (t2 == json_type_double)) {
    // We have to call NaNs equal!                                                                               
    double d1 = json_object_get_double(o1);
    double d2 = json_object_get_double(o2);
    if (isnan(d1) && isnan(d2)) {
      return true;
    } else {
      return d1 == d2;
    }
  }
  if ((t1 == json_type_double) && (t2 == json_type_int)) {
    // This function is needed because, e.g., "7e" will become a double but string will                          
    // produce an int                                                                                            
    int64_t i1 = json_object_get_int64(o1);
    int64_t i2 = json_object_get_int64(o2);
    if (i1 != i2) {
      LOG(TRACE) << "i1 = " << i1;
      LOG(TRACE) << "i2 = " << i2;
    }
    return (i1 == i2);
  }
  if (t1 != t2) {
    return false; // Types must be equal, otherwise                                                              
  }
  if (t1 == json_type_array) {
    size_t l1 = json_object_array_length(o1);
    size_t l2 = json_object_array_length(o1);
    if (l1 != l2) {
      return false;
    }
    for (size_t i = 0; i < l1; i++) {
      if (!equal_enough(json_object_array_get_idx(o1, i), json_object_array_get_idx(o2, i))) {
        return false;
      }
    }
    return true;
  }
  if (t1 == json_type_object) {
    return object_equal_enough(o1, o2);
  }
  // If not a recursive type or double/int pair, just check equality                                             
  return json_object_equal(o1, o2);
}

TEST(JSON, RoundTrip) {
  size_t size1 = DeepState_UIntInRange(0, MAX_LEN);
  // char* str1 = DeepState_CStr(size1, ",\":{}[]abcdef0123456789E.-\t\n ");                                     
  char* str1 = DeepState_CStr(size1);
  LOG(TRACE) << "String 1:" << str1;
  LOG(TRACE) << "Length 1:" << size1;
  json_tokener *tok1 = json_tokener_new();
  json_object *obj1 = json_tokener_parse_ex(tok1, str1, size1 + 1);
  json_type type1 = json_object_get_type(obj1);
  LOG(TRACE) << "Type 1:" << type1 << " " << json_type_to_name(type1);
  if (obj1 != NULL) {
    const char* str2 = json_object_to_json_string(obj1);
    size_t size2 = strlen(str2);;
    LOG(TRACE) << "String 2:" << str2;
    LOG(TRACE) << "Length 2:" << size2;
    json_tokener *tok2 = json_tokener_new();
    json_object *obj2 = json_tokener_parse_ex(tok2, str2, size2 + 1);
    ASSERT (obj2 != NULL) << "Object 2 should not be null!";
    json_type type2 = json_object_get_type(obj2);
    LOG(TRACE) << "Type 2:" << type2 << " " << json_type_to_name(type2);
    if (!json_object_equal(obj1, obj2)) {
      ASSERT(equal_enough(obj1, obj2)) << "OBJECTS NOT EQUAL:\n\n" << str1 << "\n\n" << str2;
    }
    json_object_put(obj2);
    json_tokener_free(tok2);
  }
  json_object_put(obj1);
  json_tokener_free(tok1);
}
