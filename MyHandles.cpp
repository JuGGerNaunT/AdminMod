#include "MyHandles.h"
#include "PluginCmds.h"
#include "Support.h"
#include "Admins.h"
#include "PluginFuncs.h"

#include <mutil.h>
#include <meta_api.h>

qboolean ClientConnect(edict_t *pEntity, const char *pszName,
					const char *pszAddress, char szRejectReason[ 128 ])
{
	//����������� � ������� ������
	//��� ����� ����������� ��� ��� ip ������ ��
	//����������� � ��������������� ��������������� ��������
	//����� ����� ����������� ��������� ����������, ���� ��� ������
	//���������� � ������ �������, �� ������� ������
	LOG_CONSOLE(PLID, "Connected:");
	LOG_CONSOLE(PLID, pszName);
	LOG_CONSOLE(PLID, pszAddress);
	LOG_CONSOLE(PLID, szRejectReason);

	int index;
	edict_t *player;
	index = g_engfuncs.pfnIndexOfEdict(pEntity);
	player = g_engfuncs.pfnPEntityOfEntIndex(index);
	LOG_CONSOLE(PLID, "Index of entity - %d", index);
	if(player != pEntity)
		LOG_CONSOLE(PLID, "Entity and index doesn't match");
	players_id[0] = index;

	//Checking is that admin or not
	FILE *file = NULL;
	const char *path = "AdminMod/admins.txt";
	const char *_pass;
	char fullpath[PATH_MAX];
	char line[MAX_CONF_LEN];
	char *name, *pas, *acc;
	name = pas = acc = NULL;
	sup_gamedir_path(path, fullpath);
	LOG_CONSOLE(PLID, fullpath);

	file = fopen(fullpath, "r");
	if(!file)
	{
		LOG_CONSOLE(PLID, "Unable to open admin list file '%s': %s", fullpath,
			strerror(errno));
		RETURN_META_VALUE(MRES_IGNORED, FALSE);
	}

	for(int i = 0; !feof(file); i++)
	{
		if(!fgets(line, sizeof(line), file))
			break;

		if(line[0] == '#' || line[0] == ';')
			continue;
		if(strnmatch(line, "//", 2))
			continue;

		name = strtok(line, " ,\t\n");
		if(!name)
		{
			LOG_CONSOLE(PLID, "Line %d, bad Name", i);
			continue;
		}
		LOG_CONSOLE(PLID, "Name %s read", name);
		if(strcmp(pszName, name) == 0)
		{
			pas = strtok(NULL, " ,\t\n");
			if(!pas)
			{
				LOG_CONSOLE(PLID, "Line %d, bad Password", i);
				continue;
			}
			LOG_CONSOLE(PLID, "Password %s read", pas);

			//��� �� ������ �������� � ��������������� ������
			//���� �� �� ��������� (��� ������ �����������)
			//�� �� ����� �� ������ ��������������� ������
			_pass = ENTITY_KEYVALUE(pEntity, "_pass");
			if(strcmp(_pass, pas))
			{
				//disconnect fake admin
				LOG_CONSOLE(PLID, "Player pass - %s, required - %s\n", _pass, pas);
				FuncKickPlayer(pEntity, pszName, "Your admin password doesn't valid\n");
			}
			
			//����� �������� ��������� ����� ������� � ������
			//� ������� ��� � ������ �������
			acc = strtok(NULL, " ,\t\n");
			if(!acc)
			{
				LOG_CONSOLE(PLID, "Line %d, no access rights", i);
				continue;
			}
			LOG_CONSOLE(PLID, "Access rights are '%s'", acc);
			adminlist.CreateAdmin(name, acc);
		}
	}

	fclose(file);
	//end checking

	RETURN_META_VALUE(MRES_IGNORED, FALSE);
}

void ClientCommand(edict_t *pEntity)
{
	if(strmatch(CMD_ARGV(0), "admin"))
	{
		AdminInfo(pEntity);
	}
	RETURN_META(MRES_IGNORED);
}