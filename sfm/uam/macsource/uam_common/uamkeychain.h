// ===========================================================================
//	UAMKeychain.h 				© 1999 Microsoft Corp. All rights reserved.
// ===========================================================================

#ifdef UAM_TARGET_CARBON
#include <Carbon/Carbon.h>
#else
#include "Keychain.h"
#endif

#define UAM_KC_DESCRIPTION		"\pMicrosoftš Windows Server"

typedef unsigned char Str8[9];

//
//The volume mount info structure must be aligned for 68K
//as the Mac AFP Client as of MacOS 9.0 is still 68K code!
//
#pragma options align=mac68k

typedef struct
{
	SInt16 		length;					/* length of this record */
	VolumeType 	media;					/* type of media, always AppleShareMediaType */
	SInt16 		flags;					/* bits for no messages, no reconnect, etc */
	char 		nbpInterval;			/* NBP interval parameter; 7 is a good choice */
	char 		nbpCount;				/* NBP count parameter; 5 is a good choice */
	SInt16 		uamType;				/* User Authentication Method */
	SInt16 		zoneNameOffset;			/* offset from start of record to zoneName */
	SInt16 		serverNameOffset;		/* offset from start of record to serverName */
	SInt16 		volNameOffset;			/* offset from start of record to volName */
	SInt16 		userNameOffset;			/* offset from start of record to userName */
	SInt16 		userPasswordOffset;		/* offset from start of record to userPassword */
	SInt16 		volPasswordOffset;		/* offset from start of record to volPassword */
	SInt16 		extendedFlags;			/* extended flags word */
	SInt16 		uamNameOffset;			/* offset to a pascal UAM name string */
	SInt16 		alternateAddressOffset; /* offset to Alternate Addresses in tagged format */
	Str32 		zoneName;				/* server's AppleTalk zone name */					
	char 		filler1;				/* to word align volPassword */
	Str32 		serverName;				/* server name */					
	char 		filler2;				/* to word align volPassword */
	Str27 		volName;				/* volume name */					
	Str31 		userName;				/* user name (zero length Pascal string for guest) */
	Str64 		userPassword;			/* user password (zero length Pascal string if no user password) */					
	char 		filler3;				/* to word align volPassword */
	Str8 		volPassword;			/* volume password (zero length Pascal string if no volume password) */					
	char 		filler4;				/* to word align uamNameOffset */
	Str32 		uamName;				/* UAM name */
	char 		filler5;				/* to word align alternateAddress */
	char 		alternateAddress[kVariableLengthArray];	/* AFPAlternateAddress */
}UAM_AFPXVolMountInfo, *PUAM_AFPXVolMountInfo;

#pragma options align=reset

//
//This macro helps us get offsets into the struct above.
//
#define uamx_member_offset(member)	((size_t) &((UAM_AFPXVolMountInfo *) 0)->member)

OSStatus 	UAM_KCDeleteItem(
				StringPtr 	inUserName,
				Str255 		inServerName
);
void 		UAM_KCInitialize(UAMArgs* inUAMArgs);
Boolean 	UAM_KCAvailable(void);
OSStatus	UAM_KCSavePassword(
				StringPtr 	inUserName,
				StringPtr 	inPassword,
				Str255 		inServerName
);
OSStatus 	UAM_KCFindAppleSharePassword(
				StringPtr	inUserName,
				StringPtr	inPassword,
				StringPtr	inServerName,
				KCItemRef	*outItemRef
);
OSStatus 	UAM_BuildAFPXVolMountInfo(
				StringPtr 				inUserName,
				StringPtr 				inPassword,
				Str255 					inServerName,
				const Str32				inUAMString,
				PUAM_AFPXVolMountInfo*	outVolInfo
);