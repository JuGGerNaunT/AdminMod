#include "MyHandles.h"
#include "MyPlugin.h"
#include "Support.h"

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
	LOG_CONSOLE(PLID, "Connected:");
	LOG_CONSOLE(PLID, pszName);
	LOG_CONSOLE(PLID, pszAddress);
	LOG_CONSOLE(PLID, szRejectReason);

	int index;
	index = g_engfuncs.pfnIndexOfEdict(pEntity);
	LOG_CONSOLE(PLID, "Index os entity - %d", index);
	players_id[0] = index;

	//Checking is that admin or not
	FILE *file = NULL;
	const char *path = "AdminMod/admins.txt";
	char fullpath[PATH_MAX];
	char line[MAX_CONF_LEN];
	char *name, *pas;
	name = pas = NULL;
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

			//тут мы читаем локалвар у подконекченного игрока
			//если он не совпадает (или вообще отсутствует)
			//то мы сразу же кикием подконекченного игрока
			/*some code*/
		
			//authentication successfully done
			//далее наделяем проверяем права доступа у игрока
			//и заносим его в массив админов
		}
	}

	fclose(file);
	//end checking

	RETURN_META_VALUE(MRES_IGNORED, FALSE);
}