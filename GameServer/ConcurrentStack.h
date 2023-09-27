#pragma once

#include <mutex>

template<typename T>
class LockStack
{
public:
	LockStack() {}

	LockStack(const LockStack&) = delete;
	LockStack& operator = (const LockStack&) = delete;


	void Push(T value)
	{
		lock_guard<mutex> lock(_mutex);
		_stack.push(std::move(value));
		_condVar.notify_one();
	}

	bool TryPop(T& value)
	{
		lock_guard<mutex> lock(_mutex);
		if (_stack.empty())
			return false;

		// 싱글 쓰레드는 empty -> top ->pop 인데 
		// 멀티 쓰레드는 이런식으로 처리해야한다.
		value = std::move(_stack.top());
		_stack.pop();
		return true;
	}
	
	void WaitPop(T& value)
	{
		unique_lock<mutex> lock(_mutex);
		_condVar.wait(lock, [this] {return _stack.empty() == false; });
		value = std::move(_stack.top());
		_stack.pop();
	}


	// 멀티쓰레드환경에서는 empty 체크후 팝하는건 의미가없다.
	/*bool Empty() 
	{
		lock_guard<mutex> lock(_mutex);
		return _stack.empty();
	}*/

private:
	stack<T> _stack;
	mutex _mutex;
	condition_variable _condVar;
};

template<typename T>
class LcokFreeStack
{
	struct Node
	{
		Node(const T& value) : data(value)
		{

		}

		T data;
		Node* next;
	};

public:

	// 1) 새 노드를 만들고
	// 2) 새 노드의 next = head
	// 3) head = 새 노드
	void Push(const T& value)
	{
		Node* node = new Node(value); // 멀티쓰레드 환경에서 만드는 건 상관없음
		node->next = _head;			  // 여기부터 문제가 발생
		// 이 사이에 새치기를 당할 수 있다.
		//_head = node;
		// 그래서 CAS 사용
		while (_head.compare_exchange_weak(node->next, node) == false)
		{

		}
	}

	// 1) head 읽기
	// 2) head->next 읽기
	// 3) head = head->next 
	// 4) data 추출해서 반환
	// 5) 추출한 노드를 삭제
	bool TryPop(T& value)
	{
		Node* oldHead = _head;

	/*	if (head == oldHead)
		{
			_head = oldHead->next;
			return  true;
		}
		else
		{
			OldHead = _head;
			return false;
		}*/

		while (oldHead && _head.compare_exchange_weak(oldHead, oldHead->next) == false)
		{
			//oldHead =_head;
		}

		if (oldHead == nullptr)
			return false;

		//Exception X
		value = oldHead->data;

		// 잠시 삭제 보류
		//delete oldHead;

		return true;
	}

private:

	atomic<Node*> _head;
};
