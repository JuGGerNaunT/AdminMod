#pragma warning(disable : 4996)
#include "PluginCmds.h"
#include "Support.h"
#include "PluginFuncs.h"
#include "Players.h"

#include <meta_api.h>
#include <eiface.h>
#include <support_meta.h>

int players_id[32];
hudtextparms_t g_hudset;

void PluginInit()
{
	//Инициализация всего плагина
	REG_SVR_COMMAND("admin", AdminInfo);

	/*g_hudset.a1 = 0;
	g_hudset.a2 = 0;
	g_hudset.r2 = 255;
	g_hudset.g2 = 255;
	g_hudset.b2 = 250;
	g_hudset.r1 = 0;
	g_hudset.g1 = 255;
	g_hudset.b1 = 0;
	g_hudset.x = -1.f;
	g_hudset.y = 1.f;
	g_hudset.effect = 0;
	g_hudset.fxTime = 6.f;
	g_hudset.holdTime = 6.;
	g_hudset.fadeinTime = 0.5f;
	g_hudset.fadeoutTime = 0.15f;
	g_hudset.channel = -1;*/
}

bool AdminSay(edict_t *pEntity)
{
	char msg[190];
	int numb = CMD_ARGC(), cl = 1;

	if(sup_is_admin(pEntity) == false)
		return false;

	msg[0] = '\0';
	sup_make_str(msg, sizeof(msg), "%cAdmin %s: ", 2, sup_get_name(pEntity));
	while(--numb)
		strncat(msg, CMD_ARGV(cl++), sizeof(msg));
	strncat(msg, "\n", 1);
	
	MESSAGE_BEGIN( MSG_BROADCAST, GET_USER_MSG_ID (PLID, "SayText", NULL));
	WRITE_BYTE( 0 );
	WRITE_STRING( msg );
	MESSAGE_END();

	g_engfuncs.pfnServerPrint( msg );
	msg[strlen(msg) - 1] = '\0';
	UTIL_LogPrintf( "\"%s<%i><%s><%s>\" %s \"%s\"\n", 
		STRING( pEntity->v.netname ), 
		GETPLAYERUSERID( pEntity ),
		GETPLAYERAUTHID( pEntity ),
		g_engfuncs.pfnInfoKeyValue( g_engfuncs.pfnGetInfoKeyBuffer( pEntity ), "model" ),
		"say",
		msg);

	return true;
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
		ShowAdmins();
	else if(!strcasecmp(cmd, "plist"))
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
		LOG_CONSOLE(NULL, "Unrecognized command.");
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
			else if(!strcasecmp(cmd, "plist"))
				ClientShowPlayers(pEntity);
			else if(!strcasecmp(cmd, "pm"))
				ClientSendPersonal(pEntity);
			else
				CLIENT_PRINTF(pEntity, print_console, "Unrecognized command.\n");
		}
	}
}

