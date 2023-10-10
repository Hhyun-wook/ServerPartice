#include "pch.h"
#include "IocpCore.h"
#include "IocpEvent.h"

IocpCore::IocpCore()
{
    _iocpHandle = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);
    ASSERT_CRASH(_iocpHandle != INVALID_HANDLE_VALUE);
}

IocpCore::~IocpCore()
{
    ::CloseHandle(_iocpHandle);
}

bool IocpCore::Register(IocpObject* iocpObject)
{
    SOCKET socket;
    return ::CreateIoCompletionPort(iocpObject->GetHandle(),_iocpHandle,
        /*key */reinterpret_cast<ULONG_PTR>(iocpObject),0);
    
}

bool IocpCore::DisPatch(uint32 timeoutMs)
{
    DWORD numofBytes = 0;
    IocpObject* iocpObject = nullptr;
    IocpEvent* iocpEvent = nullptr;
    
    if (::GetQueuedCompletionStatus(_iocpHandle, OUT & numofBytes, OUT reinterpret_cast<PULONG_PTR>(&iocpObject),
        OUT reinterpret_cast<LPOVERLAPPED*>(&iocpEvent), timeoutMs))
    {
        iocpObject->Dispatch(iocpEvent, numofBytes);
    }
    else
    {
        int32 errCode = ::WSAGetLastError();
        switch (errCode)
        {
        
        case WAIT_TIMEOUT:
            return false;
        default:
            iocpObject->Dispatch(iocpEvent, numofBytes);
            break;
        }
    }


    return false;
}
