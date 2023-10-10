#pragma once
#include "IocpCore.h"
#include "IocpEvent.h"
#include "NetAddress.h"

class Session
{
public:
	Session();
	virtual ~Session();

public:
	/* ���� ����*/
	void SetNetAddress(NetAddress address) { _netAddress = address; }
	NetAddress GetAddress() { return _netAddress; }
	SOCKET		GetSocket() { return _socket; }

public:
	/*�������̽� ����*/
	virtual HANDLE GetHandle();
	virtual void Dispatch(class IocpEvent* IocpEvent, int32 numofBytes = 0) ;


public:
	char _recvBuffer[1000];

private:
	SOCKET _socket = INVALID_SOCKET;
	NetAddress	_netAddress = {};
	Atomic<bool> _connected = false;

};