void ShowList()
{
	LOG_CONSOLE(NULL, "usage: admin <command> [<arguments>]");
	LOG_CONSOLE(NULL, "available commands:");
	LOG_CONSOLE(NULL, "   msg		- send message to the center of screen");
	LOG_CONSOLE(NULL, "   kick		- kick player");
	LOG_CONSOLE(NULL, "   log		- enable logging");
	LOG_CONSOLE(NULL, "   list		- show list of admins");
	LOG_CONSOLE(NULL, "   plist		- show list of players");
	LOG_CONSOLE(NULL, "   pm		- sent personal message to player");
	LOG_CONSOLE(NULL, "   map		- change current map");
	LOG_CONSOLE(NULL, "   ban		- ban player");
	LOG_CONSOLE(NULL, "   unban	- unban player");
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
		CLIENT_PRINTF(pEntity, print_console, "   plist		- show list of players\n");
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
		else LOG_CONSOLE(NULL, "sage: admin msg <text>");
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
	if(!pEntity)
		FuncPrintfLog("[SERVER] - {MSG} %s", str);
	else FuncPrintfLog("[%s] - {MSG} %s", sup_get_name(pEntity), str);
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
		LOG_CONSOLE(NULL, "usage: admin kick \"<player>\"|<#ID> [<reason>]");
		return;
	}

	name = CMD_ARGV(2);
	id = sup_check_numb(name);
	if(id != -1)
	{
		player = plist.GetEntity(id);
	}
	else
	{
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
	}

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

	if(sup_have_rights(player, GEN))
	{
		if(pEntity)
			CLIENT_PRINTF(pEntity, print_console, "You cant kick general admin\n");
		else LOG_CONSOLE(NULL, "You cant kick general admin");
		return;
	}

	FuncKickPlayer(player, name, str);
	if(!pEntity)
		FuncPrintfLog("[SERVER] - {KICK} %s cause <%s>", name, str);
	else FuncPrintfLog("[%s] - {KICK} %s cause <%s>", sup_get_name(player), name, str);
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
			CLIENT_PRINTF(pEntity, print_console, "usage: admin kick \"<player>\"|<#ID> [<reason>]\n");
			return;
		}

		KickPlayer(pEntity);
	}
}

