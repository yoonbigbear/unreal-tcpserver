using System;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using UnityEngine;
using FlatBuffers;
using Cysharp.Threading.Tasks;

public class Network : MonoBehaviour
{
    static string host = Dns.GetHostName();
    static IPHostEntry ipHost = Dns.GetHostEntry(host);
    static IPAddress ipAddr = ipHost.AddressList[1]; // 0: ip6,  1: ip4
    static IPEndPoint endPoint = new IPEndPoint(ipAddr, 11000);
    static Socket socket = new Socket(endPoint.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
    static SocketAsyncEventArgs recvArgs = new SocketAsyncEventArgs();

    Dictionary<uint, GameObject> objs = new Dictionary<uint, GameObject>();

    DateTime start;
    DateTime now;

    // Start is called before the first frame update
    void Start()
    {
        Connector connector = new Connector();
        socket.Connect(endPoint);

        FlatBufferBuilder builder = new FlatBufferBuilder(1024);
        var id = builder.CreateString("sampleId2");
        var password = builder.CreateString("samplepassword2");
        account.LoginReq.StartLoginReq(builder);
        account.LoginReq.AddId(builder, id);
        account.LoginReq.AddPw(builder, password);
        var acc = account.LoginReq.EndLoginReq(builder);
        builder.Finish(acc.Value);
        var body = builder.SizedByteArray();

        //헤더 전송
        byte[] sendBuf = new byte[(sizeof(ushort) + sizeof(ushort))];
        Array.Copy(BitConverter.GetBytes((ushort)Protocol.LoginReq), 0, sendBuf, 0, sizeof(ushort));
        Array.Copy(BitConverter.GetBytes((ushort)body.Length), 0, sendBuf, sizeof(ushort), sizeof(ushort));
        socket.Send(sendBuf);

        //body 전송
        byte[] pkt_body = new byte[body.Length];
        Array.Copy(body, 0, pkt_body, 0, body.Length);
        socket.Send(pkt_body);

        start = DateTime.Now;
        now = DateTime.Now;
        Function();

        //        StartCoroutine(Packetreceive());
    }

    void PacketHandler(ByteBuffer recvBuf, Protocol id)
	{
        switch (id)
        {
            case Protocol.LoginAck:
                {
                    var wor = account.LoginAck.GetRootAsLoginAck(recvBuf);
                    for (int i = 0; i < wor.CharactersLength; ++i)
                    {
                        var datas = wor.Characters(i).Value;
                        Console.WriteLine($"{datas.CharId} {datas.Nickname} {datas.JobClass} {datas.CharId}");
                    }

                    FlatBufferBuilder builder = new FlatBufferBuilder(1024);
                    account.SelectCharacterReq.StartSelectCharacterReq(builder);
                    account.SelectCharacterReq.AddCharId(builder, 484280193545015295);
                    var b = account.SelectCharacterReq.EndSelectCharacterReq(builder);
                    builder.Finish(b.Value);
                    var body = builder.SizedByteArray();

                    //헤더 전송
                    byte[] sendBuf = new byte[(sizeof(ushort) + sizeof(ushort))];
                    Array.Copy(BitConverter.GetBytes((ushort)Protocol.SelectCharacterReq), 0, sendBuf, 0, sizeof(ushort));
                    Array.Copy(BitConverter.GetBytes((ushort)body.Length), 0, sendBuf, sizeof(ushort), sizeof(ushort));
                    socket.Send(sendBuf);

                    //body 전송
                    byte[] pkt_body = new byte[body.Length];
                    Array.Copy(body, 0, pkt_body, 0, body.Length);
                    socket.Send(pkt_body);
                }
                break;
            case Protocol.SelectCharacterAck:
                {
                    var wor = account.SelectCharacterAck.GetRootAsSelectCharacterAck(recvBuf);

                    Console.WriteLine($"{wor.Position.Value.X}  {wor.Position.Value.Y}  {wor.Position.Value.Z}");
                }
                break;
            case Protocol.MoveStartSync:
                {
                    var wor = world.MoveStartSync.GetRootAsMoveStartSync(recvBuf);
                    uint objid = wor.ObjId;
                    var dir = wor.Dir.Value;
                    var spd = wor.Speed;

                    objs[objid].spd = spd;
                    objs[objid].move = true;
                    objs[objid].d.x = dir.X;
                    objs[objid].d.y = dir.Y;

                }
                break;
            case Protocol.EnterFieldSync:
                {
                    var wor = world.EnterFieldSync.GetRootAsEnterFieldSync(recvBuf);
                    for (int i = 0; i < wor.ObjIdLength; ++i)
                    {
                        var objid = wor.ObjId(i);
                        var pos = wor.Pos(i);

                        GameObject obj = new GameObject();
                        obj.v.x = pos.Value.X;
                        obj.v.y = pos.Value.Y;
                        obj.v.z = pos.Value.Z;

                        Debug.Log($" 입장 {objid} {obj.v.x} {obj.v.y} {obj.v.z}");
                        objs.TryAdd(objid, obj);
                    }
                }
                break;
            case Protocol.MoveStopSync:
                {
                    var wor = world.MoveStopSync.GetRootAsMoveStopSync(recvBuf);
                    var objid = wor.ObjId;
                    var pos = wor.Pos;

                    objs[objid].d.x = 0;
                    objs[objid].d.y = 0;
                    objs[objid].move = false;
                    Debug.Log($" 예상 {objid} {objs[objid].v.x} {objs[objid].v.y} {objs[objid].v.z}");
                    Debug.Log($" 실제 {objid} {pos.Value.X} {pos.Value.Y} {pos.Value.Z}");
                    objs[objid].v.x = pos.Value.X;
                    objs[objid].v.y = pos.Value.Y;
                    objs[objid].v.z = pos.Value.Z;
                }
                break;
        }
    }
    // Update is called once per frame
    void Update()
    {

        now = DateTime.Now;
        var interval = now - start;
        start = now;
        float dt = (float)interval.TotalMilliseconds;

        foreach (var obj in objs)
        {
            var e = obj.Value;
            if (e.move)
            {
                Vector2 pre = new Vector2(e.v.x, e.v.z);
                pre = pre + (dt * e.spd * e.d);
                e.v.x = pre.x;
                e.v.z = pre.y;
            }
        }
    }

    public async UniTaskVoid Function()
    {
        byte[] recvbuf = new byte[4];
		await UniTask.Run(() => socket.Receive(recvbuf));
        var prot = BitConverter.ToUInt16(recvbuf, 0);
        var size = BitConverter.ToUInt16(recvbuf, 2);

        Protocol rceived_id = (Protocol)prot;
        Console.WriteLine($"{rceived_id.ToString()}");

        byte[] recv = new byte[size];
        await UniTask.Run(() => socket.Receive(recv));
        var body = new ByteBuffer(recv);
        PacketHandler(body, rceived_id);
        Function();
    }

}


