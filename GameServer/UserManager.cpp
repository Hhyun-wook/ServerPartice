#include "pch.h"
#include "UserManager.h"
#include "AccountManager.h"

void UserManager::ProcessSave()
{
	// UserLock
	lock_guard<mutex> guard(_mutex);

	// AccountLock
	Account* Account = AccountManager::instance()->GetAccount(100);

}