bool EnableLog(edict_t *pEntity)
{
	const char *cmd;
	USHORT numb = 0;

	numb = CMD_ARGC();
	if(numb < 3)
	{
		LOG_CONSOLE(NULL, "usage: admin log <parameter>");
		LOG_CONSOLE(NULL, "parameters:");
		LOG_CONSOLE(NULL, "   0 - disable logging");
		LOG_CONSOLE(NULL, "   1 - only logging");
		LOG_CONSOLE(NULL, "   2 - logging + developer mode");
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
		LOG_CONSOLE(NULL, "unknown mode");
		LOG_CONSOLE(NULL, "print 'admin log' for log command info");
		return false;
	}
	if(!pEntity)
		FuncPrintfLog("[SERVER] - {LOG} switch to %c", cmd[0]);
	else FuncPrintfLog("[%s] - {LOG} switch to %c", sup_get_name(pEntity), cmd[0]);
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

		if(EnableLog(pEntity) == false)
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

	LOG_CONSOLE(NULL, "Admins are:");
	for(int j = 0; j < MAX_PLAYERS; j++)
	{
		adminlist.NextAdmin(&name);
		if(!name)
			break;

		LOG_CONSOLE(NULL, "%d) %s", i, name);
		i++;
	}
	if(i == 1)
		LOG_CONSOLE(NULL, "No online admins");
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

	LOG_CONSOLE(NULL, "Players are:");
	for(int j = 0; j < MAX_PLAYERS; j++)
	{
		id = plist.NextPlayer(&pEntity);
		if(!pEntity)
			break;

		LOG_CONSOLE(NULL, "%d) %s - #%d", i, sup_get_name(pEntity), id);
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
	int id;

	numb = CMD_ARGC();
	if(numb < 4)
	{
		LOG_CONSOLE(NULL, "usage: admin pm \"<name>\"|<#ID> <text>");
	}

	str = CMD_ARGV(2);
	id = sup_check_numb(str);
	if(id != -1)
	{
		pEntity = plist.GetEntity(id);
	}
	else
	{
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
	}
	
	sup_make_str(word, sizeof(word), "From SERVER: ");
	length = strlen(word);
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

	{
		if (GET_USER_MSG_ID (PLID, "SayText", NULL) <= 0)
			REG_USER_MSG ("SayText", -1);
		
		g_hudset.channel = 1;
		//UTIL_HudMessage(0, g_hudset, word);
		MESSAGE_BEGIN( MSG_ONE, GET_USER_MSG_ID (PLID, "SayText", NULL), NULL, pEntity );
		WRITE_BYTE( 0 );
		WRITE_STRING( word );
		MESSAGE_END();
	}

	FuncPrintfLog("[SERVER] - {PM} to [%s] - %s", sup_get_name(pEntity), word);
}

void ClientSendPersonal(edict_t *pEdict)
{
	const char *str;
	USHORT numb, length = 0;
	edict_t *pEntity = NULL;
	char word[MAX_CMD_LEN];
	int id;

	numb = CMD_ARGC();
	if(numb < 4)
	{
		CLIENT_PRINTF(pEdict, print_console, "usage: admin pm \"<name>\"|<#ID <text>");
	}

	str = CMD_ARGV(2);
	id = sup_check_numb(str);
	if(id != -1)
	{
		pEntity = plist.GetEntity(id);
	}
	else
	{
		for(int i = 0; i < MAX_PLAYERS; i++)
		{
			plist.NextPlayer(&pEntity);
			if(!pEntity)
			{
				CLIENT_PRINTF(pEdict, print_console, "No such player");
				return;
			}

			if(strcasecmp(str, sup_get_name(pEntity)) == 0)
				break;
		}
	}

	sup_make_str(word, sizeof(word), "From %s: ", sup_get_name(pEdict));
	length = strlen(word);
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

	{
		if (GET_USER_MSG_ID (PLID, "SayText", NULL) <= 0)
			REG_USER_MSG ("SayText", -1);

		MESSAGE_BEGIN( MSG_ONE, GET_USER_MSG_ID (PLID, "SayText", NULL), NULL, pEntity );
		WRITE_BYTE( ENTINDEX(pEdict) );
		WRITE_STRING( word );
		MESSAGE_END();
	}

	FuncPrintfLog("[%s] - {PM} to [%s] - %s", sup_get_name(pEdict), sup_get_name(pEntity), word);
}

void ChangeMap()
{
	USHORT numb;
	const char *map;
	int id;

	numb = CMD_ARGC();
	if(numb < 3)
	{
		LOG_CONSOLE(NULL, "usage: admin map <map_name>|<#ID>");
		LOG_CONSOLE(NULL, "Available maps:");
		FuncShowMaps(NULL);
		return;
	}

	map = CMD_ARGV(2);
	id = sup_check_numb(map);
	if(id != -1)
	{
		map = FuncGetMap(id);
	}
	if(FuncValidMap(map) == false)
	{
		LOG_CONSOLE(NULL, "No such map. Type 'admin map' to see list of available maps");
		return;
	}

	sup_server_cmd("changelevel %s", map);
	FuncPrintfLog("[SERVER] - {MAP} %s", map);
}

void ClientChangeMap(edict_t *pEntity)
{
	USHORT numb;
	const char *map;
	int id;

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
			CLIENT_PRINTF(pEntity, print_console, "usage: admin map <map_name>|<#ID>\n");
			CLIENT_PRINTF(pEntity, print_console, "Available maps:\n");
			FuncShowMaps(pEntity);
			return;
		}

		map = CMD_ARGV(2);
		id = sup_check_numb(map);
		if(id != -1)
		{
			map = FuncGetMap(id);
		}
		if(FuncValidMap(map) == false)
		{
			CLIENT_PRINTF(pEntity, print_console, "No such map. Type 'admin map' to see list of available maps\n");
			return;
		}

		sup_server_cmd("changelevel %s", map);
		FuncPrintfLog("[%s] - {MAP} %s", sup_get_name(pEntity), map);
	}
}

