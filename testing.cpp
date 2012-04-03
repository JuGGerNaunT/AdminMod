#include "testing.h"

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

	RETURN_META_VALUE(MRES_IGNORED, FALSE);
}