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
		static IPEndPoint endPoint = new IPEndPoint(ipAddr, 60000);
		static Socket socket = new Socket(endPoint.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
		static byte[] asyncbuf = new byte[1024];
		public static void re(object sender, SocketAsyncEventArgs arg)
		{
			if (arg.BytesTransferred > 0 && SocketError.Success == arg.SocketError)
			{
				var offset = 0;

				var id = BitConverter.ToUInt32(asyncbuf, offset);
				offset += 4;
				var size = BitConverter.ToUInt32(asyncbuf, offset);
				offset += 4;
				Protocol key = (Protocol)id;

				Console.WriteLine($"{key.ToString()}");

				if (key == Protocol.MoveStartSync)
				{
					{
						var recvBuf = new ByteBuffer(asyncbuf, offset);
						var wor = world.MoveStartSync.GetRootAsMoveStartSync(recvBuf);
						var objid = wor.ObjId;
						var dir = wor.Dir;
						var spd = wor.Speed;
					}
				}
				//asyncbuf = new byte[1024];
			}
			socket.ReceiveAsync(arg);
		}

		public static async Task TaskAsync(Socket socket, SocketAsyncEventArgs arg)
		{
			await Task.Run(() =>
			{
				byte[] buf = new byte[1024];

				arg.SetBuffer(buf, 0, 1024);
				while (true)
				{
					Thread.Sleep(250);
					try
					{
						var pending = socket.ReceiveAsync(arg);
						while (pending)
						{ Thread.Sleep(250); }

						
					}
					catch (Exception e)
					{
						Console.WriteLine($"ClientMain Exception {e}");
					}
				}
			});
		}
		static async Task Main(string[] args)
		{
			Console.WriteLine("Server Configuration");



			Console.ForegroundColor = ConsoleColor.White;
			Connector connector = new Connector();
			//ServerSession session = new ServerSession();

			//connector.Connect(session, endPoint, () => { return session = new ServerSession(); });

			SocketAsyncEventArgs socketarg = new SocketAsyncEventArgs();
			socketarg.Completed += new EventHandler<SocketAsyncEventArgs>(re);
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
				var buf = builder.SizedByteArray();

				int pkt_size = buf.Length + sizeof(int) + sizeof(int);
				byte[] sendBuf = new byte[pkt_size];
				Array.Copy(BitConverter.GetBytes((int)Protocol.LoginReq), 0, sendBuf, 0, sizeof(int));
				Array.Copy(BitConverter.GetBytes(pkt_size), 0, sendBuf, 4, sizeof(int));
				Array.Copy(buf, 0, sendBuf, 8, buf.Length);

				socket.Send(sendBuf);
				socketarg.SetBuffer(asyncbuf);
				socket.ReceiveAsync(socketarg);
				//{
				//	byte[] recvbuf = new byte[1024];
				//	socket.Receive(recvbuf);

				//	{
				//		var offset = 0;
				//		var prot = BitConverter.ToUInt32(recvbuf, offset);
				//		offset += 4;
				//		var size = BitConverter.ToUInt32(recvbuf, offset);
				//		offset += 4;

				//		Protocol rceived_id = (Protocol)prot;

				//		Console.WriteLine($"{rceived_id.ToString()}");
				//		{
				//			var recv = new ByteBuffer(recvbuf, offset);
				//			var wor = account.LoginAck.GetRootAsLoginAck(recv);
				//			for (int i = 0; i < wor.CharactersLength; ++i)
				//			{
				//				var datas = wor.Characters(i).Value;
				//				Console.WriteLine($"{datas.CharId} {datas.Nickname} {datas.JobClass} {datas.CharId}");
				//			}
				//		}
				//	}
				//}
			}
		

			// Create Chracter
			//{
			//	FlatBufferBuilder builder = new FlatBufferBuilder(1024);
			//	var id = builder.CreateString("sampleId23");
			//	account.CreateCharacterReq.StartCreateCharacterReq(builder);
			//	account.CreateCharacterReq.AddNickname(builder, id);
			//	account.CreateCharacterReq.AddJobClass(builder, 1);
			//	var b = account.CreateCharacterReq.EndCreateCharacterReq(builder);
			//	builder.Finish(b.Value);
			//	var buf = builder.SizedByteArray();

			//	int pkt_size = buf.Length + sizeof(int) + sizeof(int);
			//	byte[] sendBuf = new byte[pkt_size];
			//	Array.Copy(BitConverter.GetBytes((int)Protocol.CreateCharacterReq), 0, sendBuf, 0, sizeof(ushort));
			//	Array.Copy(BitConverter.GetBytes(pkt_size), 0, sendBuf, 4, sizeof(int));
			//	Array.Copy(buf, 0, sendBuf, 8, buf.Length);

			//	socket.Send(sendBuf);
			//	socket.Send(sendBuf);
			//}

			// select character
			{
				FlatBufferBuilder builder = new FlatBufferBuilder(1024);
				account.SelectCharacterReq.StartSelectCharacterReq(builder);
				account.SelectCharacterReq.AddCharId(builder, 484280193545015295);
				var b = account.SelectCharacterReq.EndSelectCharacterReq(builder);
				builder.Finish(b.Value);
				var buf = builder.SizedByteArray();

				int pkt_size = buf.Length + sizeof(int) + sizeof(int);
				byte[] sendBuf = new byte[pkt_size];
				Array.Copy(BitConverter.GetBytes((int)Protocol.SelectCharacterReq), 0, sendBuf, 0, sizeof(int));
				Array.Copy(BitConverter.GetBytes(pkt_size), 0, sendBuf, 4, sizeof(int));
				Array.Copy(buf, 0, sendBuf, 8, buf.Length);

			}

			while (true)
			{ }
			//await k;
		}
	}
}
