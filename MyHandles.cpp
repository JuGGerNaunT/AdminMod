#include "MyHandles.h"
#include "PluginCmds.h"
#include "Support.h"
#include "Admins.h"
#include "PluginFuncs.h"
#include "Players.h"

#include <mutil.h>
#include <meta_api.h>

qboolean ClientConnect(edict_t *pEntity, const char *pszName,
					const char *pszAddress, char szRejectReason[ 128 ])
{
	//Подключение к серверу игрока
	//Тут будет проверяться имя или ip игрока на
	//забаненость и предприниматься соответственные действия
	//Также будет проверяться локальные переменные, если ник игрока
	//содержится в списке админов, на пердмет пароля
	LOG_CONSOLE(NULL, "Connected:");
	LOG_CONSOLE(NULL, pszName);
	LOG_CONSOLE(NULL, pszAddress);
	LOG_CONSOLE(NULL, szRejectReason);

	int index;
	edict_t *player;
	index = g_engfuncs.pfnIndexOfEdict(pEntity);
	player = g_engfuncs.pfnPEntityOfEntIndex(index);
	LOG_CONSOLE(NULL, "Index of entity - %d", index);
	if(player != pEntity)
		LOG_CONSOLE(NULL, "Entity and index doesn't match");
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
	LOG_CONSOLE(NULL, fullpath);

	if(FuncIsBanned(pszName, index))
	{
		if(index == 0)
		{
			FuncKickPlayer(pEntity, pszName, "\nYour are banned forever\n");
			RETURN_META_VALUE(MRES_IGNORED, FALSE);
		}
		else if(index > 0)
		{
			sup_make_str(line, sizeof(line), "\nYour are banned. Time left - %d\n", index);
			FuncKickPlayer(pEntity, pszName, line);
			RETURN_META_VALUE(MRES_IGNORED, FALSE);
		}
	}

	file = fopen(fullpath, "r");
	if(!file)
	{
		LOG_CONSOLE(NULL, "Unable to open admin list file '%s': %s", fullpath,
			strerror(errno));
		RETURN_META_VALUE(MRES_IGNORED, FALSE);
	}

	for(int i = 1; !feof(file); i++)
	{
		if(!fgets(line, sizeof(line), file))
			break;

		if(line[0] == '#' || line[0] == ';')
			continue;
		if(strnmatch(line, "//", 2))
			continue;

		name = strtok(line, "\"");
		if(!name)
		{
			LOG_CONSOLE(NULL, "Line %d, bad Name", i);
			continue;
		}
		sup_del_quotes(name);
		LOG_CONSOLE(NULL, "Name %s read", name);
		if(strcmp(pszName, name) == 0)
		{
			pas = strtok(NULL, " ,\t\n");
			if(!pas)
			{
				LOG_CONSOLE(NULL, "Line %d, bad Password", i);
				continue;
			}
			LOG_CONSOLE(NULL, "Password %s read", pas);

			//тут мы читаем локалвар у подконекченного игрока
			//если он не совпадает (или вообще отсутствует)
			//то мы сразу же кикаем подконекченного игрока
			_pass = ENTITY_KEYVALUE(pEntity, "_pass");
			if(strcmp(_pass, pas))
			{
				//disconnect fake admin
				LOG_CONSOLE(NULL, "Player pass - %s, required - %s", _pass, pas);
				FuncKickPlayer(pEntity, pszName, "\nYour admin password doesn't valid\n");
				fclose(file);
				RETURN_META_VALUE(MRES_IGNORED, FALSE);
			}
			
			//далее наделяем проверяем права доступа у игрока
			//и заносим его в массив админов
			acc = strtok(NULL, " ,\t\n");
			if(!acc)
			{
				LOG_CONSOLE(NULL, "Line %d, no access rights", i);
				continue;
			}
			LOG_CONSOLE(NULL, "Access rights are '%s'", acc);
			adminlist.CreateAdmin(name, acc);
		}
	}

	fclose(file);
	//end checking

	plist.AddPlayer(pEntity);

	RETURN_META_VALUE(MRES_IGNORED, FALSE);
}

void CLientDisconnect(edict_t *pEntity)
{
	const char *name;
	name = ENTITY_KEYVALUE(pEntity, "name");
	adminlist.DeleteAdmin(name);
	LOG_CONSOLE(NULL, "Player %s has left the game.", name);

	plist.DeletePlayer(pEntity);
	RETURN_META(MRES_IGNORED);
}

void ClientCommand(edict_t *pEntity)
{
	if(strmatch(CMD_ARGV(0), "admin"))
	{
		ClientAdminInfo(pEntity);
	}
	else if(strmatch(CMD_ARGV(0), "say"))
	{
		if(AdminSay(pEntity) == true)
			RETURN_META(MRES_SUPERCEDE);
	}
	RETURN_META(MRES_HANDLED);
}