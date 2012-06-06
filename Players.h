#pragma once

#include <extdll.h>
#include <mplayer.h>

class PlayerList;
extern PlayerList plist;

class PlayerList
{
	struct player
	{
		edict_t *entity;
		int id;
	};

	player PList[MAX_PLAYERS];
	BYTE p_numb;
	int p_id;

public:
	PlayerList():
	p_numb(0),
	p_id(1)
	{}

	void AddPlayer(edict_t *pEntity);
	void DeletePlayer(edict_t *pEntity);
	int NextPlayer(edict_t **pEntity);
	edict_t* GetEntity(int id);
};