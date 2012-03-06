#include <extdll.h>

#include <h_export.h>		// me

// Holds engine functionality callbacks
enginefuncs_t g_engfuncs;
globalvars_t  *gpGlobals;

#ifdef _WIN32

// Required DLL entry point
bool WINAPI DllMain(HINSTANCE hinstDLL,	DWORD fdwReason, LPVOID lpvReserved)
{
	if(fdwReason == DLL_PROCESS_ATTACH)
	{}
	else if (fdwReason == DLL_PROCESS_DETACH)
	{}

	return true;
}

#elif defined(linux)

// Linux routines to correspond to ATTACH and DETACH cases above.  These
// aren't required by Linux, but are included here for completeness, and
// just in case we come across a need to do something at dll load or
// unload.
void _init(void)
{	// called before dlopen() returns
}
void _fini(void)
{	// called before dlclose() returns
}

#endif

// Receive engine function table from engine.
// This appears to be the _first_ DLL routine called by the engine, so we
// do some setup operations here.
void WINAPI GiveFnptrsToDll(enginefuncs_t* pengfuncsFromEngine, globalvars_t *pGlobals)
{
	memcpy(&g_engfuncs, pengfuncsFromEngine, sizeof(enginefuncs_t));
	gpGlobals = pGlobals;
}