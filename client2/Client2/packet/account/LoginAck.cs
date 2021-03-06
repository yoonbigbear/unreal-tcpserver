// <auto-generated>
//  automatically generated by the FlatBuffers compiler, do not modify
// </auto-generated>

namespace account
{

using global::System;
using global::System.Collections.Generic;
using global::FlatBuffers;

public struct LoginAck : IFlatbufferObject
{
  private Table __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public static void ValidateVersion() { FlatBufferConstants.FLATBUFFERS_1_12_0(); }
  public static LoginAck GetRootAsLoginAck(ByteBuffer _bb) { return GetRootAsLoginAck(_bb, new LoginAck()); }
  public static LoginAck GetRootAsLoginAck(ByteBuffer _bb, LoginAck obj) { return (obj.__assign(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __p = new Table(_i, _bb); }
  public LoginAck __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public ushort Result { get { int o = __p.__offset(4); return o != 0 ? __p.bb.GetUshort(o + __p.bb_pos) : (ushort)0; } }
  public CharacterInfo? Characters(int j) { int o = __p.__offset(6); return o != 0 ? (CharacterInfo?)(new CharacterInfo()).__assign(__p.__indirect(__p.__vector(o) + j * 4), __p.bb) : null; }
  public int CharactersLength { get { int o = __p.__offset(6); return o != 0 ? __p.__vector_len(o) : 0; } }

  public static Offset<account.LoginAck> CreateLoginAck(FlatBufferBuilder builder,
      ushort result = 0,
      VectorOffset charactersOffset = default(VectorOffset)) {
    builder.StartTable(2);
    LoginAck.AddCharacters(builder, charactersOffset);
    LoginAck.AddResult(builder, result);
    return LoginAck.EndLoginAck(builder);
  }

  public static void StartLoginAck(FlatBufferBuilder builder) { builder.StartTable(2); }
  public static void AddResult(FlatBufferBuilder builder, ushort result) { builder.AddUshort(0, result, 0); }
  public static void AddCharacters(FlatBufferBuilder builder, VectorOffset charactersOffset) { builder.AddOffset(1, charactersOffset.Value, 0); }
  public static VectorOffset CreateCharactersVector(FlatBufferBuilder builder, Offset<CharacterInfo>[] data) { builder.StartVector(4, data.Length, 4); for (int i = data.Length - 1; i >= 0; i--) builder.AddOffset(data[i].Value); return builder.EndVector(); }
  public static VectorOffset CreateCharactersVectorBlock(FlatBufferBuilder builder, Offset<CharacterInfo>[] data) { builder.StartVector(4, data.Length, 4); builder.Add(data); return builder.EndVector(); }
  public static void StartCharactersVector(FlatBufferBuilder builder, int numElems) { builder.StartVector(4, numElems, 4); }
  public static Offset<account.LoginAck> EndLoginAck(FlatBufferBuilder builder) {
    int o = builder.EndTable();
    return new Offset<account.LoginAck>(o);
  }
};


}
