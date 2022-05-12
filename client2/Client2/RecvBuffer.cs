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
		//버퍼 생성
		buffer = new ArraySegment<byte>(new byte[_bufferSize], 0, _bufferSize);
	}

	public int DataSize { get { return writePos - readPos; } } // 처리해야 하는 데이터 사이즈
	public int FreeSize { get { return buffer.Count - writePos; } } // 사용할 수 있는 남은 버퍼 사이즈

	public ArraySegment<byte> ReadSegment
	{
		get { return new ArraySegment<byte>(buffer.Array, buffer.Offset + readPos, DataSize); } // 데이터를 담을 시작포지션
	}
	public ArraySegment<byte> WriteSegment
	{
		get { return new ArraySegment<byte>(buffer.Array, buffer.Offset + writePos, FreeSize); } // 남은 버퍼
	}

	public void Reset()
	{
		int dataSize = DataSize;
		if (dataSize == 0)
		{//처리 되지 않은 데이터가 없음
			readPos = writePos = 0;
		}
		else
		{//처리할 데이터가 남아있음
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
