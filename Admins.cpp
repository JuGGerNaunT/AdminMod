#pragma warning(disable : 4996)
#include "Admins.h"
#include "string.h"

#include <extdll.h>
#include <support_meta.h>

AdminList::admin* AdminList::FindAdmin(const char *value)
{
	admin *ptr = AdmList;
	while(ptr)
	{
		if(strcmp(ptr->name, value) == 0)
			return ptr;
		ptr = ptr->nxt;
	}
	return NULL;
}

void AdminList::AddAdmin(admin *data)
{
	admin *ptr = AdmList;
	if(!ptr)
	{
		ptr = data;
		return;
	}

	while(ptr->nxt)
		ptr = ptr->nxt;
	ptr->nxt = data;
}

bool AdminList::RemoveAdmin(const char *value)
{
	admin *ptr = FindAdmin(value);
	if(ptr)
	{
		if(ptr->prv)
			ptr->prv->nxt = ptr->nxt;
		if(ptr->nxt)
			ptr->nxt->prv = ptr->prv;
		delete ptr;
		return true;
	}
	return false;
}

void AdminList::CreateAdmin(const char *name, const char *acc)
{
	admin *adm = new admin();
	STRNCPY(adm->name, name, sizeof(adm->name));
	for(int i = strlen(acc); i; i--)
	{
		switch(acc[i - 1])
		{
		case 'a':
			adm->gen = true;
			break;
		case 'b':
			adm->kick = true;
			break;
		case 'c':
			adm->ban = true;
			break;
		case 'd':
			adm->map = true;
			break;
		}
	}
	AddAdmin(adm);
}

bool AdminList::DeleteAdmin(const char *name)
{
	return RemoveAdmin(name);
}

bool AdminList::IsAdmin(const char *name)
{
	admin *ptr = FindAdmin(name);
	if(ptr)
		return true;
	else return false;
}

bool AdminList::CheckRights(const char *name, AdminList::ACC_RIGHT acc)
{
	if(IsAdmin(name) == false)
		return false;

	admin *ptr = FindAdmin(name);
	switch(acc)
	{
	case GEN:
		if(ptr->gen)
			return true;
		break;
	case KICK:
		if(ptr->kick)
			return true;
		break;
	case BAN:
		if(ptr->ban)
			return true;
		break;
	case MAP:
		if(ptr->map)
			return true;
		break;
	}
	return false;
}