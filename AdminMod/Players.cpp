#include "Players.h"

PlayerList plist;

void PlayerList::AddPlayer(edict_t *pEntity)
{
	PList[p_numb].entity = pEntity;
	PList[p_numb].id = p_id;
	p_id++;
	p_numb++;
}

void PlayerList::DeletePlayer(edict_t *pEntity)
{
	for(int i = 0; i < p_numb; i++)
	{
		if(PList[i].entity == pEntity)
		{
			while(i < p_numb)
			{
				PList[i] = PList[i + 1];
				i++;
			}
			p_numb--;
		}
	}
}

int PlayerList::NextPlayer(edict_t **pEntity)
{
	if(!p_numb)
	{
		*pEntity = NULL;
		return 0;
	}

	if( !(*pEntity) )
	{
		*pEntity = PList[0].entity;
		return PList[0].id;
	}

	for(int i = 0; i < p_numb; i++)
	{
		if(PList[i].entity == *pEntity)
		{
			if((i + 1) == p_numb)
				*pEntity = NULL;
			else
			{
				*pEntity = PList[i + 1].entity;
				return PList[i + 1].id;
			}
		}
	}

	return 0;
}

edict_t* PlayerList::GetEntity(int id)
{
	for(int i = 0; i < p_numb; i++)
	{
		if(PList[i].id == id)
			return PList[i].entity;
	}

	return NULL;
}