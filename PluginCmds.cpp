#pragma warning(disable : 4996)
#include "PluginCmds.h"
#include "Support.h"
#include "PluginFuncs.h"

#include <extdll.h>
#include <meta_api.h>
#include <eiface.h>
#include <support_meta.h>

int players_id[32];

void PluginInit()
{
	//Инициализация всего плагина
	REG_SVR_COMMAND("admin", AdminInfo);
}

void AdminInfo()
{
	const char *cmd;
	cmd = CMD_ARGV(1);

	if(CMD_ARGC() == 1)
		ShowList();

	if(!strcasecmp(cmd, "msg"))
		SndMessage();
	else if(!strcasecmp(cmd, "kick"))
		KickPlayer();
	else if(!strcasecmp(cmd, "log"))
		EnableLog();
	else
		LOG_CONSOLE(PLID, "Unrecognized command.");
}

void ShowList()
{
	LOG_CONSOLE(PLID, "usage: admin <command> [<arguments>]");
	LOG_CONSOLE(PLID, "available commands:");
	LOG_CONSOLE(PLID, "   msg		- send message to the center of screen");
	LOG_CONSOLE(PLID, "   kick		- kick player");
	LOG_CONSOLE(PLID, "   log		- enable logging");
}

void SndMessage()
{
	const char *cmd;
	USHORT numb = 0, length = 0;
	char str[100];

	numb = CMD_ARGC();
	if(numb < 3)
	{
		LOG_CONSOLE(PLID, "usage: admin msg <text>");
		return;
	}

	for(int i = 2; i < numb; i++)
	{
		cmd = CMD_ARGV(i);
		if(!STRNCPY((str + length), cmd, sizeof(str) - length))
		{
			length = 99;
			break;
		}
		length += strlen(cmd);
		str[length] = ' ';
		length++;
	}
	str[length] = '\0';

	CENTER_SAY(PLID, str);
}

void KickPlayer()
{
	const char *cmd;

	cmd = CMD_ARGV(2);
	//CENTER_SAY(PLID, cmd);
	edict_t *player;
	player = INDEXENT(players_id[0]);
	FuncKickPlayer(player, cmd, "reason");
}

void EnableLog()
{
	const char *cmd;
	USHORT numb = 0;

	numb = CMD_ARGC();
	if(numb < 3)
	{
		LOG_CONSOLE(PLID, "usage: admin log <parameter>");
		LOG_CONSOLE(PLID, "parameters:");
		LOG_CONSOLE(PLID, "   0 - disable logging");
		LOG_CONSOLE(PLID, "   1 - only logging");
		LOG_CONSOLE(PLID, "   2 - logging + developer mode");
		return;
	}

	edict_t *server=INDEXENT(0);
	cmd = CMD_ARGV(2);
	switch(cmd[0])
	{
	case '0':
		//g_engfuncs.pfnServerCommand("developer");
		//g_engfuncs.pfnServerExecute();
		break;
	case '1':
		break;
	case '2':
		break;
	default:
		LOG_CONSOLE(PLID, "unknown mode");
		LOG_CONSOLE(PLID, "print 'admin log' for log command info");
	}
}