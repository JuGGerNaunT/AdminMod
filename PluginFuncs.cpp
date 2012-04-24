#include "PluginFuncs.h"

void FuncKickPlayer(edict_t *pEntity, const char *name, const char *reason)
{
	char str[MAX_CMD_LEN + 20] = "You've been kicked.";
	strncat(str, reason, MAX_CMD_LEN);
	CLIENT_PRINTF(pEntity, print_console, str);

	sup_server_cmd("kick %s", name);
}