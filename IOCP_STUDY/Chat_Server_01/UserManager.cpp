#include "pch.h"
#include "UserManager.h"

void UserManager::Init(const INT32 maxUserCount)
{
    _maxUserCnt = maxUserCount;
    _userObjPool = std::vector<User*>(_maxUserCnt);

	for (auto i = 0; i < _maxUserCnt; i++)
	{
		_userObjPool[i] = new User();
		_userObjPool[i]->Init(i);
	}
}

void UserManager::DecreaseUserCnt()
{
	if (_currentUserCnt > 0)
	{
		_currentUserCnt--;
	}
}

ERROR_CODE UserManager::AddUser(char* userID, int clientIndex)
{
	auto user_idx = clientIndex;

	_userObjPool[user_idx]->SetLogin(userID);
	_userIDDictionary.insert(std::pair< char*, int>(userID, clientIndex));

	return ERROR_CODE::NONE;
}

INT32 UserManager::FindUserIndexByID(char* userID)
{
	auto res = _userIDDictionary.find(userID);
	if (res != _userIDDictionary.end())
		return (*res).second;

	return -1;
}

void UserManager::DeleteUserInfo(User* user)
{
	_userIDDictionary.erase(user->GetUserId());
	user->Clear();
}
