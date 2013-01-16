#include "Support.h"

#include <mutil.h>
#include <osdep.h>

#include <stdio.h>
#include <time.h>

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

void sup_make_str(char *string, int size, char *fmt, ...)
{
	va_list			argptr;

	va_start(argptr, fmt);
	vsnprintf(string, size, fmt, argptr);
	va_end(argptr);
}

void sup_convert_date(const char *time_var, char *date)
{
	int time_int = 0, buf = 0;
	time_t td;
	tm *lt = NULL;

	td = time(NULL);
	lt = localtime(&td);

	time_int = sup_str_to_int(time_var);
	if(!time_int)
	{
		STRNCPY(date, "-", 2);
		return;
	}

	for(int i = 0, j = 0; i < 4; i++)
	{
		switch(i)
		{
		case 0:
			j = lt->tm_min;
			j += time_int % 60;
			time_int /= 60;
			if(j > 59)
			{
				j -= 59;
				time_int++;
			}

			strncat(date, sup_int_to_str(j), 2);
			strncat(date, ":", 1);
			break;
		case 1:
			j = lt->tm_hour;
			j += time_int % 24;
			time_int /= 24;
			if(j > 23)
			{
				j -= 23;
				time_int++;
			}

			strncat(date, sup_int_to_str(j), 2);
			strncat(date, " ", 1);
			break;
		case 2:
			j = lt->tm_yday;
			j += time_int % 365;
			time_int /= 365;
			if(j > 365)
			{
				j -= 365;
				time_int++;
			}

			strncat(date, sup_int_to_str(j), 3);
			strncat(date, ":", 1);
			break;
		case 3:
			strncat(date, sup_int_to_str(lt->tm_year + time_int), 3);
			break;
		}
	}

	return;
}

void sup_add_symb(char *str, int num)
{
	if(str[num] == '9')
	{
		if(str[num - 1] >= '0' && str[num - 1] <= '9')
		{
			sup_add_symb(str, num - 1);
			str[num] = '0';
		}
		else
		{
			str[strlen(str) - 4] = '\0';
			strncat(str, "0.txt", 5);
			str[num] = '1';
		}
	}
	else str[num] = str[num] + 1;
}

int sup_check_numb(const char *str)
{
	if(str[0] != '#')
		return -1;

	return sup_str_to_int(&str[1]);
}

void sup_add_quotes(char *str)
{
	int length = strlen(str);
	if(str[0] != '"' && str[length - 1] != '"')
	{
		str[length + 1] = '\0';
		while(length)
		{
			str[length] = str[length - 1];
			length--;
		}
		strncat(str, "\"", 1);
		str[0] = '"';
	}
}

void sup_del_quotes(char *str)
{
	int length = strlen(str);
	if(str[0] == '"' && str[length - 1] == '"')
	{
		str[--length] = '\0';
		STRNCPY(str, str + 1, length--);
	}
}

int sup_str_to_int(const char *str)
{
	int data = 0;

	for(USHORT i = 0; i < strlen(str); i++)
	{
		data *= 10;
		data += str[i] - '0';
	}

	return data;
}

char* sup_int_to_str(int data)
{
	static char buf[7];
	char str[7];
	int i;

	for(i = 0; data; i++)
	{
		str[i] = '0' + data % 10;
		data /= 10;
	}

	str[i] = buf[i] = '\0';
	for(int j = 0; i; j++)
	{
		i--;
		buf[j] = str[i];
	}

	return buf;
}