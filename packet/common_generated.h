// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_COMMON_H_
#define FLATBUFFERS_GENERATED_COMMON_H_

#include "flatbuffers/flatbuffers.h"

struct CharacterInfo;
struct CharacterInfoBuilder;

struct CharacterInfo FLATBUFFERS_FINAL_CLASS : private flatbuffers::Table {
  typedef CharacterInfoBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_CLASS_ = 4,
    VT_NICKNAME = 6
  };
  uint8_t class_() const {
    return GetField<uint8_t>(VT_CLASS_, 0);
  }
  const flatbuffers::String *nickname() const {
    return GetPointer<const flatbuffers::String *>(VT_NICKNAME);
  }
  bool Verify(flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint8_t>(verifier, VT_CLASS_, 1) &&
           VerifyOffset(verifier, VT_NICKNAME) &&
           verifier.VerifyString(nickname()) &&
           verifier.EndTable();
  }
};

struct CharacterInfoBuilder {
  typedef CharacterInfo Table;
  flatbuffers::FlatBufferBuilder &fbb_;
  flatbuffers::uoffset_t start_;
  void add_class_(uint8_t class_) {
    fbb_.AddElement<uint8_t>(CharacterInfo::VT_CLASS_, class_, 0);
  }
  void add_nickname(flatbuffers::Offset<flatbuffers::String> nickname) {
    fbb_.AddOffset(CharacterInfo::VT_NICKNAME, nickname);
  }
  explicit CharacterInfoBuilder(flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  flatbuffers::Offset<CharacterInfo> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = flatbuffers::Offset<CharacterInfo>(end);
    return o;
  }
};

inline flatbuffers::Offset<CharacterInfo> CreateCharacterInfo(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint8_t class_ = 0,
    flatbuffers::Offset<flatbuffers::String> nickname = 0) {
  CharacterInfoBuilder builder_(_fbb);
  builder_.add_nickname(nickname);
  builder_.add_class_(class_);
  return builder_.Finish();
}

inline flatbuffers::Offset<CharacterInfo> CreateCharacterInfoDirect(
    flatbuffers::FlatBufferBuilder &_fbb,
    uint8_t class_ = 0,
    const char *nickname = nullptr) {
  auto nickname__ = nickname ? _fbb.CreateString(nickname) : 0;
  return CreateCharacterInfo(
      _fbb,
      class_,
      nickname__);
}

#endif  // FLATBUFFERS_GENERATED_COMMON_H_
