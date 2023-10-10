#pragma once
#include "IocpCore.h"
#include "NetAddress.h"

class Listener :public IocpObject
{
public:
	Listener() = default;
	~Listener();

public:
	/*�ܺο��� ���*/
	bool StartAccept(NetAddress netAddress);
	void CloseSocket();

public:
	/*�������̽� ����*/
	virtual HANDLE GetHandle()override;
	virtual void Dispatch(class IocpEvent* IocpEvent, int32 numofBytes = 0) override;

private:
	/*���� ����*/
	void RegisterAccept(class AcceptEvent* acceptEvent);
	void ProcessAccept(class AcceptEvent* acceptEvent);


protected:
	SOCKET _socket = INVALID_SOCKET;
	xVector<class AcceptEvent*> _acceptEvents;


};

