using System;
using System.Net;

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
			IPEndPoint endPoint = new IPEndPoint(ipAddr, 60001);
			Console.WriteLine($"end point : {endPoint.Address}:{endPoint.Port}");

			Console.ForegroundColor = ConsoleColor.White;

			Connector connector = new Connector();

			connector.Connect(endPoint, () => { return new ServerSession(); });


		}
	}
}
