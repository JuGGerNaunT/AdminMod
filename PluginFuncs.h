#pragma once

#include "Support.h"
#include "Admins.h"

void FuncKickPlayer(edict_t *pEntity, const char *name, const char *reason);
void FuncShowMaps(edict_t *pEntity);
bool FuncValidMap(const char *map);
bool FuncAddToBanned(const char *name, const char *time);
bool FuncRemoveFromBanned(const char *name);
bool FuncIsBanned(const char *name, int &minutes);