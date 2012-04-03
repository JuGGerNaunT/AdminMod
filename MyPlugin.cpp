#include "MyPlugin.h"

#include <extdll.h>
#include <meta_api.h>

void PluginInit()
{
	//Инициализация всего плагина
	REG_SVR_COMMAND("admin", AdminInfo);
}

void AdminInfo()
{
	const char *cmd;
	cmd = CMD_ARGV(1);
	if(cmd)
		LOG_CONSOLE(PLID, "Unrecognised command.");
}