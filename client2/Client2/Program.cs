﻿using FlatBuffers;
using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Threading.Tasks;

namespace Client2
{
	public struct vector2
	{
		public float x;
		public float y;
	}
	public struct vector3
	{
		public float x;
		public float y;
		public float z;	
	}
	public struct GameObject
	{
		public vector3 v;
		public float spd;
		public vector2 d;
		public bool move;
	}

	class Program
	{
		static string host = Dns.GetHostName();
		static IPHostEntry ipHost = Dns.GetHostEntry(host);
		static IPAddress ipAddr = ipHost.AddressList[1]; // 0: ip6,  1: ip4
		static IPEndPoint endPoint = new IPEndPoint(ipAddr, 11000);
		static Socket socket = new Socket(endPoint.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
		static void Main(string[] args)
		{
			Console.WriteLine("Server Configuration");

			Console.ForegroundColor = ConsoleColor.White;
			Connector connector = new Connector();

			socket.Connect(endPoint);
			//Create Account

			//Task k = TaskAsync(socket, socketarg);
			{
				//	FlatBufferBuilder builder = new FlatBufferBuilder(1024);
				//	var id = builder.CreateString("sampleId2");
				//	var password = builder.CreateString("samplepassword2");
				//	account.CreateAccountReq.StartCreateAccountReq(builder);
				//	account.CreateAccountReq.AddId(builder, id);
				//	account.CreateAccountReq.AddPw(builder, password);
				//	var acc = account.CreateAccountReq.EndCreateAccountReq(builder);
				//	builder.Finish(acc.Value);
				//	var buf = builder.SizedByteArray();

				//	int pkt_size = buf.Length + sizeof(int) + sizeof(int);
				//	byte[] sendBuf = new byte[pkt_size];
				//	Array.Copy(BitConverter.GetBytes((int)Protocol.CreateAccountReq), 0, sendBuf, 0, sizeof(ushort));
				//	Array.Copy(BitConverter.GetBytes(pkt_size), 0, sendBuf, 4, sizeof(int));
				//	Array.Copy(buf, 0, sendBuf, 8, buf.Length);

				//	session.Send(new ArraySegment<byte>(sendBuf));
			}

			//LoginAccount
			{
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
			}

			{
				//헤더 수신
				byte[] recvbuf = new byte[4];
				socket.Receive(recvbuf);
				var prot = BitConverter.ToUInt16(recvbuf, 0);
				var size = BitConverter.ToUInt16(recvbuf, 2);

				Protocol rceived_id = (Protocol)prot;
				Console.WriteLine($"{rceived_id.ToString()}");


				//바디 수신
				byte[] recv = new byte[size];
				socket.Receive(recv);
				var bb = new ByteBuffer(recv);

				var wor = account.LoginAck.GetRootAsLoginAck(bb);
				for (int i = 0; i < wor.CharactersLength; ++i)
				{
					var datas = wor.Characters(i).Value;
					Console.WriteLine($"{datas.CharId} {datas.Nickname} {datas.JobClass} {datas.CharId}");
				}

			}


			//// Create Chracter
			////{
			////	FlatBufferBuilder builder = new FlatBufferBuilder(1024);
			////	var id = builder.CreateString("sampleId23");
			////	account.CreateCharacterReq.StartCreateCharacterReq(builder);
			////	account.CreateCharacterReq.AddNickname(builder, id);
			////	account.CreateCharacterReq.AddJobClass(builder, 1);
			////	var b = account.CreateCharacterReq.EndCreateCharacterReq(builder);
			////	builder.Finish(b.Value);
			////	var buf = builder.SizedByteArray();

			////	int pkt_size = buf.Length + sizeof(int) + sizeof(int);
			////	byte[] sendBuf = new byte[pkt_size];
			////	Array.Copy(BitConverter.GetBytes((int)Protocol.CreateCharacterReq), 0, sendBuf, 0, sizeof(ushort));
			////	Array.Copy(BitConverter.GetBytes(pkt_size), 0, sendBuf, 4, sizeof(int));
			////	Array.Copy(buf, 0, sendBuf, 8, buf.Length);

			////	socket.Send(sendBuf);
			////	socket.Send(sendBuf);
			////}

			// select character
			{

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

				{
					//헤더 수신
					byte[] recvbuf = new byte[4];
					socket.Receive(recvbuf);
					var prot = BitConverter.ToUInt16(recvbuf, 0);
					var size = BitConverter.ToUInt16(recvbuf, 2);
					Protocol rceived_id = (Protocol)prot;
					Console.WriteLine($"{rceived_id.ToString()}");

					//바디 수신
					byte[] packet_body = new byte[size];
					socket.Receive(packet_body);
					var recv = new ByteBuffer(packet_body);

					var wor = account.SelectCharacterAck.GetRootAsSelectCharacterAck(recv);

					Console.WriteLine($"{wor.Position.Value.X}  {wor.Position.Value.Y}  {wor.Position.Value.Z}");
				}
			}



			Dictionary<uint, GameObject> objs = new Dictionary<uint, GameObject>();

			var start = DateTime.Now;
			var now = DateTime.Now;
			while (true)
			{
				now = DateTime.Now;
				var interval = now - start;
				start = now;
				float dt = (float)interval.TotalMilliseconds;

				byte[] buf = new byte[4];
				socket.Receive(buf);
				{
					var id = BitConverter.ToUInt16(buf, 0);
					var size = BitConverter.ToUInt16(buf, 2);
					byte[] packet_body = new byte[size];
					socket.Receive(packet_body);
					var recvBuf = new ByteBuffer(packet_body);

					switch ((Protocol)id)
					{
						case Protocol.MoveStartSync:
							{
								var wor = world.MoveStartSync.GetRootAsMoveStartSync(recvBuf);
								uint objid = wor.ObjId;
								var dir = wor.Dir.Value;
								var spd = wor.Speed;
								//Console.WriteLine($"{objid}  {dir.Value.X}  {dir.Value.Y} {spd}");
								var obj = objs[objid];
								obj.spd = spd;
								obj.move = true;

								obj.d.x = dir.X;
								obj.d.y = dir.Y;
							}
							break;
						case Protocol.EnterFieldSync:
							{
								var wor = world.EnterFieldSync.GetRootAsEnterFieldSync(recvBuf);
								for (int i = 0; i < wor.ObjIdLength; ++i)
								{
									var objid = wor.ObjId(i);
									var pos = wor.Pos(i);
									Console.WriteLine($" 입장 {objid} {pos.Value.X} {pos.Value.Y} {pos.Value.Z}");

									GameObject obj = new GameObject();
									obj.v.x = pos.Value.X;
									obj.v.y = pos.Value.Y;
									obj.v.z = pos.Value.Z;

									objs.TryAdd(objid, obj);
								}
							}
							break;
						case Protocol.MoveStopSync:
							{
								var wor = world.MoveStopSync.GetRootAsMoveStopSync(recvBuf);
								var objid = wor.ObjId;
								var pos = wor.Pos;
								var obj = objs[objid];

								Console.WriteLine($" 도착 {objid} {pos.Value.X} {pos.Value.Y} {pos.Value.Z}");

								obj.d.x = 0;
								obj.d.y = 0;
								obj.move = false;
							}
							break;
					}
;
				}

				foreach(var obj in objs)
				{
					var e = obj.Value;
					if (e.move)
					{
						e.v.x = e.v.x + (dt * e.spd * e.d.x);
						e.v.y = e.v.y + (dt * e.spd * e.d.y);
					}
				}
			}
		}
	}
}
