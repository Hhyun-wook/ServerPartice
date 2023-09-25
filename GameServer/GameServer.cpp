#include "pch.h"
#include <iostream>
#include "Corepch.h"
#include <thread> // 리눅스든 윈도우든 상관없음
#include <atomic> // 리눅스든 윈도우든 상관없음
#include <mutex>  // 리눅스든 윈도우든 상관없음
#include <future> // 리눅스든 윈도우든 상관없음
#include <Windows.h>
int32 buffer[10000][10000];



int main()
{
	memset(buffer, 0, sizeof(buffer));

	{
		int64 sum = 0;

		uint64 start = GetTickCount64();

		for (int32 i = 0; i < 10000; ++i)
		{
			for (int32 j = 0; j < 10000; ++j)
				sum += buffer[i][j];
		}

		uint64 end = GetTickCount64();
		cout << "Elasped_Tick" << end - start << endl;
	}


	{
		int64 sum = 0;

		uint64 start = GetTickCount64();

		for (int32 i = 0; i < 10000; ++i)
		{
			for (int32 j = 0; j < 10000; ++j)
				sum += buffer[j][i];
		}

		uint64 end = GetTickCount64();
		cout << "Elasped_Tick" << end - start << endl;
	}
	// 거의 3배이상의 차이가 난다.
	// 공간지역성 + 시간지역성의 원리로 작성된 캐시 철학 때문이다.



}

