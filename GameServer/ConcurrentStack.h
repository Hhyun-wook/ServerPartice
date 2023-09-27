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

		// �̱� ������� empty -> top ->pop �ε� 
		// ��Ƽ ������� �̷������� ó���ؾ��Ѵ�.
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


	// ��Ƽ������ȯ�濡���� empty üũ�� ���ϴ°� �ǹ̰�����.
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

	// 1) �� ��带 �����
	// 2) �� ����� next = head
	// 3) head = �� ���
	void Push(const T& value)
	{
		Node* node = new Node(value); // ��Ƽ������ ȯ�濡�� ����� �� �������
		node->next = _head;			  // ������� ������ �߻�
		// �� ���̿� ��ġ�⸦ ���� �� �ִ�.
		//_head = node;
		// �׷��� CAS ���
		while (_head.compare_exchange_weak(node->next, node) == false)
		{

		}
	}

	// 1) head �б�
	// 2) head->next �б�
	// 3) head = head->next 
	// 4) data �����ؼ� ��ȯ
	// 5) ������ ��带 ����
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

		// ��� ���� ����
		//delete oldHead;

		return true;
	}

private:

	atomic<Node*> _head;
};
