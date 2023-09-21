#include "pch.h"
#include <iostream>
#include "Corepch.h"
#include <thread> // 리눅스든 윈도우든 상관없음
#include <atomic> // 리눅스든 윈도우든 상관없음
// atomic atom(원자) -> All - Or -Nothing

// DB 와 관련된 작업을 할 때
// A라는 유저 인벤에서 집행검을 빼고
// B라는 유저 인벤에 집행검을 추가 할 때 
// ->atomic 한 연산이 필요하다. 한번에 다 일어나거나 아예 일어나지않게 하는것

atomic<int32> sum = 0;		//int32 sum = 0;
// 생각보다 연산이 많이 느리다 
void Add()
{
	for (int32 i = 0; i < 100'0000; ++i)
	{
		sum.fetch_add(1);
		//++sum;
	}
}

void Sub()
{
	for (int32 i = 0; i < 100'0000; ++i)
	{
		sum.fetch_sub(1);
	
		//--sum;
	}
}


int main()
{
	/*Add();
	Sub();
	cout << sum << endl;*/

	std::thread t1(Add);
	std::thread t2(Sub);
	t1.join();
	t2.join();
	cout << sum << endl;  // 실행 할 때마다 값이 다르다
					      // 공유 데이터를 사용할 때 문제점이 발생한다. -> 동기화의 필요성

}

