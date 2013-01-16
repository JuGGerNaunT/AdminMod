#pragma once

#define MAX_NAME 64
#define NULL 0

enum ACC_RIGHT {GEN, KICK, BAN, MAP};

class AdminList;
extern AdminList adminlist;

class AdminList
{
	struct admin
	{
		char name[MAX_NAME];
		bool gen, kick, ban, map, none;
		admin *prv, *nxt;
		admin()
		{
			name[0] = '\0';
			gen = kick = ban = map = false;
			prv = nxt = NULL;
		}
	};

	admin *AdmList;

	void AddAdmin(admin *data);
	bool RemoveAdmin(const char *value);
	admin* FindAdmin(const char *value);

public:

	AdminList()
	{
		AdmList = NULL;
	}

	void CreateAdmin(const char *name, const char *acc);
	bool DeleteAdmin(const char *name);
	bool IsAdmin(const char *name);
	bool CheckRights(const char *name, ACC_RIGHT acc);
	void NextAdmin(const char **name);
};
