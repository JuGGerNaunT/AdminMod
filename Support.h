#pragma once
#pragma warning(disable : 4996)

#include "Admins.h"

#include <extdll.h>
#include <ctype.h>			// isupper, tolower
#include <support_meta.h>

#ifndef MAX_CONF_LEN
#define MAX_CONF_LEN 1024
#endif
#define MAX_CMD_LEN 512
#define DATE_LEN 18

extern void UTIL_HudMessage(edict_t *pEntity, const hudtextparms_t &textparms, char *pMessage);
extern void UTIL_ClientPrint(edict_t *pEntity, int msg_dest, char *msg);

void sup_gamedir_path(const char *path, char *fullpath);
void sup_server_cmd(const char *command, ...);
void sup_make_str(char *string, int size, char *fmt, ...);
void sup_convert_date(const char *time_var, char *date);

int sup_str_to_int(const char *str);
char* sup_int_to_str(int data);

//Checking is entity admin or not
inline bool sup_is_admin(edict_t *pEntity)
{
	const char *name;
	name = ENTITY_KEYVALUE(pEntity, "name");
	return adminlist.IsAdmin(name);
}

inline const char* sup_get_name(edict_t *pEntity)
{
	return ENTITY_KEYVALUE(pEntity, "name");
}

inline bool sup_have_rights(edict_t *pEntity, ACC_RIGHT right)
{
	return adminlist.CheckRights(sup_get_name(pEntity), right);
}

// Normalize/standardize a pathname.
//  - For win32, this involves:
//    - Turning backslashes (\) into slashes (/), so that config files and
//      Metamod internal code can be simpler and just use slashes (/).
//    - Turning upper/mixed case into lowercase, since windows is
//      non-case-sensitive.
//  - For linux, this requires no work, as paths uses slashes (/) natively,
//    and pathnames are case-sensitive.
#ifdef linux
#define normalize_path(a)
#elif defined(_WIN32)
inline void normalize_path(char *path)
{
	for(char *cp = path; *cp; cp++)
	{
		if(isupper(*cp))
			*cp=tolower(*cp);
		if(*cp=='\\') *cp='/';
	}
}
#endif /* _WIN32 */