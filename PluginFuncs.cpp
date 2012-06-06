#include "PluginFuncs.h"

#include <meta_api.h>

#include <time.h>

void FuncKickPlayer(edict_t *pEntity, const char *name, const char *reason)
{
	char buf[MAX_NAME];
	char str[MAX_CMD_LEN + 28] = "You've been kicked. Reason: ";
	strncat(str, reason, MAX_CMD_LEN);
	strncat(str, "\n", MAX_CMD_LEN);
	STRNCPY(buf, name, sizeof(buf));
	sup_add_quotes(buf);
	CLIENT_PRINTF(pEntity, print_console, str);

	sup_server_cmd("kick %s", buf);
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
			fclose(file);
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

char* FuncGetMap(int id)
{
	FILE *file = NULL;
	const char *path = "AdminMod/maps.txt";
	char fullpath[PATH_MAX];
	static char line[36];

	sup_gamedir_path(path, fullpath);

	file = fopen(fullpath, "r");
	if(!file)
	{
		UTIL_LogPrintf("Unable to open map list file '%s': %s", fullpath, strerror(errno));
		return NULL;
	}

	for(int i = 1, j = 0; !feof(file); i++)
	{
		if(!fgets(line, sizeof(line), file))
		{
			UTIL_LogPrintf("Unable to read %d line", i);
			fclose(file);
			break;
		}

		if(line[0] == '#' || line[0] == ';')
			continue;
		if(strnmatch(line, "//", 2))
			continue;

		j++;
		if(j != id)
			continue;
		break;
	}
	
	if(line[strlen(line) - 1] == '\n')
		line[strlen(line) - 1] = '\0';
	fclose(file);
	return line;
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

	sup_make_str(line, sizeof(line), "\"%s\" ", name);
	//STRNCPY(line, "\n", 2);
	//strncat(line, name, sizeof(line));
	//strncat(line, " ", 1);
	
	sup_convert_date(time, &line[strlen(line)]);
	strncat(line, "\n", 1);
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
		banned = strtok(line, "\"");
		if(!banned)
		{
			UTIL_LogPrintf("Invalid parameters at line %d in file %s", i, fullpath);
			fclose(file);
			return false;
		}
		sup_del_quotes(banned);

		LOG_CONSOLE(NULL, "banned - %s; list name - %s", banned, name);
		if(strcmp(banned, name) == 0)
		{
			LOG_CONSOLE(NULL, "size of offset - %d", offset);

			while(1)
			{
				if(fgets(line, sizeof(line), file) == NULL)
				{
					fseek(file, -(offset + 3), SEEK_CUR);
					fputs("\n", file);
					chsize(fileno(file), ftell(file));
					fclose(file);
					return true;
				}
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
	FILE *file = NULL;
	const char *path = "AdminMod/banned.txt";
	char fullpath[PATH_MAX];
	char line[MAX_CONF_LEN];
	char *banned = NULL, *date = NULL, *cur = NULL;

	time_t clock = time(NULL);
	tm *ltm = localtime(&clock);

	sup_gamedir_path(path, fullpath);
	LOG_CONSOLE(NULL, "path - %s", fullpath);

	file = fopen(fullpath, "r");
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

		banned = strtok(line, "\"");
		if(!banned)
		{
			UTIL_LogPrintf("Invalid parameters at line %d in file %s", i, fullpath);
			fclose(file);
			return false;
		}
		sup_del_quotes(banned);

		if(strcmp(banned, name) == 0)
		{
			minutes = 0;
			date = strtok(NULL, "\n");
			for(USHORT k = 0; k < strlen(date); k++)
				if(date[k] != ' ' && date[k] != '\t')
				{
					STRNCPY(date, date + k, strlen(date));
					break;
				}
			if(date[0] == '-')
			{
				fclose(file);
				return true;
			}

			cur = strtok(date, ":");
			minutes += sup_str_to_int(cur) - ltm->tm_min;
			cur = strtok(NULL, " ");
			minutes += 60 * (sup_str_to_int(cur) - ltm->tm_hour);
			cur = strtok(NULL, ":");
			minutes += 60 * 24 * (sup_str_to_int(cur) - ltm->tm_yday);
			cur = strtok(NULL, ":");
			minutes += 60 * 24 * 30 * (sup_str_to_int(cur) - ltm->tm_year);

			fclose(file);
			if(minutes <= 0)
			{
				FuncRemoveFromBanned(banned);
				return false;
			}
			else return true;
		}
	}

	fclose(file);
	return false;
}

void FuncPrintfLog(const char *str, ...)
{
	va_list			argptr;
	static char		string[1024];

	va_start(argptr, str);
	vsnprintf(string, sizeof(string), str, argptr);
	va_end(argptr);

	FILE *file = NULL;
	int length;
	const char *path = "AdminMod/logs/";
	char fullpath[PATH_MAX];
	char line[MAX_CONF_LEN];

	time_t clock = time(NULL);
	tm *ltm = localtime(&clock);

	sup_gamedir_path(path, fullpath);

	sup_make_str(fullpath + strlen(fullpath), sizeof(fullpath), "%d-%d-%d_1.txt", ltm->tm_mday, ltm->tm_mon + 1,
		1900 + ltm->tm_year);
	sup_make_str(line, sizeof(line), "[%d-%d-%d] - [%d:%d:%d] : %s\n", ltm->tm_mday, ltm->tm_mon + 1,
		1900 + ltm->tm_year, ltm->tm_hour, ltm->tm_min, ltm->tm_sec, string);

	while(1)
	{
		file = fopen(fullpath, "a+");
		if(!file)
		{
			UTIL_LogPrintf("Unable to create log list file '%s': %s", fullpath, strerror(errno));
			return;
		}

		fseek(file, 0, SEEK_END);
		length = ftell(file);

		if(length >= MAX_FILE_LEN)
			sup_add_symb(fullpath, strlen(fullpath) - 5);
		else break;

		fclose(file);
	}

	fputs(line, file);
	fclose(file);
}