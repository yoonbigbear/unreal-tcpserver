using FlatBuffers;
using System;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using System.Threading;
using System.Threading.Tasks;

namespace Client2
{

	class Program
	{
		static string host = Dns.GetHostName();
		static IPHostEntry ipHost = Dns.GetHostEntry(host);
		static IPAddress ipAddr = ipHost.AddressList[1]; // 0: ip6,  1: ip4
		static IPEndPoint endPoint = new IPEndPoint(ipAddr, 11000);
		static Socket socket = new Socket(endPoint.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
		static async Task Main(string[] args)
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

				byte[] header = new byte[8];
				Array.Copy(BitConverter.GetBytes((int)Protocol.LoginReq), 0, header, 0, sizeof(int));
				Array.Copy(BitConverter.GetBytes(body.Length), 0, header, 4, sizeof(int));

				socket.Send(header);
				socket.Send(body);
			}

			{
				byte[] recvbuf = new byte[1024];
				socket.Receive(recvbuf);
				{
					var offset = 0;
					var prot = BitConverter.ToUInt32(recvbuf, offset);
					offset += 4;
					var size = BitConverter.ToUInt32(recvbuf, offset);
					offset += 4;
					var body_buf = new byte[size];
					socket.Receive(body_buf);

					Protocol rceived_id = (Protocol)prot;
					Console.WriteLine($"{rceived_id.ToString()}");
					{

						var recv = new ByteBuffer(body_buf, 0);
						var wor = account.LoginAck.GetRootAsLoginAck(recv);
						for (int i = 0; i < wor.CharactersLength; ++i)
						{
							var datas = wor.Characters(i).Value;
							Console.WriteLine($"{datas.CharId} {datas.Nickname} {datas.JobClass} {datas.CharId}");
						}
					}
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

				byte[] header = new byte[8];
				Array.Copy(BitConverter.GetBytes((int)Protocol.SelectCharacterReq), 0, header, 0, sizeof(int));
				Array.Copy(BitConverter.GetBytes(body.Length), 0, header, 4, sizeof(int));

				socket.Send(header);
				socket.Send(body);

				{
					byte[] recvbuf = new byte[1024];
					socket.Receive(recvbuf);
					{
						var offset = 0;
						var prot = BitConverter.ToUInt32(recvbuf, offset);
						offset += 4;
						var size = BitConverter.ToUInt32(recvbuf, offset);
						offset += 4;
						var body_buf = new byte[size];
						socket.Receive(body_buf);

						Protocol rceived_id = (Protocol)prot;
						Console.WriteLine($"{rceived_id.ToString()}");
						{

							var recv = new ByteBuffer(body_buf, 0);
							var wor = account.SelectCharacterAck.GetRootAsSelectCharacterAck(recv);
							
							Console.WriteLine($"{wor.Position.Value.X}  {wor.Position.Value.Y}  {wor.Position.Value.Z}");
						}
					}
				}
			}

			while (true)
			{
				byte[] buf = new byte[1024];
				socket.Receive(buf);
				{
					var id = BitConverter.ToUInt32(buf, 0);
					var size = BitConverter.ToUInt32(buf, 4);
					switch ((Protocol)id)
					{
						case Protocol.MoveStartSync:
							{
								var body_buf = new byte[size];
								socket.Receive(body_buf);

								var recvBuf = new ByteBuffer(body_buf, 0);
								var wor = world.MoveStartSync.GetRootAsMoveStartSync(recvBuf);
								var objid = wor.ObjId;
								var dir = wor.Dir;
								var spd = wor.Speed;
								Console.WriteLine($"{objid}  {dir.Value.X}  {dir.Value.Y} {spd}");
							}
							break;
						case Protocol.EnterFieldSync:
							{
								var body_buf = new byte[size];
								socket.Receive(body_buf);

								var recvBuf = new ByteBuffer(body_buf, 0);
								var wor = world.EnterFieldSync.GetRootAsEnterFieldSync(recvBuf);
								for (int i = 0; i < wor.ObjIdLength; ++i)
								{
									var objid = wor.ObjId(i);
									var pos = wor.Pos(i);
									Console.WriteLine($"{objid} {pos.Value.X} {pos.Value.Y} {pos.Value.Z}");
								}
							}
							break;
						case Protocol.MoveStopSync:
							{
								var body_buf = new byte[size];
								socket.Receive(body_buf);

								var recvBuf = new ByteBuffer(body_buf, 0);
								var wor = world.MoveStopSync.GetRootAsMoveStopSync(recvBuf);
								var objid = wor.ObjId;
								Console.WriteLine($"{objid}");
							}
							break;
					}
;
				}
			}
			//await k;
		}
	}
}
