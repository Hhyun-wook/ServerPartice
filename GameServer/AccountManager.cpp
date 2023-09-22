#include "pch.h"
#include "AccountManager.h"
#include "UserManager.h"

void AccountManager::ProcessLogin()
{
	// accountLock
	lock_guard<mutex> guard(_mutex);

	// UserLock
	User* user = UserManager::instance()->GetUser(10);


	// 2개의 락을 잡는 경우
	// 쓰레드 끼리 경합상태에 빠져 데드락현상이 빠질 수있다.

}
