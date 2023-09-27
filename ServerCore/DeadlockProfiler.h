#pragma once

#include <stack>
#include <map>
#include <vector>

/*
	DeadlockProFiler
*/

// 그래프 알고리즘 적용
class DeadlockProfiler
{
public:
	void PushLock(const char* name);
	void PopLock(const char* name);
	void CheckCycle();

private:
	void Dfs(int32 here);

private:
	unordered_map<const char*, int32> _nameToId;
	unordered_map<int32,const char*> _idToName;
	stack<int32>					 _lockStack;
	map<int32, set<int32>>			_lockHistory; // 간선에 대한 정보

	Mutex _lock;

private:
	vector<int32>					_discoveredOrder; // 노드가 발견된 순서를 기록하는 배열
	int32 _discoveredCount = 0;		//노드가 발견된 순서
	vector<bool> _finished; //dfs[i]가 종료되었는지여부
	vector<int32> _parent;

};

