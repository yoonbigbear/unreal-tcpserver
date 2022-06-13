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

    public GameObject field_object_prefab;

    Dictionary<uint, GameObject> objs = new Dictionary<uint, GameObject>();

    DateTime start;
    DateTime now;

    // Start is called before the first frame update
    void Start()
    {

        Connector connector = new Connector();
        socket.Connect(endPoint);

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

                    var fieldobj = objs[objid].GetComponent<FieldObject>();
                    fieldobj.spd = spd;
                    fieldobj.d = new Vector2(dir.X, dir.Y);
                    fieldobj.move = true;

                }
                break;
            case Protocol.EnterFieldSync:
                {
                    var wor = world.EnterFieldSync.GetRootAsEnterFieldSync(recvBuf);
                    for (int i = 0; i < wor.ObjIdLength; ++i)
                    {
                        var objid = wor.ObjId(i);
                        var pos = wor.Pos(i);

                        var obj = Instantiate(field_object_prefab);

                        obj.gameObject.transform.position = new Vector3(
                            pos.Value.X, pos.Value.Y, pos.Value.Z);

                        Debug.Log($" 입장 {objid} {obj.transform.position.x} " +
							$"{obj.transform.position.y} {obj.transform.position.z}");

                        objs.TryAdd(objid, obj);
                    }
                }
                break;
            case Protocol.MoveStopSync:
                {
                    var wor = world.MoveStopSync.GetRootAsMoveStopSync(recvBuf);
                    var objid = wor.ObjId;
                    var pos = wor.Pos;

                    var fieldobj = objs[objid].GetComponent<FieldObject>();
                    fieldobj.d = new Vector2(0,0);
                    fieldobj.move = false;

                    //Debug.Log($" 예상 {objid} {objs[objid].v.x} {objs[objid].v.y} {objs[objid].v.z}");
                    //Debug.Log($" 실제 {objid} {pos.Value.X} {pos.Value.Y} {pos.Value.Z}");
                    objs[objid].transform.position = new Vector3(
                            pos.Value.X, pos.Value.Y, pos.Value.Z);
                }
                break;
            case Protocol.CreateAccountAck:
                {
                    var wor = account.CreateAccountAck.GetRootAsCreateAccountAck(recvBuf);
                    var result = wor.Result;
                    
                    if ((ResultCode)result == ResultCode.CreateSuccess)
					{
                        //씬 이동
					}
                }
                break;
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

    public void Login(string _id, string _password)
    {
        FlatBufferBuilder builder = new FlatBufferBuilder(1024);
        var id = builder.CreateString(_id);
        var password = builder.CreateString(_password);
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
    }

    public void CreateAccount(string _id, string _password)
	{
        FlatBufferBuilder builder = new FlatBufferBuilder(1024);
        var id = builder.CreateString(_id);
        var password = builder.CreateString(_password);
        account.CreateAccountReq.StartCreateAccountReq(builder);
        account.CreateAccountReq.AddId(builder, id);
        account.CreateAccountReq.AddPw(builder, password);
        var acc = account.CreateAccountReq.EndCreateAccountReq(builder);
        builder.Finish(acc.Value);
        var body = builder.SizedByteArray();

        //헤더 전송
        byte[] sendBuf = new byte[(sizeof(ushort) + sizeof(ushort))];
        Array.Copy(BitConverter.GetBytes((ushort)Protocol.CreateAccountReq), 0, sendBuf, 0, sizeof(ushort));
        Array.Copy(BitConverter.GetBytes((ushort)body.Length), 0, sendBuf, sizeof(ushort), sizeof(ushort));
        socket.Send(sendBuf);

        //body 전송
        byte[] pkt_body = new byte[body.Length];
        Array.Copy(body, 0, pkt_body, 0, body.Length);
        socket.Send(pkt_body);
    }
}


