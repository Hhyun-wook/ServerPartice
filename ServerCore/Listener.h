#pragma once
#include "IocpCore.h"
#include "NetAddress.h"

class Listener :public IocpObject
{
public:
	Listener() = default;
	~Listener();

public:
	/*외부에서 사용*/
	bool StartAccept(NetAddress netAddress);
	void CloseSocket();

public:
	/*인터페이스 구현*/
	virtual HANDLE GetHandle()override;
	virtual void Dispatch(class IocpEvent* IocpEvent, int32 numofBytes = 0) override;

private:
	/*수신 관련*/
	void RegisterAccept(class AcceptEvent* acceptEvent);
	void ProcessAccept(class AcceptEvent* acceptEvent);


protected:
	SOCKET _socket = INVALID_SOCKET;
	xVector<class AcceptEvent*> _acceptEvents;


};

