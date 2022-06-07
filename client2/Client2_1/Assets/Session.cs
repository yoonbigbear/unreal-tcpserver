using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

public class ServerSession : PacketSession
{
	public override void OnConnected(EndPoint _endpoint)
	{
		Console.WriteLine($"Onconnected {_endpoint}");
	}

	public override void OnDisconnected(EndPoint _endPoint)
	{
		Console.WriteLine($"OnDisconnected{_endPoint}");
	}

	public override void OnRecvPacket(ArraySegment<byte> _buf)
	{
		Console.WriteLine($"Recv Packet");
		//PacketManager.Instance.OnRecvPacket(this, _buf);
	}

	public override void OnSend(int _numOfBytes)
	{
		//Console.WriteLine($"Transferred bytes = {_numOfBytes}");
	}
}

public abstract class PacketSession : Session
{
	public static readonly short HeaderSize = 8;

	public sealed override int OnRecv(ArraySegment<byte> buf)
	{
		int processLen = 0;
		int packetCount = 0;

		while (true)
		{
			if (buf.Count < HeaderSize)
				break;

			var header = BitConverter.ToInt32(buf.Array, buf.Offset);
			if (buf.Count < header)
				break;

			var datasize = BitConverter.ToInt32(buf.Array, buf.Offset);
			if (buf.Count < header)
				break;

			OnRecvPacket(new ArraySegment<byte>(buf.Array, buf.Offset, datasize));
			packetCount++;

			//모든 데이터를 받은 상태
			processLen += datasize;
			buf = new ArraySegment<byte>(buf.Array, buf.Offset + datasize, buf.Count - datasize);
		}

		if (packetCount > 1)
			Console.WriteLine($"Packet Count = {packetCount}");
		return processLen;
	}
	public abstract void OnRecvPacket(ArraySegment<byte> buf);
}

public abstract class Session
{
	int _escLock = 0;
	object _lock = new object();

	public Socket _socket;
	RecvBuffer _recvBuffer = new RecvBuffer(65535);

	SocketAsyncEventArgs _sendArgs = new SocketAsyncEventArgs();
	SocketAsyncEventArgs _recvArgs = new SocketAsyncEventArgs();

	List<ArraySegment<byte>> sendingList = new List<ArraySegment<byte>>();
	Queue<ArraySegment<byte>> sendQueue = new Queue<ArraySegment<byte>>();

	public abstract void OnConnected(EndPoint endpoint);
	public abstract int OnRecv(ArraySegment<byte> buf);
	public abstract void OnSend(int numOfBytes);
	public abstract void OnDisconnected(EndPoint endPoint);

	public void Enter(Socket socket)
	{
		_socket = socket;

		_recvArgs.Completed += new EventHandler<SocketAsyncEventArgs>(OnRecvCompleted);
		_sendArgs.Completed += new EventHandler<SocketAsyncEventArgs>(OnSendCompleted);

		//RegisterRecv();
	}

	void Clear()
	{
		lock (_lock)
		{
			sendQueue.Clear();
			sendingList.Clear();
		}
	}

	public void Escape()
	{
		if (Interlocked.Exchange(ref _escLock, 1) == 1)
			return;

		OnDisconnected(_socket.RemoteEndPoint);
		_socket.Shutdown(SocketShutdown.Both);
		_socket.Close();

		Clear();
	}

	public void Send(ArraySegment<byte> buf)
	{
		//lock (_lock)
		//{
		//	sendQueue.Enqueue(buf);
		//	if (sendingList.Count == 0)
		//	{
		//		RegisterSend();
		//	}
		//}
		_sendArgs.SetBuffer(buf);
		bool pending = _socket.SendAsync(_sendArgs);
		Thread.Sleep(250);
		if (!pending)
			OnSendCompleted(null, _sendArgs);
	}
	public void Send(List<ArraySegment<byte>> buf) { }
	public void RegisterSend()
	{
		if (_escLock == 1)
			return;

		while (sendQueue.Count > 0)
		{
			ArraySegment<byte> buf = sendQueue.Dequeue();
			sendingList.Add(buf);
		}
		_sendArgs.BufferList = sendingList;

		try
		{
			bool pending = _socket.SendAsync(_sendArgs);
			if (!pending)
				OnSendCompleted(null, _sendArgs);
		}
		catch (Exception e)
		{
			Console.ForegroundColor = ConsoleColor.Red;
			Console.WriteLine($"register Send Exception : {e}");
			Console.ForegroundColor = ConsoleColor.White;
		}
	}
	public void RegisterRecv()
	{
		if (_escLock == 1)
			return;

		try
		{
			bool pending = _socket.ReceiveAsync(_recvArgs);
			if (pending == false)
				OnRecvCompleted(null, _recvArgs);
		}
		catch (Exception e)
		{
			Console.BackgroundColor = ConsoleColor.Red;
			Console.WriteLine($"register Recv Exception {e}");
			Console.BackgroundColor = ConsoleColor.White;
		}
	}
	void OnSendCompleted(object sender, SocketAsyncEventArgs args) { }
	void OnRecvCompleted(object sender, SocketAsyncEventArgs args)
	{
		if (args.BytesTransferred > 0 && SocketError.Success == args.SocketError)
		{
			try
			{
				if (!_recvBuffer.OnWrite(args.BytesTransferred))
				{
					Escape();
					return;
				}

				int processLen = OnRecv(_recvBuffer.ReadSegment);
				if (processLen < 0 || _recvBuffer.DataSize < processLen)
				{
					Escape();
					return;
				}

				if (!_recvBuffer.OnRead(processLen))
				{
					Escape();
					return;
				}

				RegisterRecv();
			}
			catch (Exception e)
			{
				Console.BackgroundColor = ConsoleColor.Red;
				Console.WriteLine($"OnRecvCompleted Exception {e}");
				Console.BackgroundColor = ConsoleColor.White;
			}
		}
		else
		{
			Escape();
		}
	}
}

