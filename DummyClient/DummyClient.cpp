#include "pch.h"
#include <iostream>

#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib") // 서버를 사용할려면 필요하다.

int main()
{

	// 윈속 초기화(ws2_32 라이브러리 초기화)
	// 관련 정보가 wsaData에 채워짐
	WSAData wsaData;
	if (::WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 0;

	// 기본적으로 한국 게임은 TCP이다.
	// TCP는 통신을 UDP보다 안전하게 해준다.


	// af : address Family (AF_INET = IPv4 , AF_INET6 = IPv6)
	// type : TCP(SOCK_STREAM, vs UDP(SOCK_DGRAM)
	// protocol : descriptor
	// return : descriptor
	SOCKET clientSocket = ::socket(AF_INET, SOCK_STREAM, 0);		// 소문자는 리눅스에서도 사용가능할 수 있음
	if (clientSocket == INVALID_SOCKET)
	{
		int32 errCode = ::WSAGetLastError();
		cout << "Sokcet ErrCode : " << errCode <<endl;
		return 0;
	}

	// 연결할 목적지는? (IP 주소 + port) -> xx 아파트 yy호
	SOCKADDR_IN serverAddr; // IPv4 버젼
	::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	//serverAddr.sin_addr.s_addr = ::inet_addr("127.0.0.1"); 구식버전
	::inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr); // 현대식 버전
	serverAddr.sin_port = ::htons(7777);  // 80 : HTTP 특정번호는 규약이 있다.

	// htons 
	// host to networt short(long 도있다.) 
	// Little-Endian vs Big-Endian
	// ex) 0x12345678 4바이트 정수
	// low [0x78][0x56][0x34][0x12] high   little 앤디안
	// low [0x12][0x34][0x56][0x78] high   big    앤디안 -> network는 빅앤디안규약이다.
	// 왠만하면 리틀앤디안 사용한다. 나는 리틀앤디안으로 하는데 상대통신은 빅앤디안을 사용한다?
	// 문제발생 
	// 이러한 빅앤디안 리틀앤디안을 맞춰주기위해 htons 함수를 사용한다.
	// 반대 버전의 함수도 있다. 
	if (::connect(clientSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		int32 errCode = ::WSAGetLastError();
		cout << "Sokcet ErrCode : " << errCode << endl;
		return 0;
	}

	// -------------------------
	// 연결 성공 : 이제부터 데이터 송수신 가능!

	cout << "Connected To Server ! " << endl;

	while (true)
	{
		//ToDo
		char sendBuffer[100] = "HelloServer";

		for (int32 i = 0; i < 10; ++i)
		{
			int32 reslutCode = ::send(clientSocket, sendBuffer, sizeof(sendBuffer), 0);
			if (reslutCode == SOCKET_ERROR)
			{
				int32 errCode = ::WSAGetLastError();
				cout << "Sokcet ErrCode : " << errCode << endl;
				return 0;
			}

		}

		
		cout << "Send Data Len : " << sizeof(sendBuffer) << endl;

	/*	char recvBuffer[1000];
		int32 recvLen = ::recv(clientSocket, recvBuffer, sizeof(recvBuffer), 0);

		if (recvLen <= 0)
		{
			int32 errCode = ::WSAGetLastError();
			cout << "Accept ErrorCode : " << errCode << endl;
			return 0;
		}

		cout << "Client Recv Data :  Data : " << recvBuffer << endl;
		cout << "Client Recv Data :  Len : " << recvLen << endl;*/




		this_thread::sleep_for(1s);
	}


	// ----------------------------

	// 소켓 리소스 반환
	::closesocket(clientSocket);

	// 윈속 종료
	::WSACleanup();


}

