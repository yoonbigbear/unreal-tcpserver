using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

class ServerSession : PacketSession
{
	public override void OnConnected(EndPoint _endpoint)
	{
		Console.WriteLine($"Onconnected{_endpoint}");
	}

	public override void OnDisconnected(EndPoint _endPoint)
	{
		Console.WriteLine($"OnDisconnected{_endPoint}");
	}

	public override void OnRecvPacket(ArraySegment<byte> _buf)
	{
		//PacketManager.Instance.OnRecvPacket(this, _buf);
	}

	public override void OnSend(int _numOfBytes)
	{
		//Console.WriteLine($"Transferred bytes = {_numOfBytes}");
	}
}

public abstract class PacketSession : Session
{
	public static readonly short HeaderSize = 2;

	public sealed override int OnRecv(ArraySegment<byte> buf)
	{
		throw new NotImplementedException();
	}
	public abstract void OnRecvPacket(ArraySegment<byte> buf);
}

public abstract class Session
{
	int _escLock = 0;
	object _lock = new object();

	Socket _socket;

	SocketAsyncEventArgs _sendArgs = new SocketAsyncEventArgs();
	SocketAsyncEventArgs _recvArgs = new SocketAsyncEventArgs();

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

		RegisterRecv();
	}

	void Clear() 
	{
		lock (_lock)
		{
			sendQueue.Clear();
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

	public void Send(ArraySegment<byte> buf) { }
	public void Send(List<ArraySegment<byte>> buf) { }
	public void RegisterSend() { }
	public void RegisterRecv() 
	{
		if (_escLock == 1)
			return;

		try
		{
			//bool pending = _socket.ReceiveAsync(_recvArgs);
			//if (pending == false)
			_socket.Receive(_recvArgs.Buffer);
				OnRecvCompleted(null, _recvArgs);
		}
		catch (Exception e)
		{
			Console.WriteLine($"register Recv Exception {e}");
		}
	}
	void OnSendCompleted(object sender, SocketAsyncEventArgs args) { }
	void OnRecvCompleted(object sender, SocketAsyncEventArgs args)
	{
		if (args.BytesTransferred > 0 && SocketError.Success == args.SocketError)
		{
			try
			{
			}
			catch(Exception e)
			{
				Console.WriteLine($"OnRecvCompleted Exception {e}");
			}
		}
		else
		{
			Escape();
		}
	}
}

