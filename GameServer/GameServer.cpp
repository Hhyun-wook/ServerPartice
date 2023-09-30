#include "pch.h"
#include <iostream>
#include "Corepch.h"
#include <thread> // 리눅스든 윈도우든 상관없음
#include <atomic> // 리눅스든 윈도우든 상관없음
#include <mutex>  // 리눅스든 윈도우든 상관없음
#include <future> // 리눅스든 윈도우든 상관없음
#include <Windows.h>
#include "ThreadManager.h"
#include "RefCounting.h"

class Wraight : public RefCountable
{
public:


public:
	int _hp = 150;
	int _posX = 0;
	int _posY = 0;

};

using WraightRef = TSharedPtr<Wraight>;

class Missile : public RefCountable
{
public:
	
	void SetTarget(WraightRef target)
	{ 
		// 싱글스레드에서 상관없지만 멀티쓰레드에서는 문제가 된다.
		_target = target;
		// 중간에 개입 가능하다.(멀티쓰레드에서 문제가 생김)
		//target->AddRef();  // 래퍼랜스 카운트를 증가시키는 것이랑
	}

	void Test(WraightRef& target)
	{
		// 복사비용이 아쉬우면 참조를 사용하면 래퍼랜스가 늘어나지않는다.
		// 그래서 잠시만 사용할거면 참조를 이용해서 사용하면 래퍼랜스카운트가 증가하지않는다.
	}


	bool Update()
	{
		if (_target == nullptr)
			return true;

		int PosX = _target->_posX;
		int PosY = _target->_posY;
	
		if (_target->_hp == 0)
		{
			_target->ReleaseRef();
			_target = nullptr;
			return true;
		}

		return false;
	}

	Wraight* _target = nullptr;
	

};

using MissileRef = TSharedPtr<Missile>;

int main()
{
	WraightRef wraight(new Wraight());
	wraight->ReleaseRef();
	
	MissileRef missile(new Missile());
	missile->ReleaseRef();


	

	missile->SetTarget(wraight);
		
	wraight->_hp = 0;
	//wraight->ReleaseRef();
	wraight = nullptr;

	while (true)
	{
		if (missile->Update())
		{
			missile->ReleaseRef();
			missile = nullptr;

		}
	}
	
	

}