// <auto-generated>
//  automatically generated by the FlatBuffers compiler, do not modify
// </auto-generated>

namespace world
{

using global::System;
using global::System.Collections.Generic;
using global::FlatBuffers;

public struct EnterFieldSync : IFlatbufferObject
{
  private Table __p;
  public ByteBuffer ByteBuffer { get { return __p.bb; } }
  public static void ValidateVersion() { FlatBufferConstants.FLATBUFFERS_1_12_0(); }
  public static EnterFieldSync GetRootAsEnterFieldSync(ByteBuffer _bb) { return GetRootAsEnterFieldSync(_bb, new EnterFieldSync()); }
  public static EnterFieldSync GetRootAsEnterFieldSync(ByteBuffer _bb, EnterFieldSync obj) { return (obj.__assign(_bb.GetInt(_bb.Position) + _bb.Position, _bb)); }
  public void __init(int _i, ByteBuffer _bb) { __p = new Table(_i, _bb); }
  public EnterFieldSync __assign(int _i, ByteBuffer _bb) { __init(_i, _bb); return this; }

  public uint ObjId(int j) { int o = __p.__offset(4); return o != 0 ? __p.bb.GetUint(__p.__vector(o) + j * 4) : (uint)0; }
  public int ObjIdLength { get { int o = __p.__offset(4); return o != 0 ? __p.__vector_len(o) : 0; } }
#if ENABLE_SPAN_T
  public Span<uint> GetObjIdBytes() { return __p.__vector_as_span<uint>(4, 4); }
#else
  public ArraySegment<byte>? GetObjIdBytes() { return __p.__vector_as_arraysegment(4); }
#endif
  public uint[] GetObjIdArray() { return __p.__vector_as_array<uint>(4); }
  public Vec3? Pos(int j) { int o = __p.__offset(6); return o != 0 ? (Vec3?)(new Vec3()).__assign(__p.__vector(o) + j * 12, __p.bb) : null; }
  public int PosLength { get { int o = __p.__offset(6); return o != 0 ? __p.__vector_len(o) : 0; } }

  public static Offset<world.EnterFieldSync> CreateEnterFieldSync(FlatBufferBuilder builder,
      VectorOffset obj_idOffset = default(VectorOffset),
      VectorOffset posOffset = default(VectorOffset)) {
    builder.StartTable(2);
    EnterFieldSync.AddPos(builder, posOffset);
    EnterFieldSync.AddObjId(builder, obj_idOffset);
    return EnterFieldSync.EndEnterFieldSync(builder);
  }

  public static void StartEnterFieldSync(FlatBufferBuilder builder) { builder.StartTable(2); }
  public static void AddObjId(FlatBufferBuilder builder, VectorOffset objIdOffset) { builder.AddOffset(0, objIdOffset.Value, 0); }
  public static VectorOffset CreateObjIdVector(FlatBufferBuilder builder, uint[] data) { builder.StartVector(4, data.Length, 4); for (int i = data.Length - 1; i >= 0; i--) builder.AddUint(data[i]); return builder.EndVector(); }
  public static VectorOffset CreateObjIdVectorBlock(FlatBufferBuilder builder, uint[] data) { builder.StartVector(4, data.Length, 4); builder.Add(data); return builder.EndVector(); }
  public static void StartObjIdVector(FlatBufferBuilder builder, int numElems) { builder.StartVector(4, numElems, 4); }
  public static void AddPos(FlatBufferBuilder builder, VectorOffset posOffset) { builder.AddOffset(1, posOffset.Value, 0); }
  public static void StartPosVector(FlatBufferBuilder builder, int numElems) { builder.StartVector(12, numElems, 4); }
  public static Offset<world.EnterFieldSync> EndEnterFieldSync(FlatBufferBuilder builder) {
    int o = builder.EndTable();
    return new Offset<world.EnterFieldSync>(o);
  }
};


}
