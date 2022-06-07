using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

class RecvBuffer
{
	ArraySegment<byte> buffer;
	int readPos;
	int writePos;

	public RecvBuffer(int _bufferSize)
	{
		//���� ����
		buffer = new ArraySegment<byte>(new byte[_bufferSize], 0, _bufferSize);
	}

	public int DataSize { get { return writePos - readPos; } } // ó���ؾ� �ϴ� ������ ������
	public int FreeSize { get { return buffer.Count - writePos; } } // ����� �� �ִ� ���� ���� ������

	public ArraySegment<byte> ReadSegment
	{
		get { return new ArraySegment<byte>(buffer.Array, buffer.Offset + readPos, DataSize); } // �����͸� ���� ����������
	}
	public ArraySegment<byte> WriteSegment
	{
		get { return new ArraySegment<byte>(buffer.Array, buffer.Offset + writePos, FreeSize); } // ���� ����
	}

	public void Reset()
	{
		int dataSize = DataSize;
		if (dataSize == 0)
		{//ó�� ���� ���� �����Ͱ� ����
			readPos = writePos = 0;
		}
		else
		{//ó���� �����Ͱ� ��������
			Array.Copy(buffer.Array, buffer.Offset + readPos, buffer.Array, buffer.Offset, dataSize);
			readPos = 0;
			writePos = dataSize;
		}
	}

	public bool OnRead(int _numOfBytes)
	{
		if (_numOfBytes > DataSize)
			return false;

		readPos += _numOfBytes;
		return true;
	}

	public bool OnWrite(int _numOfBytes)
	{
		if (_numOfBytes > FreeSize)
			return false;

		writePos += _numOfBytes;
		return true;
	}
}
