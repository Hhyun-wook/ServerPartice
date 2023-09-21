#include "pch.h"
#include <iostream>
#include "Corepch.h"
#include <thread> // 리눅스든 윈도우든 상관없음
#include <atomic> // 리눅스든 윈도우든 상관없음
#include <mutex>  // 리눅스든 윈도우든 상관없음

// [1][2][3]
// [1][2][3][][] 
// 나도 카파시티를 늘리고 해야지 -> 근데 벡터 사이즈를 늘릴때 복사생성자 문제가 있어서 -> 이미 지워진 데이터를 사용할 수있어서 문제가 생긴다.
// 그렇다고 Reserve를 사용해서 사이즈를 늘리면 다른애들을 고려하지않기 때문에 데이터의 분실이 발생할 수 있다.
// atomic은 컨테이너의 세부기능을 사용할 수 없다 그래서 atomic<vector<int32>> v; 이런 구조를 사용할 수 없다.

vector<int32> v; 

// 상호배타적인 성격을 가지고 있다.
mutex m;

//RAII :  자원의 획득은 곧 초기화이다.
template <typename T>
class LockGuard
{
public:
	LockGuard(T& m)
	{
		_mutex = &m;
		_mutex->lock();
	}

	~LockGuard()
	{
		_mutex.unlock();
	}
private:
	T* _mutex;
};

void Push()
{
	for (int32 i = 0; i < 10000; ++i)
	{
		//v.push_back(i); // 초기 문제점
		
		//// 자물쇠 잠그기
		//m.lock();	
		////m.lock();  //재귀적인 사용이 불가능하다. 다른 것을 사용해야함

		//v.push_back(i); 
		//
		////if (i == 5000)
		////	break;		// unlock 을 하지않아 무한대기가 걸린다. 이런 수동관리는 긴함수면 힘든다.

		//// 자물쇠 풀기
		//m.unlock();
		////m.unlock(); //재귀적인 사용이 불가능하다. 다른 것을 사용해야함


		//LockGuard<std::mutex> lockGuard(m);		// 이런식으로 자동으로 잠궈주는 만들어줄 수 있다.
		//std::lock_guard<std::mutex> lockGuard(m);	// == LockGuard<std::mutex> lockGuard(m);	
		
		std::unique_lock<std::mutex> uniqueLock(m,std::defer_lock); // 잠기는 시점을 내가 직접 락을 쓰면 가능하다.  lock_guard 보다 용량이 조금 크다.
		uniqueLock.lock();

		v.push_back(i); 
		
		if (i == 5000)
			break;


	}

	// 싱글쓰레드로 작동하는 코드라고 생각하면 편하다. -> 느리다.
}

int main()
{
	//v.reserve(20000);
	std::thread t1(Push);
	std::thread t2(Push);

	t1.join();
	t2.join();

	cout << v.size() << endl;
}

