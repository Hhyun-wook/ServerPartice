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

bool IocpCore::Register(IocpObjectRef iocpObject)
{
    return ::CreateIoCompletionPort(iocpObject->GetHandle(),_iocpHandle,
        /*key */0 ,0);
    
}

bool IocpCore::DisPatch(uint32 timeoutMs)
{
    DWORD numofBytes = 0;
    ULONG_PTR key = 0;
    IocpEvent* iocpEvent = nullptr;
    
    if (::GetQueuedCompletionStatus(_iocpHandle, OUT & numofBytes, OUT &key,
        OUT reinterpret_cast<LPOVERLAPPED*>(&iocpEvent), timeoutMs))
    {
        IocpObjectRef iocpObject = iocpEvent->owner;
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
            IocpObjectRef iocpObject = iocpEvent->owner;
            iocpObject->Dispatch(iocpEvent, numofBytes);
            break;
        }
    }


    return false;
}
