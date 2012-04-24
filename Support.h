#pragma once
#pragma warning(disable : 4996)

#include <extdll.h>
#include <ctype.h>			// isupper, tolower
#include <support_meta.h>

#ifndef MAX_CONF_LEN
#define MAX_CONF_LEN 1024
#endif
#define MAX_CMD_LEN 512

void sup_gamedir_path(const char *path, char *fullpath);
void sup_server_cmd(const char *command, ...);

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