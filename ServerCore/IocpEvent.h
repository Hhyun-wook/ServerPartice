#pragma once

class Session;

enum class EventType : uint8
{
	Connect,
	DisConnect,
	Accept,
	//PreRecv,
	Recv,
	Send,
};

/*
	IocpEvent
*/

class IocpEvent : public OVERLAPPED
{
public:
	IocpEvent(EventType type);

	void	Init();
	EventType GetType() { return _eventtype; }

public:
	EventType _eventtype;
	IocpObjectRef owner;
};

/*
	ConnectEvent
*/
class ConnectEvent :public IocpEvent
{
public:
	ConnectEvent() : IocpEvent(EventType::Connect){}
};


/*
	DisConnectEvent
*/
class DisConnectEvent :public IocpEvent
{
public:
	DisConnectEvent() : IocpEvent(EventType::DisConnect) {}
};


/*
	AcceptEvent
*/
class AcceptEvent :public IocpEvent
{
public:
	AcceptEvent() : IocpEvent(EventType::Accept) {}

	
public:
	SessionRef  _session = nullptr;
};


/*
	RecvEvent
*/
class RecvEvent :public IocpEvent
{
public:
	RecvEvent() : IocpEvent(EventType::Recv) {}
};


/*
	SendEvent
*/
class SendEvent :public IocpEvent
{
public:
	SendEvent() : IocpEvent(EventType::Send) {}

	xVector<SendBufferRef> SendBuffers;
};
