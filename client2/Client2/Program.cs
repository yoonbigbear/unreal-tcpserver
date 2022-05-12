using FlatBuffers;
using System;
using System.Net;
using System.Net.Sockets;
using System.Threading;

public struct header
{
	int _protocol = 0;
	int _size = 0;

    public header(Protocol protocol, int bodysize)
    {
		_protocol = (int)protocol;
		_size = sizeof(int) + sizeof(int) + bodysize;
    }
}

public class ayc : IAsyncResult
{
	public object AsyncState => AsyncState;

	public WaitHandle AsyncWaitHandle => AsyncWaitHandle;

	public bool CompletedSynchronously => true;

	public bool IsCompleted => true;
}

namespace Client2
{
	class Program
	{
		static void Main(string[] args)
		{
			Console.ForegroundColor = ConsoleColor.Green;
			Console.WriteLine("Server Configuration");

			string host = Dns.GetHostName();
			Console.WriteLine($"host : {host}");
			IPHostEntry ipHost = Dns.GetHostEntry(host);
			IPAddress ipAddr = ipHost.AddressList[1]; // 0: ip6,  1: ip4
			IPEndPoint endPoint = new IPEndPoint(ipAddr, 60000);
			Console.WriteLine($"end point : {endPoint.Address}:{endPoint.Port}");

			Console.ForegroundColor = ConsoleColor.White;
			Connector connector = new Connector();
			//ServerSession session = new ServerSession();

			//connector.Connect(session, endPoint, () => { return session = new ServerSession(); });

			var socket = new Socket(endPoint.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
			socket.Connect(endPoint);
			//Create Account
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
				Array.Copy(BitConverter.GetBytes((int)Protocol.LoginReq), 0, sendBuf, 0, sizeof(ushort));
				Array.Copy(BitConverter.GetBytes(pkt_size), 0, sendBuf, 4, sizeof(int));
				Array.Copy(buf, 0, sendBuf, 8, buf.Length);

				ayc aa = new ayc();
				socket.Send(sendBuf);
				socket.EndSend(aa);
			}

            {
                byte[] buf = new byte[1024];
				socket.Receive(buf);
                //if (_recvArgs.BytesTransferred > 0)
                {
                    Console.WriteLine("Packet Received");
                }
            }


            while (true)
			{
				try
				{

				}
				catch (Exception e)
				{
					Console.WriteLine($"ClientMain Exception {e}");
				}

			}

		}
	}
}
