// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_ACCOUNT_ACCOUNT_H_
#define FLATBUFFERS_GENERATED_ACCOUNT_ACCOUNT_H_

#include "flatbuffers/flatbuffers.h"

#include "common_generated.h"

namespace account {

struct LoginReq;
struct LoginReqBuilder;

struct LoginAck;
struct LoginAckBuilder;

struct CreateAccountReq;
struct CreateAccountReqBuilder;

struct CreateAccountAck;
struct CreateAccountAckBuilder;

struct SelectCharacterReq;
struct SelectCharacterReqBuilder;

struct SelectCharacterAck;
struct SelectCharacterAckBuilder;

struct CreateCharacterReq;
struct CreateCharacterReqBuilder;

struct CreateCharacterAck;
struct CreateCharacterAckBuilder;

struct CheckCharacterNicknameReq;
struct CheckCharacterNicknameReqBuilder;

struct CheckCharacterNicknameAck;
struct CheckCharacterNicknameAckBuilder;

struct LoginReq FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef LoginReqBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_ID = 4,
    VT_PW = 6
  };
  const flatbuffers::String *id() const {
    return GetPointer<const flatbuffers::String *>(VT_ID);
  }
  const flatbuffers::String *pw() const {
    return GetPointer<const flatbuffers::String *>(VT_PW);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_ID) &&
           verifier.VerifyString(id()) &&
           VerifyOffset(verifier, VT_PW) &&
           verifier.VerifyString(pw()) &&
           verifier.EndTable();
  }
};

struct LoginReqBuilder {
  typedef LoginReq Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_id(flatbuffers::Offset<flatbuffers::String> id) {
    fbb_.AddOffset(LoginReq::VT_ID, id);
  }
  void add_pw(flatbuffers::Offset<flatbuffers::String> pw) {
    fbb_.AddOffset(LoginReq::VT_PW, pw);
  }
  explicit LoginReqBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<LoginReq> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<LoginReq>(end);
    return o;
  }
};

inline flatbuffers::Offset<LoginReq> CreateLoginReq(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> id = 0,
    flatbuffers::Offset<flatbuffers::String> pw = 0) {
  LoginReqBuilder builder_(_fbb);
  builder_.add_pw(pw);
  builder_.add_id(id);
  return builder_.Finish();
}

inline flatbuffers::Offset<LoginReq> CreateLoginReqDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *id = nullptr,
    const char *pw = nullptr) {
  auto id__ = id ? _fbb.CreateString(id) : 0;
  auto pw__ = pw ? _fbb.CreateString(pw) : 0;
  return account::CreateLoginReq(
      _fbb,
      id__,
      pw__);
}

struct LoginAck FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef LoginAckBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_RESULT = 4,
    VT_CHAR_COUNT = 6,
    VT_CHARACTERS = 8
  };
  uint16_t result() const {
    return GetField<uint16_t>(VT_RESULT, 0);
  }
  uint8_t char_count() const {
    return GetField<uint8_t>(VT_CHAR_COUNT, 0);
  }
  const flatbuffers::Vector<flatbuffers::Offset<CharacterInfo>> *characters() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<CharacterInfo>> *>(VT_CHARACTERS);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint16_t>(verifier, VT_RESULT, 2) &&
           VerifyField<uint8_t>(verifier, VT_CHAR_COUNT, 1) &&
           VerifyOffset(verifier, VT_CHARACTERS) &&
           verifier.VerifyVector(characters()) &&
           verifier.VerifyVectorOfTables(characters()) &&
           verifier.EndTable();
  }
};

struct LoginAckBuilder {
  typedef LoginAck Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_result(uint16_t result) {
    fbb_.AddElement<uint16_t>(LoginAck::VT_RESULT, result, 0);
  }
  void add_char_count(uint8_t char_count) {
    fbb_.AddElement<uint8_t>(LoginAck::VT_CHAR_COUNT, char_count, 0);
  }
  void add_characters(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<CharacterInfo>>> characters) {
    fbb_.AddOffset(LoginAck::VT_CHARACTERS, characters);
  }
  explicit LoginAckBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<LoginAck> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<LoginAck>(end);
    return o;
  }
};

