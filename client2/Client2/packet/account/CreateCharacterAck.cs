// <auto-generated>
//  automatically generated by the FlatBuffers compiler, do not modify
// </auto-generated>

namespace account
{

using global::System;
using global::System.Collections.Generic;
using global::FlatBuffers;

public struct CreateCharacterAck : IFlatbufferObject
{
  private Table __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public static void ValidateVersion() { FlatBufferConstants.FLATBUFFERS_1_12_0(); }
  public static CreateCharacterAck GetRootAsCreateCharacterAck(ByteBuffer _bb) { return GetRootAsCreateCharacterAck(_bb, new CreateCharacterAck()); }
  public static CreateCharacterAck GetRootAsCreateCharacterAck(ByteBuffer _bb, CreateCharacterAck obj) { return (obj.__assign(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __p = new Table(_i, _bb); }
  public CreateCharacterAck __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public ushort Result { get { int o = __p.__offset(4); return o != 0 ? __p.bb.GetUshort(o + __p.bb_pos) : (ushort)0; } }

  public static Offset<account.CreateCharacterAck> CreateCreateCharacterAck(FlatBufferBuilder builder,
      ushort result = 0) {
    builder.StartTable(1);
    CreateCharacterAck.AddResult(builder, result);
    return CreateCharacterAck.EndCreateCharacterAck(builder);
  }

  public static void StartCreateCharacterAck(FlatBufferBuilder builder) { builder.StartTable(1); }
  public static void AddResult(FlatBufferBuilder builder, ushort result) { builder.AddUshort(0, result, 0); }
  public static Offset<account.CreateCharacterAck> EndCreateCharacterAck(FlatBufferBuilder builder) {
    int o = builder.EndTable();
    return new Offset<account.CreateCharacterAck>(o);
  }
};


}
