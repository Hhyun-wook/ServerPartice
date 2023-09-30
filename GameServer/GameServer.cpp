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



int main()
{


	// 직접 만든 래퍼랜스카운팅의 한계
	// 1) 이미 만들어진 클래스 대상으로 사용불가
	// 2) 표준 shared_ptr도 똑같은 문제 : 순환문제
	// 순환 문제 : 한 클래스에서 다른 클래스의 객체를 멤버변수로 가지고 있어서
	//			   지워질때 래퍼랜스카운트가 남아있는 문제입니다.
	// 또한 클래스에서 객체를 멤버변수로 가질 때 참조형식으로 가지고 
	//      있으면 순환문제를 해결할 수 있다.

	// Unique_ptr : 생포인터를 사용하는것이랑 차이가 없지만 복사하는 부분이 막혀있다.
	//			  :  소유권의 이전(이동)은 가능하다.
	// shared_ptr : 
	// weak_ptr   : weak 포인터를 이용해서 순환문제를 해결할 수 있다.

	// shared_Ptr을 만들게 되면
	// [ptr][RefCountingBlock] 
	// 2개의 포인터로 되어있다. 하나는 포인터, 하나는 레퍼랜스카운팅과관련된 포인터가 있다.
	
	// make_shared<T>(); -> [ptr][RefCountingBlock]  : 1+1으로 정보를 넣는다.
	// RefCountingBlock : shared, weak 둘다 공통으로 사용하는 것이다.
	// RefCountingBlock (useCount(shared), weakCount)
	// 둘다 카운트 1로 시작한다. useCount(shared(1)), weakCount(1))
	shared_ptr<int> spr = make_shared<int>();
	weak_ptr<int> wpr = spr;

	bool expired = wpr.expired();	// 포인터가 존재하나?
	shared_ptr<int> spr2 = wpr.lock(); // 다시 shared_ptr로 캐스팅하는법

	// weak_ptr : 상대방의 수명주기에 영향을 주지 않기 때문에 순환문제를 해결할 수 있다.


}