inline flatbuffers::Offset<LoginAck> CreateLoginAck(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint16_t result = 0,
    uint8_t char_count = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<CharacterInfo>>> characters = 0) {
  LoginAckBuilder builder_(_fbb);
  builder_.add_characters(characters);
  builder_.add_result(result);
  builder_.add_char_count(char_count);
  return builder_.Finish();
}

inline flatbuffers::Offset<LoginAck> CreateLoginAckDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint16_t result = 0,
    uint8_t char_count = 0,
    const std::vector<flatbuffers::Offset<CharacterInfo>> *characters = nullptr) {
  auto characters__ = characters ? _fbb.CreateVector<flatbuffers::Offset<CharacterInfo>>(*characters) : 0;
  return account::CreateLoginAck(
      _fbb,
      result,
      char_count,
      characters__);
}

struct CreateAccountReq FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef CreateAccountReqBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_ID = 4,
    VT_PW = 6
  };
  const flatbuffers::String *id() const {
    return GetPointer<const flatbuffers::String *>(VT_ID);
  }
  const flatbuffers::String *pw() const {
    return GetPointer<const flatbuffers::String *>(VT_PW);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_ID) &&
           verifier.VerifyString(id()) &&
           VerifyOffset(verifier, VT_PW) &&
           verifier.VerifyString(pw()) &&
           verifier.EndTable();
  }
};

struct CreateAccountReqBuilder {
  typedef CreateAccountReq Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_id(flatbuffers::Offset<flatbuffers::String> id) {
    fbb_.AddOffset(CreateAccountReq::VT_ID, id);
  }
  void add_pw(flatbuffers::Offset<flatbuffers::String> pw) {
    fbb_.AddOffset(CreateAccountReq::VT_PW, pw);
  }
  explicit CreateAccountReqBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<CreateAccountReq> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<CreateAccountReq>(end);
    return o;
  }
};

inline flatbuffers::Offset<CreateAccountReq> CreateCreateAccountReq(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> id = 0,
    flatbuffers::Offset<flatbuffers::String> pw = 0) {
  CreateAccountReqBuilder builder_(_fbb);
  builder_.add_pw(pw);
  builder_.add_id(id);
  return builder_.Finish();
}

inline flatbuffers::Offset<CreateAccountReq> CreateCreateAccountReqDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *id = nullptr,
    const char *pw = nullptr) {
  auto id__ = id ? _fbb.CreateString(id) : 0;
  auto pw__ = pw ? _fbb.CreateString(pw) : 0;
  return account::CreateCreateAccountReq(
      _fbb,
      id__,
      pw__);
}

struct CreateAccountAck FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef CreateAccountAckBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_RESULT = 4,
    VT_CHAR_COUNT = 6,
    VT_CHARACTERS = 8
  };
  uint16_t result() const {
    return GetField<uint16_t>(VT_RESULT, 0);
  }
  uint8_t char_count() const {
    return GetField<uint8_t>(VT_CHAR_COUNT, 0);
  }
  const flatbuffers::Vector<flatbuffers::Offset<CharacterInfo>> *characters() const {
    return GetPointer<const flatbuffers::Vector<flatbuffers::Offset<CharacterInfo>> *>(VT_CHARACTERS);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint16_t>(verifier, VT_RESULT, 2) &&
           VerifyField<uint8_t>(verifier, VT_CHAR_COUNT, 1) &&
           VerifyOffset(verifier, VT_CHARACTERS) &&
           verifier.VerifyVector(characters()) &&
           verifier.VerifyVectorOfTables(characters()) &&
           verifier.EndTable();
  }
};

struct CreateAccountAckBuilder {
  typedef CreateAccountAck Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_result(uint16_t result) {
    fbb_.AddElement<uint16_t>(CreateAccountAck::VT_RESULT, result, 0);
  }
  void add_char_count(uint8_t char_count) {
    fbb_.AddElement<uint8_t>(CreateAccountAck::VT_CHAR_COUNT, char_count, 0);
  }
  void add_characters(flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<CharacterInfo>>> characters) {
    fbb_.AddOffset(CreateAccountAck::VT_CHARACTERS, characters);
  }
  explicit CreateAccountAckBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<CreateAccountAck> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<CreateAccountAck>(end);
    return o;
  }
};

