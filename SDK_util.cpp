#pragma warning (disable: 4996)
#include <extdll.h>
#include <enginecallback.h>		// ALERT()
#include <osdep.h>				// win32 vsnprintf, etc

//=========================================================
// UTIL_LogPrintf - Prints a logged message to console.
// Preceded by LOG: ( timestamp ) < message >
//=========================================================
void UTIL_LogPrintf(char *fmt, ...)
{
	va_list			argptr;
	static char		string[1024];

	va_start(argptr, fmt);
	vsnprintf(string, sizeof(string), fmt, argptr);
	va_end(argptr);

	// Print to server console
	ALERT( at_logged, "%s", string );
}

//=========================================================
// UTIL_HudMessage - Prints a message to player's chat.
//=========================================================
void UTIL_HudMessage(edict_t *pEntity, const hudtextparms_t &textparms, char *pMessage)
{
	/*if (pEntity)
		MESSAGE_BEGIN(MSG_ONE_UNRELIABLE, SVC_TEMPENTITY, NULL, pEntity);
	else
		MESSAGE_BEGIN(MSG_BROADCAST, SVC_TEMPENTITY);

	WRITE_BYTE(29);
	WRITE_BYTE(textparms.channel & 0xFF);
	WRITE_SHORT(FixedSigned16(textparms.x, (1<<13)));
	WRITE_SHORT(FixedSigned16(textparms.y, (1<<13)));
	WRITE_BYTE(textparms.effect);
	WRITE_BYTE(textparms.r1);
	WRITE_BYTE(textparms.g1);
	WRITE_BYTE(textparms.b1);
	WRITE_BYTE(0);
	WRITE_BYTE(255);
	WRITE_BYTE(255);
	WRITE_BYTE(250);
	WRITE_BYTE(0);
	WRITE_SHORT(FixedUnsigned16(textparms.fadeinTime, (1<<8)));
	WRITE_SHORT(FixedUnsigned16(textparms.fadeoutTime, (1<<8)));
	WRITE_SHORT(FixedUnsigned16(textparms.holdTime, (1<<8)));

	if (textparms.effect == 2)
		WRITE_SHORT(FixedUnsigned16(textparms.fxTime, (1<<8)));

	WRITE_STRING(pMessage);
	MESSAGE_END();*/
}

/* warning - buffer of msg must be longer than 190 chars!
(here in AMX it is always longer) */
void UTIL_ClientPrint(edict_t *pEntity, int msg_dest, char *msg)
{
	char c = msg[190];
	msg[190] = 0;			// truncate without checking with strlen()

	if (pEntity)
		MESSAGE_BEGIN(MSG_ONE, SVC_TEMPENTITY, NULL, pEntity);
	else
		MESSAGE_BEGIN(MSG_BROADCAST, SVC_TEMPENTITY);

	WRITE_BYTE(msg_dest);
	WRITE_STRING(msg);
	MESSAGE_END();
	msg[190] = c;
}