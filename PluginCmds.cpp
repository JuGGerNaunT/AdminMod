#pragma warning(disable : 4996)
#include "PluginCmds.h"
#include "Support.h"
#include "PluginFuncs.h"
#include "Players.h"

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
	{
		ShowList();
		return;
	}

	if(!strcasecmp(cmd, "msg"))
		ClientSndMessage(NULL);
	else if(!strcasecmp(cmd, "kick"))
		KickPlayer(NULL);
	else if(!strcasecmp(cmd, "log"))
		EnableLog();
	else if(!strcasecmp(cmd, "list"))
		ShowPlayers();
	else if(!strcasecmp(cmd, "pm"))
		SendPersonal();
	else if(!strcasecmp(cmd, "map"))
		ChangeMap();
	else if(!strcasecmp(cmd, "ban"))
		BanPlayer();
	else if(!strcasecmp(cmd, "unban"))
		UnbanPlayer();
	else
		LOG_CONSOLE(PLID, "Unrecognized command.");
}

void ClientAdminInfo(edict_t *pEntity)
{
	const char *cmd;
	cmd = CMD_ARGV(1);
	
	if(CMD_ARGC() == 1)
	{
		ClientShowList(pEntity);
		return;
	}

	//common commands
	if(!strcasecmp(cmd, "pm"))
		ClientSendPersonal(pEntity);
	else
	{
		if(sup_is_admin(pEntity))
		{//admin commands
			if(!strcasecmp(cmd, "msg"))
				ClientSndMessage(pEntity);
			else if(!strcasecmp(cmd, "kick"))
				ClientKickPlayer(pEntity);
			else if(!strcasecmp(cmd, "log"))
				ClientEnableLog(pEntity);
			else if(!strcasecmp(cmd, "list"))
				ClientShowPlayers(pEntity);
			else if(!strcasecmp(cmd, "pm"))
				ClientSendPersonal(pEntity);
			else if(!strcasecmp(cmd, "map"))
				ClientChangeMap(pEntity);
			else if(!strcasecmp(cmd, "ban"))
				ClientBanPlayer(pEntity);
			else if(!strcasecmp(cmd, "unban"))
				ClientUnbanPlayer(pEntity);
			else
				CLIENT_PRINTF(pEntity, print_console, "Unrecognized command.\n");
		}
		else
		{//ordinary mortal commands
			if(!strcasecmp(cmd, "list"))
				ClientShowAdmins(pEntity);
			else if(!strcasecmp(cmd, "pm"))
				ClientSendPersonal(pEntity);
			else
				CLIENT_PRINTF(pEntity, print_console, "Unrecognized command.\n");
		}
	}
}

void ShowList()
{
	LOG_CONSOLE(PLID, "usage: admin <command> [<arguments>]");
	LOG_CONSOLE(PLID, "available commands:");
	LOG_CONSOLE(PLID, "   msg		- send message to the center of screen");
	LOG_CONSOLE(PLID, "   kick		- kick player");
	LOG_CONSOLE(PLID, "   log		- enable logging");
	LOG_CONSOLE(PLID, "   list		- show list of players");
	LOG_CONSOLE(PLID, "   pm		- sent personal message to player");
	LOG_CONSOLE(PLID, "   map		- change current map");
	LOG_CONSOLE(PLID, "   ban		- ban player");
	LOG_CONSOLE(PLID, "   unban	- unban player");
}

void ClientShowList(edict_t *pEntity)
{
	if(sup_is_admin(pEntity))
	{//admin commands
		CLIENT_PRINTF(pEntity, print_console, "usage: admin <command> [<arguments>]\n");
		CLIENT_PRINTF(pEntity, print_console, "available commands:\n");
		CLIENT_PRINTF(pEntity, print_console, "   msg		- send message to the center of screen\n");
		CLIENT_PRINTF(pEntity, print_console, "   kick		- kick player\n");
		CLIENT_PRINTF(pEntity, print_console, "   log		- enable logging\n");
		CLIENT_PRINTF(pEntity, print_console, "   list		- show list of players\n");
		CLIENT_PRINTF(pEntity, print_console, "   map		- change current map\n");
		CLIENT_PRINTF(pEntity, print_console, "   ban		- ban player\n");
		CLIENT_PRINTF(pEntity, print_console, "   unban	- unban player\n");
	}
	else
	{//ordinary mortal commands
		CLIENT_PRINTF(pEntity, print_console, "usage: admin <command> [<arguments>]\n");
		CLIENT_PRINTF(pEntity, print_console, "available commands:\n");
		CLIENT_PRINTF(pEntity, print_console, "   list		- show list of admins\n");
	}
	//common commands
	CLIENT_PRINTF(pEntity, print_console, "   pm		- sent personal message to player\n");
}

