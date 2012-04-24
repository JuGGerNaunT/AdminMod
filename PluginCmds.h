#pragma once

extern int players_id[32];

void PluginInit();
void AdminInfo();
void ClientAdminInfo(edict_t *pEntity);
void ShowList();
void SndMessage();
void KickPlayer();
void EnableLog();