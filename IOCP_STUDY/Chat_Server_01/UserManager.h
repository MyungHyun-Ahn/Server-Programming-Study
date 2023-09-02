#pragma once
#include "pch.h"

class UserManager
{
public:
	UserManager() = default;
	~UserManager() = default;

	void Init(const INT32 maxUserCount);

	INT32 GetCurrentUserCnt() { return _currentUserCnt; }
	INT32 GetMaxUserCnt() { return _maxUserCnt; }
	User* GetUserByConnIdx(INT32 clientIndex) { return _userObjPool[clientIndex]; }

	void IncreaseUserCnt() { _currentUserCnt++; }
	void DecreaseUserCnt();

	ERROR_CODE AddUser(char* userID, int clientIndex);
	INT32 FindUserIndexByID(char* userID);
	void DeleteUserInfo(User* user);


private:
	INT32 _maxUserCnt = 0;
	INT32 _currentUserCnt = 0;

	vector<User*> _userObjPool;
	unordered_map<string, int> _userIDDictionary;
};