void ClientSndMessage(edict_t *pEntity)
{
	const char *cmd;
	USHORT numb = 0, length = 0;
	char str[MAX_CMD_LEN];

	numb = CMD_ARGC();
	if(numb < 3)
	{
		if(pEntity)
			CLIENT_PRINTF(pEntity, print_console, "usage: admin msg <text>\n");
		return;
	}

	for(int i = 2; i < numb; i++)
	{
		cmd = CMD_ARGV(i);
		if(!STRNCPY((str + length), cmd, sizeof(str) - length))
		{
			length = MAX_CMD_LEN;
			break;
		}
		length += strlen(cmd);
		str[length] = ' ';
		length++;
	}
	str[length] = '\0';

	CENTER_SAY(PLID, str);
}

void KickPlayer(edict_t *pEntity)
{
	const char *name, *cmd;
	USHORT numb = 0, length = 0;
	char str[MAX_CMD_LEN];
	edict_t *player = NULL;
	int id;

	numb = CMD_ARGC();
	if(numb < 3)
	{
		LOG_CONSOLE(PLID, "usage: admin kick <player> [<reason>]");
		return;
	}

	name = CMD_ARGV(2);
	for(int i = 3; i < numb; i++)
	{
		cmd = CMD_ARGV(i);
		if(!STRNCPY((str + length), cmd, sizeof(str) - length))
		{
			length = MAX_CMD_LEN;
			break;
		}
		length += strlen(cmd);
		str[length] = ' ';
		length++;
	}
	str[length] = '\0';

	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		id = plist.NextPlayer(&player);
		if(!player)
		{
			if(pEntity)
				CLIENT_PRINTF(pEntity, print_console, "No such player\n");
			else LOG_CONSOLE(NULL, "No such player");
			return;
		}

		if(!strcasecmp(sup_get_name(player), name))
			break;
	}

	if(sup_have_rights(player, GEN))
	{
		if(pEntity)
			CLIENT_PRINTF(pEntity, print_console, "You cant kick general admin\n");
		else LOG_CONSOLE(NULL, "You cant kick general admin");
		return;
	}

	FuncKickPlayer(player, name, str);
}

void ClientKickPlayer(edict_t *pEntity)
{
	USHORT numb = 0;

	if(sup_is_admin(pEntity))
	{
		if(sup_have_rights(pEntity, KICK) == false)
		{
			CLIENT_PRINTF(pEntity, print_console, "You have few right for this command\n");
			return;
		}

		numb = CMD_ARGC();
		if(numb < 3)
		{
			CLIENT_PRINTF(pEntity, print_console, "usage: admin kick <player> [<reason>]\n");
			return;
		}

		KickPlayer(pEntity);
	}
}

bool EnableLog()
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
		return false;
	}

	cmd = CMD_ARGV(2);
	switch(cmd[0])
	{
	case '0':
		SERVER_COMMAND("developer 0\n");
		SERVER_COMMAND("log off\n");
		break;
	case '1':
		SERVER_COMMAND("developer 0\n");
		SERVER_COMMAND("log on\n");
		break;
	case '2':
		SERVER_COMMAND("developer 1\n");
		SERVER_COMMAND("log on\n");
		break;
	default:
		LOG_CONSOLE(PLID, "unknown mode");
		LOG_CONSOLE(PLID, "print 'admin log' for log command info");
		return false;
	}
	return true;
}

void ClientEnableLog(edict_t *pEntity)
{
	USHORT numb = 0;

	if(sup_is_admin(pEntity))
	{
		numb = CMD_ARGC();
		if(numb < 3)
		{
			CLIENT_PRINTF(pEntity, print_console, "usage: admin log <parameter>\n");
			CLIENT_PRINTF(pEntity, print_console, "parameters:\n");
			CLIENT_PRINTF(pEntity, print_console, "   0 - disable logging\n");
			CLIENT_PRINTF(pEntity, print_console, "   1 - only logging\n");
			CLIENT_PRINTF(pEntity, print_console, "   2 - logging + developer mode\n");
			return;
		}

		if(EnableLog() == false)
		{
			CLIENT_PRINTF(pEntity, print_console, "unknown mode\n");
			CLIENT_PRINTF(pEntity, print_console, "print 'admin log' for log command info\n");
		}
	}
}

void ShowAdmins()
{
	const char *name = NULL;
	int i = 1;

	LOG_CONSOLE(PLID, "Admins are:");
	for(int j = 0; j < MAX_PLAYERS; j++)
	{
		adminlist.NextAdmin(&name);
		if(!name)
			break;

		LOG_CONSOLE(PLID, "%d) %s", i, name);
		i++;
	}
	if(i == 1)
		LOG_CONSOLE(PLID, "No online admins");
}

void ClientShowAdmins(edict_t *pEntity)
{
	const char *name = NULL;
	char str[MAX_NAME + 4];
	int i = 1;

	CLIENT_PRINTF(pEntity, print_console, "Admins are:\n");
	for(int j = 0; j < MAX_PLAYERS; j++)
	{
		adminlist.NextAdmin(&name);
		if(!name)
			break;

		sup_make_str(str, sizeof(str), "%d) %s\n", i, name);
		CLIENT_PRINTF(pEntity, print_console, str);
		i++;
	}
	if(i == 1)
		CLIENT_PRINTF(pEntity, print_console, "No online admins\n");
}

