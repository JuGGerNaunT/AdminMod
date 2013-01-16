#pragma once

#include <extdll.h>

extern int players_id[32];

void PluginInit();

bool AdminSay(edict_t *pEntity);

void AdminInfo();
void ClientAdminInfo(edict_t *pEntity);

void ShowList();
void ClientShowList(edict_t *pEntity);

void ClientSndMessage(edict_t *pEntity);

void KickPlayer(edict_t *pEntity);
void ClientKickPlayer(edict_t *pEntity);

bool EnableLog(edict_t *pEntity = NULL);
void ClientEnableLog(edict_t *pEntity);

void ShowAdmins();
void ClientShowAdmins(edict_t *pEntity);

void ShowPlayers();
void ClientShowPlayers(edict_t *pEntity);

void SendPersonal();
void ClientSendPersonal(edict_t *pEntity);

void ChangeMap();
void ClientChangeMap(edict_t *pEntity);

void BanPlayer();
void ClientBanPlayer(edict_t *pEntity);

void UnbanPlayer();
void ClientUnbanPlayer(edict_t *pEntity);