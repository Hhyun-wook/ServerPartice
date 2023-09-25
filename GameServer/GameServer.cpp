#include "pch.h"
#include <iostream>
#include "Corepch.h"
#include <thread> // 리눅스든 윈도우든 상관없음
#include <atomic> // 리눅스든 윈도우든 상관없음
#include <mutex>  // 리눅스든 윈도우든 상관없음
#include <future>

// 단발성이벤트에서는 condition_variable 보다 가벼운 future 가 있다.



int64 Calculate()
{
	int64 sum = 0;

	for (int32 i = 0; i < 100'000; ++i)
	{
		sum += i;
	}

	return sum;
}

void PromiseWorker(std::promise<string>&& promise)
{
	promise.set_value("Value");
}

void TaskWorker(std::packaged_task<int64(void)>&& task)
{
	task();
}

int main()
{
	// 동기 방식 실행
	int64 sum = Calculate();
	cout << sum << endl;

	// std::future == 비동기 실행
	{
		// std::async 옵션
		// 1) deferred -> lazy evaluation 지연해서 실행하세요 -> 멀티 쓰레드 x 나중에 호출하세요
		// 2) async -> 별도의 쓰레드를 만들어서 실행하세요  -> 병렬로 연산가능 (비동기랑 멀티쓰레드랑 같은 의미는 아니다.)
		// 3) deferred | async -> 둘중 알아서 골라주세요
		// ex) 서버를 띄웠는데 모든 데이터파일을 로드해서 해야하는데 시간이 오래걸린다. 
		//     그래서 비동기 방식을 통해 만남의 장소에서 호출의 완료되면 끝나게한다.
		std::future<int64> future = std::async(std::launch::async, Calculate);
		
		// 언젠가 미래에 결과물을 뱉어줄거야! == std::future

		// 딴짓 해도 가능
		int64 sum = future.get(); // 결과물이 이제서야 필요하다! 
		
		std::future_status status =future.wait_for(1ms); // 작업이 끝났는지 잠시 살펴보는 기능
		if (status == future_status::ready)
		{

		}

		future.wait(); // 결과물이 이제 필요하다!

		

		class Knight
		{
		public:
			int64 GetHp() { return 100; }
		};

		Knight knight;
		std::future<int64> future2 = std::async(std::launch::async, &Knight::GetHp, knight);
		// 클래스에 속한 멤버 함수는 어떤 객체에 의존적으로 호출되므로 클래스명을 붙어야한다.

		//std::promise
		{
			// 미래(std::future)의 결과물을 반환해줄거라 약속해줘(std::promise)
			std::promise<string> promise;
			std::future<string> future = promise.get_future(); // 1 : 1 로 연동된 상태이다.

			thread t(PromiseWorker, std::move(promise));
			
			string msg = future.get(); // future.get은 한번만 호출해야한다.
			cout << msg << endl;
			t.join();
		}


		// std::packaged_task
		{
			std::packaged_task<int64(void)> task(Calculate); // 함수 인풋 아웃풋 순서대로 <>안에 써줘야한다.
			std::future<int64> future = task.get_future();

			std::thread t(TaskWorker, std::move(task));

			int64 sum = future.get();
			cout << sum << endl;
			t.join();
		}

		// 결론) 
		// mutex , condition_variable 까지 가지 않고 단순한 애들을 처리할 수 있는 future 사용예시 (딱한번, 단순히 한번) 이런경우
		// 특히나 , 한 번 발생하는 이벤트에 유용하다.
		// 1. async
		//  원하는 함수를 비동기적으로 실행
		// 2. promise
		// 결과물을 promise 를 통해 futrue로 받아줌
		// 3. packaged_task
		// 원하는 함수의 실행 결과를 packaged_task를 통해 future로 받아줌
	}

}