void ShowPlayers()
{
	edict_t *pEntity = NULL;
	int id, i = 1;

	LOG_CONSOLE(PLID, "Players are:");
	for(int j = 0; j < MAX_PLAYERS; j++)
	{
		id = plist.NextPlayer(&pEntity);
		if(!pEntity)
			break;

		LOG_CONSOLE(PLID, "%d) %s - #%d", i, sup_get_name(pEntity), id);
		i++;
	}
}

void ClientShowPlayers(edict_t *pEdict)
{
	edict_t *pEntity = NULL;
	int id;
	char str[MAX_NAME + 12];

	CLIENT_PRINTF(pEdict, print_console, "Players are:\n");
	for(int j = 0; j < MAX_PLAYERS; j++)
	{
		id = plist.NextPlayer(&pEntity);
		if(!pEntity)
			break;

		sup_make_str(str, sizeof(str), "%d) %s - #%d\n", j + 1, sup_get_name(pEntity), id);
		CLIENT_PRINTF(pEdict, print_console, str);
	}
}

void SendPersonal()
{
	const char *str;
	USHORT numb, length = 0;
	edict_t *pEntity = NULL;
	char word[MAX_CMD_LEN];

	numb = CMD_ARGC();
	if(numb < 4)
	{
		LOG_CONSOLE(NULL, "usage: admin pm <name> <text>");
	}

	str = CMD_ARGV(2);
	for(int i = 0; i < MAX_PLAYERS; i++)
	{
		plist.NextPlayer(&pEntity);
		if(!pEntity)
		{
			LOG_CONSOLE(NULL, "No such player");
			return;
		}

		if(strcasecmp(str, sup_get_name(pEntity)) == 0)
			break;
	}

	for(int i = 3; i < numb; i++)
	{
		str = CMD_ARGV(i);
		if(!STRNCPY((word + length), str, sizeof(word) - length))
		{
			length = MAX_CMD_LEN;
			break;
		}
		length += strlen(str);
		word[length] = ' ';
		length++;
	}
	word[length++] = '\n';
	word[length] = '\0';

	CLIENT_PRINTF(pEntity, print_chat, word);
}

void ClientSendPersonal(edict_t *pEntity)
{

}

void ChangeMap()
{
	USHORT numb;
	const char *map;

	numb = CMD_ARGC();
	if(numb < 3)
	{
		LOG_CONSOLE(NULL, "usage: admin map <map_name>");
		LOG_CONSOLE(NULL, "Available maps:");
		FuncShowMaps(NULL);
		return;
	}

	map = CMD_ARGV(2);
	if(FuncValidMap(map) == false)
	{
		LOG_CONSOLE(NULL, "No such map. Type 'admin map' to see list of available maps");
		return;
	}

	sup_server_cmd("changelevel %s", map);
}

void ClientChangeMap(edict_t *pEntity)
{
	USHORT numb;
	const char *map;

	if(sup_is_admin(pEntity))
	{
		if(sup_have_rights(pEntity, MAP) == false)
		{
			CLIENT_PRINTF(pEntity, print_console, "You have few right for this command\n");
			return;
		}

		numb = CMD_ARGC();
		if(numb < 3)
		{
			CLIENT_PRINTF(pEntity, print_console, "usage: admin map <map_name>\n");
			CLIENT_PRINTF(pEntity, print_console, "Available maps:\n");
			FuncShowMaps(pEntity);
			return;
		}

		map = CMD_ARGV(2);
		if(FuncValidMap(map) == false)
		{
			CLIENT_PRINTF(pEntity, print_console, "No such map. Type 'admin map' to see list of available maps\n");
			return;
		}

		sup_server_cmd("changelevel %s", map);
	}
}

void BanPlayer()
{
	USHORT numb;

	numb = CMD_ARGC();
	if(numb < 4)
	{
		LOG_CONSOLE(NULL, "usage: admin ban <name> <minutes> [<reason>]");
		LOG_CONSOLE(NULL, "use 0 minutes for permanent ban");
		return;
	}

	if(FuncAddToBanned(CMD_ARGV(2), CMD_ARGV(3)) == false)
	{
		LOG_CONSOLE(NULL, "Unable to ban player");
		return;
	}
}

void ClientBanPlayer(edict_t *pEntity)
{
	USHORT numb;

	numb = CMD_ARGC();
	if(numb < 4)
	{
		CLIENT_PRINTF(pEntity, print_console, "usage: admin ban <name> <minutes> [<reason>]\n");
		CLIENT_PRINTF(pEntity, print_console, "use 0 minutes for permanent ban\n");
		return;
	}

	if(FuncAddToBanned(CMD_ARGV(2), CMD_ARGV(3)) == false)
	{
		CLIENT_PRINTF(pEntity, print_console, "Unable to ban player\n");
		return;
	}
}

void UnbanPlayer()
{
	USHORT numb;

	numb = CMD_ARGC();
	if(numb < 3)
	{
		LOG_CONSOLE(NULL, "usage: admin unban <name>");
		return;
	}

	if(FuncRemoveFromBanned(CMD_ARGV(2)) == false)
	{
		LOG_CONSOLE(NULL, "Unable to unban player");
		return;
	}
}

void ClientUnbanPlayer(edict_t *pEntity)
{

}