inline flatbuffers::Offset<CreateAccountAck> CreateCreateAccountAck(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint16_t result = 0,
    uint8_t char_count = 0,
    flatbuffers::Offset<flatbuffers::Vector<flatbuffers::Offset<CharacterInfo>>> characters = 0) {
  CreateAccountAckBuilder builder_(_fbb);
  builder_.add_characters(characters);
  builder_.add_result(result);
  builder_.add_char_count(char_count);
  return builder_.Finish();
}

inline flatbuffers::Offset<CreateAccountAck> CreateCreateAccountAckDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint16_t result = 0,
    uint8_t char_count = 0,
    const std::vector<flatbuffers::Offset<CharacterInfo>> *characters = nullptr) {
  auto characters__ = characters ? _fbb.CreateVector<flatbuffers::Offset<CharacterInfo>>(*characters) : 0;
  return account::CreateCreateAccountAck(
      _fbb,
      result,
      char_count,
      characters__);
}

struct SelectCharacterReq FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef SelectCharacterReqBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_CHAR_ID = 4
  };
  uint64_t char_id() const {
    return GetField<uint64_t>(VT_CHAR_ID, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint64_t>(verifier, VT_CHAR_ID, 8) &&
           verifier.EndTable();
  }
};

struct SelectCharacterReqBuilder {
  typedef SelectCharacterReq Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_char_id(uint64_t char_id) {
    fbb_.AddElement<uint64_t>(SelectCharacterReq::VT_CHAR_ID, char_id, 0);
  }
  explicit SelectCharacterReqBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<SelectCharacterReq> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<SelectCharacterReq>(end);
    return o;
  }
};

inline flatbuffers::Offset<SelectCharacterReq> CreateSelectCharacterReq(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint64_t char_id = 0) {
  SelectCharacterReqBuilder builder_(_fbb);
  builder_.add_char_id(char_id);
  return builder_.Finish();
}

struct SelectCharacterAck FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef SelectCharacterAckBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_RESULT = 4
  };
  uint16_t result() const {
    return GetField<uint16_t>(VT_RESULT, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint16_t>(verifier, VT_RESULT, 2) &&
           verifier.EndTable();
  }
};

struct SelectCharacterAckBuilder {
  typedef SelectCharacterAck Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_result(uint16_t result) {
    fbb_.AddElement<uint16_t>(SelectCharacterAck::VT_RESULT, result, 0);
  }
  explicit SelectCharacterAckBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<SelectCharacterAck> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<SelectCharacterAck>(end);
    return o;
  }
};

inline flatbuffers::Offset<SelectCharacterAck> CreateSelectCharacterAck(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint16_t result = 0) {
  SelectCharacterAckBuilder builder_(_fbb);
  builder_.add_result(result);
  return builder_.Finish();
}

struct CreateCharacterReq FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef CreateCharacterReqBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_NICKNAME = 4,
    VT_CLASS_ = 6
  };
  const flatbuffers::String *nickname() const {
    return GetPointer<const flatbuffers::String *>(VT_NICKNAME);
  }
  uint8_t class_() const {
    return GetField<uint8_t>(VT_CLASS_, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_NICKNAME) &&
           verifier.VerifyString(nickname()) &&
           VerifyField<uint8_t>(verifier, VT_CLASS_, 1) &&
           verifier.EndTable();
  }
};

struct CreateCharacterReqBuilder {
  typedef CreateCharacterReq Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_nickname(flatbuffers::Offset<flatbuffers::String> nickname) {
    fbb_.AddOffset(CreateCharacterReq::VT_NICKNAME, nickname);
  }
  void add_class_(uint8_t class_) {
    fbb_.AddElement<uint8_t>(CreateCharacterReq::VT_CLASS_, class_, 0);
  }
  explicit CreateCharacterReqBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<CreateCharacterReq> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<CreateCharacterReq>(end);
    return o;
  }
};

inline flatbuffers::Offset<CreateCharacterReq> CreateCreateCharacterReq(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> nickname = 0,
    uint8_t class_ = 0) {
  CreateCharacterReqBuilder builder_(_fbb);
  builder_.add_nickname(nickname);
  builder_.add_class_(class_);
  return builder_.Finish();
}

