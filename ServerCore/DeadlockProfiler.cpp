#include "pch.h"
#include "DeadlockProfiler.h"

/*
	DeadlockProFiler
*/

void DeadlockProfiler::PushLock(const char* name)
{
	//��Ƽ������ȯ��
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
	
	// ��� �ִ� ���� �־��ٸ�
	if (LLockStack.empty() == false)
	{
		// ������ �߰ߵ��� ���� ���̽���� ����� ���θ� �ٽ� Ȯ���Ѵ�.
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

	//��� ������ ������ ��ȸ�Ѵ�.
	auto findit = _lockHistory.find(here);
	if (findit == _lockHistory.end())
	{
		_finished[here] = true;
		return;
	}


	set<int32>& nextSet = findit->second;
	for (int32 there : nextSet)
	{
		// ���� �湮�� ���� ���ٸ� �湮�Ѵ�.
		if (_discoveredOrder[there] == -1)
		{
			_parent[there] = here;
			Dfs(there);
			continue;
		}

		// here�� there ���� ���� �߰ߵǸ�
		// there�� here�� �ļ��̴�.(������ ����)
		if (_discoveredOrder[here] < _discoveredOrder[there])
			continue;

		// �������� �ƴϰ� Dfs()�� ���� �������� �ʾҴٸ� ������ �����̴�.
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
