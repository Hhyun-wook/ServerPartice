#pragma once

/*
	IOCP_Object
*/

class IocpObject
{
public:
	virtual HANDLE GetHandle() abstract;
	virtual void Dispatch(class IocpEvent* iocpEvent, int32 numofBytes = 0)abstract;
};


/*
	IOCP_Core
*/
class IocpCore
{

public:
	IocpCore();
	~IocpCore();

	HANDLE GetHandle() { return _iocpHandle; }

	bool Register(class IocpObject* iocpObject);
	bool DisPatch(uint32 timeoutMs = INFINITE);


private:
	HANDLE _iocpHandle;

};

extern IocpCore GlocpCore;