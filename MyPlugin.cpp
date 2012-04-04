#include "MyPlugin.h"

#include <extdll.h>
#include <meta_api.h>
#include <eiface.h>

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

	if(!_stricmp(cmd, "msg"))
		SndMessage();
	else
		LOG_CONSOLE(PLID, "Unrecognised command.");
}

void ShowList()
{
	LOG_CONSOLE(PLID, "usage: admin <command> [<arguments>]");
	LOG_CONSOLE(PLID, "available commands:");
	LOG_CONSOLE(PLID, "   msg		- send ")
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
		if(!strcpy_s((str + length), sizeof(str) - length, cmd))
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