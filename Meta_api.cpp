#include <extdll.h>
#include <meta_api.h>

#include "MyPlugin.h"

// Must provide at least one of these..
static META_FUNCTIONS gMetaFunctionTable = {
	NULL,			// pfnGetEntityAPI				HL SDK; called before game DLL
	NULL,			// pfnGetEntityAPI_Post			META; called after game DLL
	GetEntityAPI2,	// pfnGetEntityAPI2				HL SDK2; called before game DLL
	NULL,			// pfnGetEntityAPI2_Post		META; called after game DLL
	NULL,			// pfnGetNewDLLFunctions		HL SDK2; called before game DLL
	NULL,			// pfnGetNewDLLFunctions_Post	META; called after game DLL
	GetEngineFunctions,	// pfnGetEngineFunctions	META; called before HL engine
	NULL,			// pfnGetEngineFunctions_Post	META; called after HL engine
};

// Description of plugin
plugin_info_t Plugin_info =
{
	META_INTERFACE_VERSION,	// ifvers
	"Admin Mod",	// name
	"0.01",	// version
	"2012/02/24",	// date
	"JuGGerNaunT <hrono8desmond15@yandex.ru>",	// author
	"http://www.metamod.org/",	// url
	"2EPSG",	// logtag, all caps please
	PT_ANYTIME,	// (when) loadable
	PT_ANYPAUSE,	// (when) unloadable
};

// Global vars from metamod:
meta_globals_t *gpMetaGlobals;		// metamod globals
gamedll_funcs_t *gpGamedllFuncs;	// gameDLL function tables
mutil_funcs_t *gpMetaUtilFuncs;		// metamod utility functions

// Metamod requesting info about this plugin:
//  ifvers			(given) interface_version metamod is using
//  pPlugInfo		(requested) struct with info about plugin
//  pMetaUtilFuncs	(given) table of utility functions provided by metamod
C_DLLEXPORT int Meta_Query(char * /*ifvers */, plugin_info_t **pPlugInfo, mutil_funcs_t *pMetaUtilFuncs)
{
	*pPlugInfo = &Plugin_info; // Give metamod our plugin_info struct
	gpMetaUtilFuncs = pMetaUtilFuncs; // Get metamod utility function table

	return true;
}

// Metamod attaching plugin to the server.
//  now				(given) current phase, i.e. during map, during change level, or at startup
//  pFunctionTable	(requested) table of function tables this plugin catches
//  pMGlobals		(given) global vars from metamod
//  pGamedllFuncs	(given) copy of function tables from game dll
C_DLLEXPORT int Meta_Attach(PLUG_LOADTIME /* now */, META_FUNCTIONS *pFunctionTable,
				meta_globals_t *pMGlobals, gamedll_funcs_t *pGamedllFuncs)
{
	if(!pMGlobals)
	{
		LOG_MESSAGE(PLID, "Meta_Attach called with null pMGlobals");
		return false;
	}
	gpMetaGlobals = pMGlobals;

	if(!pFunctionTable)
	{
		LOG_MESSAGE(PLID, "Meta_Attach called with null pFunctionTable");
		return false;
	}
	memcpy(pFunctionTable, &gMetaFunctionTable, sizeof(META_FUNCTIONS));
	gpGamedllFuncs = pGamedllFuncs;

	PluginInit();

	return true;
}

// Metamod detaching plugin from the server.
// now		(given) current phase, i.e. during map, etc
// reason	(given) why detaching (refresh, console unload, forced unload, etc)
C_DLLEXPORT int Meta_Detach(PLUG_LOADTIME /* now */, PL_UNLOAD_REASON /* reason */) 
{
	LOG_CONSOLE(PLID, "Meta_Detach");
	return true;
}