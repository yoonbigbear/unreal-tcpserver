// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_RESULTCODE_H_
#define FLATBUFFERS_GENERATED_RESULTCODE_H_

#include "flatbuffers/flatbuffers.h"

enum ResultCode : uint16_t {
  ResultCode_None = 0,
  ResultCode_CreateSuccess = 1,
  ResultCode_CreateFailure = 2,
  ResultCode_AleadyExist = 3,
  ResultCode_NotExist = 4,
  ResultCode_LoginSuccess = 5,
  ResultCode_LoginFailure = 6,
  ResultCode_EnterGameSuccess = 7,
  ResultCode_EnterGameFailure = 8,
  ResultCode_MIN = ResultCode_None,
  ResultCode_MAX = ResultCode_EnterGameFailure
};

inline const ResultCode (&EnumValuesResultCode())[9] {
  static const ResultCode values[] = {
    ResultCode_None,
    ResultCode_CreateSuccess,
    ResultCode_CreateFailure,
    ResultCode_AleadyExist,
    ResultCode_NotExist,
    ResultCode_LoginSuccess,
    ResultCode_LoginFailure,
    ResultCode_EnterGameSuccess,
    ResultCode_EnterGameFailure
  };
  return values;
}

inline const char * const *EnumNamesResultCode() {
  static const char * const names[10] = {
    "None",
    "CreateSuccess",
    "CreateFailure",
    "AleadyExist",
    "NotExist",
    "LoginSuccess",
    "LoginFailure",
    "EnterGameSuccess",
    "EnterGameFailure",
    nullptr
  };
  return names;
}

inline const char *EnumNameResultCode(ResultCode e) {
  if (flatbuffers::IsOutRange(e, ResultCode_None, ResultCode_EnterGameFailure)) return "";
  const size_t index = static_cast<size_t>(e);
  return EnumNamesResultCode()[index];
}

#endif  // FLATBUFFERS_GENERATED_RESULTCODE_H_