void BanPlayer()
{
	USHORT numb;
	const char *name, *minutes;
	int id;
	edict_t *pEntity = NULL;
	const char *cmd;
	char str[MAX_CMD_LEN];
	int length = 0;

	numb = CMD_ARGC();
	if(numb < 4)
	{
		LOG_CONSOLE(NULL, "usage: admin ban \"<name>\"|<#ID> <minutes> [<reason>]");
		LOG_CONSOLE(NULL, "use 0 minutes for permanent ban");
		return;
	}

	name = CMD_ARGV(2);
	minutes = CMD_ARGV(3);

	id = sup_check_numb(name);
	if(id != -1)
	{
		pEntity = plist.GetEntity(id);
		name = sup_get_name(pEntity);
		if(!pEntity || !name)
			return;
	}

	for(int i = 4; i < numb; i++)
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

	if(FuncAddToBanned(name, minutes) == false)
	{
		LOG_CONSOLE(NULL, "Unable to ban player");
		return;
	}
	else
	{
		if(pEntity)
			FuncKickPlayer(pEntity, name, "\nYou've been banned\n");
		FuncPrintfLog("[SERVER] - {BAN} %s for %s minutes cause <%s>", name, minutes, str);
	}
}

void ClientBanPlayer(edict_t *pEntity)
{
	USHORT numb;
	const char *name = NULL, *minutes = NULL;
	int id;
	edict_t *player = NULL;
	const char *cmd;
	char str[MAX_CMD_LEN];
	int length = 0;

	if(sup_is_admin(pEntity) == true)
	{
		numb = CMD_ARGC();
		if(numb < 4)
		{
			CLIENT_PRINTF(pEntity, print_console, "usage: admin ban \"<name>\"|<#ID> <minutes> [<reason>]\n");
			CLIENT_PRINTF(pEntity, print_console, "use 0 minutes for permanent ban\n");
			return;
		}

		if(sup_have_rights(pEntity, BAN) == false)
		{
			CLIENT_PRINTF(pEntity, print_console, "You have few right for this command\n");
			return;
		}

		name = CMD_ARGV(2);
		minutes = CMD_ARGV(3);

		id = sup_check_numb(name);
		if(id != -1)
		{
			player = plist.GetEntity(id);
			name = sup_get_name(player);
			if(!player || !name)
				return;
		}

		for(int i = 4; i < numb; i++)
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

		if(FuncAddToBanned(name, minutes) == false)
		{
			CLIENT_PRINTF(pEntity, print_console, "Unable to ban player\n");
			return;
		}
		else
		{
			if(player)
				FuncKickPlayer(player, name, "\nYou've been banned\n");
			FuncPrintfLog("[%s] - {BAN} %s for %s minutes cause <%s>", sup_get_name(pEntity),
			name, minutes, str);
		}
	}
}

void UnbanPlayer()
{
	USHORT numb;
	const char *name;
	int id;
	edict_t *pEntity;

	numb = CMD_ARGC();
	if(numb < 3)
	{
		LOG_CONSOLE(NULL, "usage: admin unban \"<name>\"");
		return;
	}

	name = CMD_ARGV(2);
	id = sup_check_numb(name);
	if(id != -1)
	{
		pEntity = plist.GetEntity(id);
		name = sup_get_name(pEntity);
	}

	if(FuncRemoveFromBanned(name) == false)
	{
		LOG_CONSOLE(NULL, "Unable to unban player");
		return;
	}
	else FuncPrintfLog("[SERVER] - {UNBAN} %s", name);
}

void ClientUnbanPlayer(edict_t *pEntity)
{
	USHORT numb;
	const char *name;
	int id;
	edict_t *player;

	if(sup_is_admin(pEntity) == true)
	{
		numb = CMD_ARGC();
		if(numb < 3)
		{
			CLIENT_PRINTF(pEntity, print_console, "usage: admin unban \"<name>\"");
			return;
		}

		if(sup_have_rights(pEntity, BAN) == false)
		{
			CLIENT_PRINTF(pEntity, print_console, "You have few right for this command\n");
			return;
		}

		name = CMD_ARGV(2);
		id = sup_check_numb(name);
		if(id != -1)
		{
			player = plist.GetEntity(id);
			name = sup_get_name(player);
		}

		if(FuncRemoveFromBanned(name) == false)
		{
			CLIENT_PRINTF(pEntity, print_console, "Unable to unban player");
			return;
		}
		else FuncPrintfLog("[SERVER] - {UNBAN} %s", name);
	}
}