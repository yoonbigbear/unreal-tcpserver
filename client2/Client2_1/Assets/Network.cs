using System;
using System.Collections;
using System.Collections.Generic;
using System.Net;
using System.Net.Sockets;
using UnityEngine;

public class Network : MonoBehaviour
{
    static string host = Dns.GetHostName();
    static IPHostEntry ipHost = Dns.GetHostEntry(host);
    static IPAddress ipAddr = ipHost.AddressList[1]; // 0: ip6,  1: ip4
    static IPEndPoint endPoint = new IPEndPoint(ipAddr, 11000);
    static Socket socket = new Socket(endPoint.AddressFamily, SocketType.Stream, ProtocolType.Tcp);

    // Start is called before the first frame update
    void Start()
    {
        Connector connector = new Connector();
    }

    // Update is called once per frame
    void Update()
    {
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
}
