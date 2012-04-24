#pragma once
#include <extdll.h>

qboolean ClientConnect(edict_t *pEntity, const char *pszName, const char *pszAddress, char szRejectReason[ 128 ]);
void ClientCommand(edict_t *pEdict);