inline flatbuffers::Offset<CreateCharacterReq> CreateCreateCharacterReqDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *nickname = nullptr,
    uint8_t class_ = 0) {
  auto nickname__ = nickname ? _fbb.CreateString(nickname) : 0;
  return account::CreateCreateCharacterReq(
      _fbb,
      nickname__,
      class_);
}

struct CreateCharacterAck FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef CreateCharacterAckBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_RESULT = 4,
    VT_CHAR_ID = 6
  };
  uint16_t result() const {
    return GetField<uint16_t>(VT_RESULT, 0);
  }
  uint64_t char_id() const {
    return GetField<uint64_t>(VT_CHAR_ID, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint16_t>(verifier, VT_RESULT, 2) &&
           VerifyField<uint64_t>(verifier, VT_CHAR_ID, 8) &&
           verifier.EndTable();
  }
};

struct CreateCharacterAckBuilder {
  typedef CreateCharacterAck Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_result(uint16_t result) {
    fbb_.AddElement<uint16_t>(CreateCharacterAck::VT_RESULT, result, 0);
  }
  void add_char_id(uint64_t char_id) {
    fbb_.AddElement<uint64_t>(CreateCharacterAck::VT_CHAR_ID, char_id, 0);
  }
  explicit CreateCharacterAckBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<CreateCharacterAck> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<CreateCharacterAck>(end);
    return o;
  }
};

inline flatbuffers::Offset<CreateCharacterAck> CreateCreateCharacterAck(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint16_t result = 0,
    uint64_t char_id = 0) {
  CreateCharacterAckBuilder builder_(_fbb);
  builder_.add_char_id(char_id);
  builder_.add_result(result);
  return builder_.Finish();
}

struct CheckCharacterNicknameReq FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef CheckCharacterNicknameReqBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_NICKNAME = 4
  };
  const flatbuffers::String *nickname() const {
    return GetPointer<const flatbuffers::String *>(VT_NICKNAME);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyOffset(verifier, VT_NICKNAME) &&
           verifier.VerifyString(nickname()) &&
           verifier.EndTable();
  }
};

struct CheckCharacterNicknameReqBuilder {
  typedef CheckCharacterNicknameReq Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_nickname(flatbuffers::Offset<flatbuffers::String> nickname) {
    fbb_.AddOffset(CheckCharacterNicknameReq::VT_NICKNAME, nickname);
  }
  explicit CheckCharacterNicknameReqBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<CheckCharacterNicknameReq> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<CheckCharacterNicknameReq>(end);
    return o;
  }
};

inline flatbuffers::Offset<CheckCharacterNicknameReq> CreateCheckCharacterNicknameReq(
    flatbuffers::FlatBufferBuilder &_fbb,
    flatbuffers::Offset<flatbuffers::String> nickname = 0) {
  CheckCharacterNicknameReqBuilder builder_(_fbb);
  builder_.add_nickname(nickname);
  return builder_.Finish();
}

inline flatbuffers::Offset<CheckCharacterNicknameReq> CreateCheckCharacterNicknameReqDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    const char *nickname = nullptr) {
  auto nickname__ = nickname ? _fbb.CreateString(nickname) : 0;
  return account::CreateCheckCharacterNicknameReq(
      _fbb,
      nickname__);
}

struct CheckCharacterNicknameAck FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef CheckCharacterNicknameAckBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_RESULT = 4
  };
  uint16_t result() const {
    return GetField<uint16_t>(VT_RESULT, 0);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint16_t>(verifier, VT_RESULT, 2) &&
           verifier.EndTable();
  }
};

struct CheckCharacterNicknameAckBuilder {
  typedef CheckCharacterNicknameAck Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_result(uint16_t result) {
    fbb_.AddElement<uint16_t>(CheckCharacterNicknameAck::VT_RESULT, result, 0);
  }
  explicit CheckCharacterNicknameAckBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<CheckCharacterNicknameAck> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<CheckCharacterNicknameAck>(end);
    return o;
  }
};

inline flatbuffers::Offset<CheckCharacterNicknameAck> CreateCheckCharacterNicknameAck(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint16_t result = 0) {
  CheckCharacterNicknameAckBuilder builder_(_fbb);
  builder_.add_result(result);
  return builder_.Finish();
}

}  // namespace account

#endif  // FLATBUFFERS_GENERATED_ACCOUNT_ACCOUNT_H_