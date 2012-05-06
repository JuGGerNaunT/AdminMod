#include "PluginFuncs.h"

#include <meta_api.h>

void FuncKickPlayer(edict_t *pEntity, const char *name, const char *reason)
{
	char str[MAX_CMD_LEN + 28] = "You've been kicked. Reason: ";
	strncat(str, reason, MAX_CMD_LEN);
	strncat(str, "\n", MAX_CMD_LEN);
	CLIENT_PRINTF(pEntity, print_console, str);

	sup_server_cmd("kick %s", name);
}

void FuncShowMaps(edict_t *pEntity)
{
	FILE *file = NULL;
	const char *path = "AdminMod/maps.txt";
	char fullpath[PATH_MAX];
	char line[MAX_CONF_LEN];
	char buf[MAX_CONF_LEN];

	sup_gamedir_path(path, fullpath);

	file = fopen(fullpath, "r");
	if(!file)
	{
		UTIL_LogPrintf("Unable to open map list file '%s': %s", fullpath, strerror(errno));
		if(pEntity)
			CLIENT_PRINTF(pEntity, print_console, "\nUnable to load map list\n");
		return;
	}

	for(int i = 1, j = 0; !feof(file); i++)
	{
		if(!fgets(line, sizeof(line), file))
		{
			UTIL_LogPrintf("Unable to read %d line", i);
			if(pEntity)
				CLIENT_PRINTF(pEntity, print_console, "\nUnable to load map list\n");
			break;
		}

		if(line[0] == '#' || line[0] == ';')
			continue;
		if(strnmatch(line, "//", 2))
			continue;

		j = strlen(line) - 1;
		if(line[j] == '\n')
			line[j] = '\0';

		if(!pEntity)
			LOG_CONSOLE(NULL, "%d) %s", i, line);
		else
		{
			sup_make_str(buf, sizeof(buf), "%d) %s\n", i, line);
			CLIENT_PRINTF(pEntity, print_console, buf);
		}
	}
	fclose(file);
}

bool FuncValidMap(const char *map)
{
	FILE *file = NULL;
	const char *path = "AdminMod/maps.txt";
	char fullpath[PATH_MAX];
	char line[MAX_CONF_LEN];

	sup_gamedir_path(path, fullpath);

	file = fopen(fullpath, "r");
	if(!file)
	{
		UTIL_LogPrintf("Unable to open map list file '%s': %s", fullpath, strerror(errno));
		return false;
	}

	for(int i = 1, j = 0; !feof(file); i++)
	{
		if(!fgets(line, sizeof(line), file))
		{
			UTIL_LogPrintf("Unable to read %d line", i);
			break;
		}

		if(line[0] == '#' || line[0] == ';')
			continue;
		if(strnmatch(line, "//", 2))
			continue;

		j = strlen(line) - 1;
		if(line[j] == '\n')
			line[j] = '\0';

		if(!strcmp(map, line))
		{
			fclose(file);
			return true;
		}
	}
	fclose(file);
	return false;
}

bool FuncAddToBanned(const char *name, const char *time)
{
	FILE *file = NULL;
	const char *path = "AdminMod/banned.txt";
	char fullpath[PATH_MAX];
	char line[MAX_CONF_LEN];

	sup_gamedir_path(path, fullpath);

	file = fopen(fullpath, "a+");
	if(!file)
	{
		UTIL_LogPrintf("Unable to open banned list file '%s': %s", fullpath, strerror(errno));
		return false;
	}

	STRNCPY(line, "\n", 2);
	strncat(line, name, sizeof(line));
	strncat(line, " ", 1);
	
	sup_convert_date(time, &line[strlen(line)]);
	fputs(line, file);
	fclose(file);

	return true;
}

bool FuncRemoveFromBanned(const char *name)
{
	FILE *file = NULL;
	const char *path = "AdminMod/banned.txt";
	char fullpath[PATH_MAX];
	char line[MAX_CONF_LEN];
	char *banned = NULL;
	int offset = 0, cur = 0;

	sup_gamedir_path(path, fullpath);

	file = fopen(fullpath, "r+");
	if(!file)
	{
		UTIL_LogPrintf("Unable to open banned list file '%s': %s", fullpath, strerror(errno));
		return false;
	}

	for(int i = 1, j = 0; !feof(file); i++)
	{
		if(!fgets(line, sizeof(line), file))
		{
			UTIL_LogPrintf("Unable to read %d line in file %s", i, fullpath);
			fclose(file);
			return false;
		}

		if(line[0] == '#' || line[0] == ';')
			continue;
		if(strnmatch(line, "//", 2))
			continue;

		j = strlen(line) - 1;
		if(line[j] == '\n')
			line[j] = '\0';

		offset = strlen(line);
		banned = strtok(line, " ,\t\n");
		if(!banned)
		{
			UTIL_LogPrintf("Invalid parameters at line %d in file %s", i, fullpath);
			fclose(file);
			return false;
		}

		if(strcmp(banned, name) == 0)
		{
			LOG_CONSOLE(NULL, "size of offset - %d", offset);

			while(1)
			{
				fgets(line, sizeof(line), file);
				if(feof(file))
					break;
				LOG_CONSOLE(NULL, "read line - %s", line);
				cur = strlen(line);
				fseek(file, -(cur + offset + 3), SEEK_CUR);
				fputs(line, file);
				fseek(file, offset + 2, SEEK_CUR);
			}

			LOG_CONSOLE(NULL, "read line - %s", line);
			cur = strlen(line);
			fseek(file, -(cur + offset + 2), SEEK_CUR);
			fputs(line, file);
			chsize(fileno(file), ftell(file));
			break;
		}
	}

	fclose(file);
	return true;
}

bool FuncIsBanned(const char *name, int &minutes)
{

}