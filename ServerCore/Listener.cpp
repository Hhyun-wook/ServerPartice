#include "pch.h"
#include "Listener.h"
#include "SocketUtils.h"
#include "IocpEvent.h"
#include "Session.h"
#include "Service.h"

Listener::~Listener()
{
    SocketUtils::Close(_socket);

    for (AcceptEvent* acceptEvent : _acceptEvents)
    {
        // TO_DO

        xdelete(acceptEvent);
    }

}

bool Listener::StartAccept(ServerServiceRef service)
{
    _service = service;
    if (_service == nullptr)
        return false;


    _socket = SocketUtils::createSocket();
    if (_socket == INVALID_SOCKET)
        return false;

    if (_service->GetIocpCore()->Register(shared_from_this()) == false)
        return false;

    if (SocketUtils::SetReuseAddress(_socket, true) == false)
        return false;

    if (SocketUtils::SetLinger(_socket, 0, 0) == false)
        return false;

    if (SocketUtils::Bind(_socket, _service->GetNetAddress()) == false)
        return false;

    if (SocketUtils::Listen(_socket) == false)
        return false;

    const int32 acceptCount = _service->GetMaxSessionCount();

    for (int32 i = 0; i < acceptCount; ++i)
    {
        AcceptEvent* acceptEvent = xnew<AcceptEvent>();
        acceptEvent->owner = shared_from_this();        //shared_ptr�� �Ҷ� �� �˾Ƶξ���Ѵ�.
        _acceptEvents.push_back(acceptEvent);
        RegisterAccept(acceptEvent);
    }



    return true;
}

void Listener::CloseSocket()
{
    SocketUtils::Close(_socket);
}

HANDLE Listener::GetHandle()
{
    return reinterpret_cast<HANDLE>(_socket);
}

void Listener::Dispatch(IocpEvent* IocpEvent, int32 numofBytes)
{
    ASSERT_CRASH(IocpEvent->GetType() == EventType::Accept);

    AcceptEvent* acceptEvent = static_cast<AcceptEvent*>(IocpEvent);
    ProcessAccept(acceptEvent);
}

void Listener::RegisterAccept(AcceptEvent* acceptEvent)
{
    SessionRef session = _service->CreateSession();  //  Register IOCP

    acceptEvent->Init();
    acceptEvent->_session = session;

    DWORD bytesRecv = 0;
    if (false == SocketUtils::AcceptEx(_socket, session->GetSocket(), session->_recvBuffer, 0,
        sizeof(SOCKADDR_IN) + 16, sizeof(SOCKADDR_IN) + 16,
        OUT & bytesRecv, static_cast<LPOVERLAPPED>(acceptEvent)))
    {
        const int32 errcode = WSAGetLastError();
        if (errcode != WSA_IO_PENDING)
        {
            // �ϴ� �ٽ� acceptEvent �� �ɾ��ش�.
            RegisterAccept(acceptEvent);
        }
    }
}

void Listener::ProcessAccept(AcceptEvent* acceptEvent)
{
    SessionRef session = acceptEvent->_session;

    if (false == SocketUtils::SetUpdateAcceptSocket(session->GetSocket(), _socket))
    {
        RegisterAccept(acceptEvent);
        return;
    }
    
    SOCKADDR_IN sockAddress;
    int32 sizeofSockAddr = sizeof(sockAddress);
    if (SOCKET_ERROR == ::getpeername(session->GetSocket(), OUT reinterpret_cast<SOCKADDR*>(&sockAddress),
        &sizeofSockAddr))
    {
        RegisterAccept(acceptEvent);
        return;
    }

    session->SetNetAddress(NetAddress(sockAddress));
    session->ProcessConnect();
    RegisterAccept(acceptEvent);
}
