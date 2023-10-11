#include "pch.h"
#include "DeadlockProfiler.h"

/*
	DeadlockProFiler
*/

void DeadlockProfiler::PushLock(const char* name)
{
	//멀티쓰레드환경
	LockGuard guard(_lock);

	int32 lockId = 0;

	auto findit = _nameToId.find(name);
	if (findit == _nameToId.end())
	{
		lockId = static_cast<int32>(_nameToId.size());
		_nameToId[name] = lockId;
		_idToName[lockId] = name;
	}
	else
	{
		lockId = findit->second;
	}
	
	// 잡고 있는 락이 있었다면
	if (LLockStack.empty() == false)
	{
		// 기존에 발견되지 않은 케이스라면 데드락 여부를 다시 확인한다.
		const int32 prevId = LLockStack.top();
		if (lockId != prevId)
		{
			set<int32>& history = _lockHistory[prevId];
			if (history.find(lockId) == history.end())
			{
				history.insert(lockId);
				CheckCycle();
			}
		}

	}

	LLockStack.push(lockId);

}

void DeadlockProfiler::PopLock(const char* name)
{
	LockGuard guard(_lock);

	if (LLockStack.empty())
		CRASH("DeadlockProfiler::PopLock_(LLockStack.empty()");

	int32 lockId = _nameToId[name];
	if (LLockStack.top() != lockId)
		CRASH("DeadlockProfiler::PopLock__lockStack.top() != lockId");

	LLockStack.pop();

}

void DeadlockProfiler::CheckCycle()
{
	const int32 lockCount = static_cast<int32>(_nameToId.size());
	_discoveredOrder = vector<int32>(lockCount, -1);
	_discoveredCount = 0;
	_finished = vector<bool>(lockCount,false);
	_parent = vector<int32>(lockCount, -1);

	for (int32 lockId = 0; lockId < lockCount; ++lockId)
	{
		Dfs(lockId);
	}

	_discoveredOrder.clear();
	_finished.clear();
	_parent.clear();
}

void DeadlockProfiler::Dfs(int32 here)
{
	if (_discoveredOrder[here] != -1)
		return;

	_discoveredOrder[here] = _discoveredCount++;

	//모든 인접한 정점을 순회한다.
	auto findit = _lockHistory.find(here);
	if (findit == _lockHistory.end())
	{
		_finished[here] = true;
		return;
	}


	set<int32>& nextSet = findit->second;
	for (int32 there : nextSet)
	{
		// 아직 방문한 적이 없다면 방문한다.
		if (_discoveredOrder[there] == -1)
		{
			_parent[there] = here;
			Dfs(there);
			continue;
		}

		// here가 there 보다 먼저 발견되면
		// there는 here의 후손이다.(순방향 간선)
		if (_discoveredOrder[here] < _discoveredOrder[there])
			continue;

		// 순방향이 아니고 Dfs()가 아직 종료하지 않았다면 역방향 간선이다.
		if (_finished[there] == false)
		{
			printf("%s -> %s\n", _idToName[here], _idToName[there]);

			int32 now = here;

			while (true)
			{
				printf("%s -> %s \n", _idToName[_parent[now]], _idToName[now]);
				now = _parent[now];
				if (now == there)
					break;
			}
			

			CRASH("DeadLOCK_DETECTED");
				
		}
	}
	_finished[here] = true;
}
