using System;
using System.Collections.Generic;
using System.Linq;
using System.Net;
using System.Net.Sockets;
using System.Text;
using System.Threading;
using System.Threading.Tasks;

public class Connector
{
	private Func<Session> _sessionFactory;
	Session _session;
	public void Connect(Session s, IPEndPoint endpoint, Func< Session> sessionFactory, int dummyClientCout = 1)
	{
		for(int i = 0; i < dummyClientCout; ++i)
		{
			var socket = new Socket(endpoint.AddressFamily, SocketType.Stream, ProtocolType.Tcp);
			_sessionFactory = sessionFactory;
			_session = s;
			SocketAsyncEventArgs args = new SocketAsyncEventArgs();
			args.Completed += OnConnectCompleted;
			args.RemoteEndPoint = endpoint;
			args.UserToken = socket;

			RegisterConnect(args);

		}
	}

	void RegisterConnect(SocketAsyncEventArgs args)
	{
		var socket = args.UserToken as Socket;
		if (socket == null)
			return;

		bool pending = socket.ConnectAsync(args);
		Thread.Sleep(250);
		if (!pending)
			OnConnectCompleted(null, args);
		
	}

	void OnConnectCompleted(object sender, SocketAsyncEventArgs args)
	{
		if (SocketError.Success == args.SocketError)
		{
			//_sessionFactory.Invoke();

			_session.Enter(args.ConnectSocket);
			_session.OnConnected(args.RemoteEndPoint);
		}
		else
		{
			Console.ForegroundColor = ConsoleColor.Red;
			Console.WriteLine($"onConnectCompleted failed");
			Console.ForegroundColor = ConsoleColor.White;
		}
	}
}
