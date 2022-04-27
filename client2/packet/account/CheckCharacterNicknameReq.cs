// <auto-generated>
//  automatically generated by the FlatBuffers compiler, do not modify
// </auto-generated>

namespace account
{

using global::System;
using global::System.Collections.Generic;
using global::FlatBuffers;

public struct CheckCharacterNicknameReq : IFlatbufferObject
{
  private Table __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public static void ValidateVersion() { FlatBufferConstants.FLATBUFFERS_2_0_0(); }
  public static CheckCharacterNicknameReq GetRootAsCheckCharacterNicknameReq(ByteBuffer _bb) { return GetRootAsCheckCharacterNicknameReq(_bb, new CheckCharacterNicknameReq()); }
  public static CheckCharacterNicknameReq GetRootAsCheckCharacterNicknameReq(ByteBuffer _bb, CheckCharacterNicknameReq obj) { return (obj.__assign(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __p = new Table(_i, _bb); }
  public CheckCharacterNicknameReq __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public string Nickname { get { int o = __p.__offset(4); return o != 0 ? __p.__string(o + __p.bb_pos) : null; } }
#if ENABLE_SPAN_T
  public Span<byte> GetNicknameBytes() { return __p.__vector_as_span<byte>(4, 1); }
#else
  public ArraySegment<byte>? GetNicknameBytes() { return __p.__vector_as_arraysegment(4); }
#endif
  public byte[] GetNicknameArray() { return __p.__vector_as_array<byte>(4); }

  public static Offset<account.CheckCharacterNicknameReq> CreateCheckCharacterNicknameReq(FlatBufferBuilder builder,
      StringOffset nicknameOffset = default(StringOffset)) {
    builder.StartTable(1);
    CheckCharacterNicknameReq.AddNickname(builder, nicknameOffset);
    return CheckCharacterNicknameReq.EndCheckCharacterNicknameReq(builder);
  }

  public static void StartCheckCharacterNicknameReq(FlatBufferBuilder builder) { builder.StartTable(1); }
  public static void AddNickname(FlatBufferBuilder builder, StringOffset nicknameOffset) { builder.AddOffset(0, nicknameOffset.Value, 0); }
  public static Offset<account.CheckCharacterNicknameReq> EndCheckCharacterNicknameReq(FlatBufferBuilder builder) {
    int o = builder.EndTable();
    return new Offset<account.CheckCharacterNicknameReq>(o);
  }
}


}
