#include "pch.h"
#include <iostream>
#include "Corepch.h"
#include <thread> // 리눅스든 윈도우든 상관없음
#include <atomic> // 리눅스든 윈도우든 상관없음
#include <mutex>  // 리눅스든 윈도우든 상관없음
#include <future> // 리눅스든 윈도우든 상관없음
#include <Windows.h>


//int main()
//{
//	atomic<bool> flag = false;
//
//	flag.store(true,std::memory_order::memory_order_seq_cst);
//
//	bool val = flag.load(memory_order::memory_order_seq_cst);
//
//	// 이전 flag 값을 prev에 넣고, flag 값을 수정
//	{
//		bool prev = flag.exchange(true);    // == bool prev = flag;	flag = true; 앞에 것으로 써야한다.
//	}
//	
//	// CAS (Compare - And - SWAP) 조건부 수정
//	{
//		bool expected = false;
//		bool desired = true;
//		flag.compare_exchange_strong(expected, desired);	// Weak 보다 부하가 크지만 묘한 상황을 막을 수 있다.
//
//		if (flag == expected)
//		{
//			// 다른 쓰레드의 interruption을 받아서 중간에 실패할 수 있다.
//			// if(묘한 상황)
//			//	  return false;
//			// flag.compare_exchange_weak(expected, desired) // compare_exchange_strong(expected, desired) 거의 비슷하지만
//									// Spurios Failure 가짜 실패 상황이 있을 때 쓰면된다.
//									// 기본적으로 성능 차이는 없지만 묘한상황을 처리할 때 있다.
//									// Weak가 먼저 나온 후 Strong이 나왔다.
//									// Weak를 쓸거면 while문 처리를 따로 해줘야한다.
//
//			flag = desired;
//			return true;
//		}
//		else
//		{
//			expected = flag;
//			return false;
//		}
//
//	
//	}
//
//
//}

atomic<bool> ready;
int32 value;

void Producer()
{
	value = 10;
	//value2 =20; //  memory_order_release  사용할 때 이러면 재배치되는 문제를 막을 수 없다  => 절취선 위에 있으니까
	//ready.store(true, memory_order::memory_order_relaxed); // -> 위에 있는 value 가 64 번째 줄로 갈 수 있다. 거의 사용할 일이 없다.

	ready.store(true, memory_order::memory_order_release);  
	// ---- 절취선 ----------------- 강제로 넘어갈 수 없게 막는다.

	// 절취선 만드는 함수가 있다. 실전에서 사용할 경우는 굳이 없다.
	// std::atomic_thread_fence(memory_order::memory_order_seq_cst);
}

void Consumer()
{
	/*while (ready.load(memory_order::memory_order_relaxed)==false) // -> 위에 있는 value 가 64 번째 줄로 갈 수 있다. 거의 사용할 일이 없다.
		;
	*/

	// ---- 절취선 ----------------- 강제로 넘어갈 수 없게 막는다.
	while (ready.load(memory_order::memory_order_acquire) == false) 
		;

	cout << value <<endl;
}

int main()
{
	ready = false;
	value = 0;
	thread t1(Producer);
	thread t2(Consumer);

	t1.join();
	t2.join();

	// Memory Model (정책)
	// 1) Sequentially Consistent (seq_cst)
	// 2) Acquire-Release (acquire,release)
	// 3) Relaxed (relaxed)

	// 1) seq_cst (가장 엄격 = 컴파일러 최적화 여지 적음 = 직관적)
	//	  가시성 문제 바로 해결, 코드 재배치 바로 해결! => 가장 많이 사용
	
	// 2) acquire_release
	// 딱 중간!
	// release 명령 이전의 메모리 명령들이, 해당 명령 이후로 재배치 되는 것을 금지
	// 그리고 acquire로 같은 변수를 읽는 쓰레드가 있다면
	// Release 이전의 명령들이 -> acquire 하는 순간에 관찰 가능하다.(가시성이 보장이 된다.)
	
	// 3) relaxed (자유롭다 = 컴파일러 최적화 여지 많음 = 직관적이지 않음)
	// 너무나도 자유롭다!
	// 코드 재배치도 멋대로 가능! 가시성 해결 NO!
	// 가장 기본 조건 (동일 객체에 대한 동일 관전 순서만 보장)

	// 인텔, AMD 의 경우 애당초 순차적 일관성을 보장해서
	// seq_cst를 써도 별다른 부하가 없다.
	// ARM 의 경우 차이가 있다.
}