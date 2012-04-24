#include "Support.h"

#include <mutil.h>
#include <osdep.h>

#include <stdio.h>

extern mutil_funcs_t *gpMetaUtilFuncs;

// Turns path into a full path:
//  - if not absolute, prepends gamedir
//  - calls realpath() to collapse ".." and such
//  - calls normalize_pathname() to fix backslashes, etc
//
// Much like realpath, buffer pointed to by fullpath is assumed to be 
// able to store a string of PATH_MAX length.
void sup_gamedir_path(const char *path, char *fullpath)
{
	char buf[PATH_MAX];
	
	// Build pathname from filename, plus gamedir if relative path.
	if(is_absolute_path(path))
		STRNCPY(buf, path, sizeof(buf));
	else
	{
		const char *plugpath = gpMetaUtilFuncs->pfnGetPluginPath(PLID);
		STRNCPY(buf, plugpath, sizeof(buf));
		for(int i = strlen(buf); i != -1; i--)
		{
			if(buf[i] == '/')
			{
				buf[i + 1] = '\0';
				break;
			}
		}
		strncat(buf, path, sizeof(buf));
	}

	normalize_path(buf);
	STRNCPY(fullpath, buf, sizeof(buf));
}

void sup_server_cmd(const char *command, ...)
{
	char cmd[MAX_CMD_LEN];
	va_list	argptr;

	va_start(argptr, command);
	vsnprintf(cmd, sizeof(cmd), command, argptr);
	va_end(argptr);

	strncat(cmd, "\n", 1);

	SERVER_COMMAND(cmd);
}

