// ===========================================================================//	UAMNetwork.c 			� 1998-2001 Microsoft Corp. All rights reserved.// ===========================================================================// 	Networking functions for use by Microsoft User Authentication Method.////	There are several versions of authentication used by the UAM to an SFM//	server. Here is the matrix which shows which Microsoft AFP encryption//	version will be used depending on the server version.////	Server Version		Auth	Chng Pswd		Notes//	--------------		----	---------		---------------//	NT 4.0				1.0		1.0				Weakest, LM Hash only//	Win2K				2.0		2.0				NTLMv2 auth, LM Hash for chgn pswd//	.NET				2.0		3.0				Strongest, using NTLMv2 for both//// ===========================================================================#ifdef UAM_TARGET_CARBON#include <CoreFoundation/CoreFoundation.h>#include <Carbon/Carbon.h>#endif#include <macssp.h>#include <String.h>#include <macstrsafe.h>#include "UAMMain.h"#include "UAMEncrypt.h"#include "UAMDebug.h"#include "UAMNetwork.h"#include "UAMUtils.h"#include "UAMDLOGUtils.h"#include "UAMDialogs.h"#include "UAMKeychain.h"#include "UAMPrefs.h"unsigned char				gCmdBuffer[kMaxAFPCommand];unsigned char				gReplyBuffer[kMaxAFPCommand];MSUAMLoginReplyBlock		gMSUAMReply;extern Str32 				gAFPVersion;extern OTAddress			*gServerAddress;extern long					gSupportedUAMs;extern UInt32				gExpirationTime;extern Str32				gServerName;extern Boolean				gGuestLogon;extern DialogPtr			gDialog;extern UAM_PREFERENCES		gUAMPreferences;// ---------------------------------------------------------------------------//		� UAM_MapCharactersIntoHostSet()// ---------------------------------------------------------------------------//	Given a counted string, and a "host mapping table", do an in-place conversion//	of that string into the host character set.  The table is construed to be//	of length 255 - StartingExtendedCharValue chars long, and a character for//	character conversion will be indicated for any chars in targetStr which //	are equal to or in excess of StartingExtendedCharValue.Boolean UAM_MapCharactersIntoHostSet(char *szTarg, char *mappingTbl){	unsigned char c;			while (*szTarg)		{		if ((unsigned char)*szTarg >= (unsigned char)kStartingExtendedCharValue)		{			c = *(mappingTbl+ (unsigned char)*szTarg - kStartingExtendedCharValue);			if (c == kIllegalMappedExtChar) {				DbgPrint_((DBGBUFF, "Illegal mapping character"));				return(false);			}			else {				*szTarg = c;			}		}				szTarg++;	}				return(true);}#pragma mark-// ---------------------------------------------------------------------------//		� UAM_UAMLogin()// ---------------------------------------------------------------------------//	This routine does the actual logging onto the server.OSStatus UAM_UAMLogin(UAMArgs *inUAMArgs){	OSStatus	theError 	= noErr;	Boolean		theLoop		= true;	CursHandle	theCursor;		Assert_(inUAMArgs != NULL);								do	{		theCursor = GetCursor(watchCursor);		SetCursor(*theCursor);						if (gGuestLogon) {			theError = UAM_LoginGuest(inUAMArgs);		}		else {        	SspDebugPrintHex(        			(char*)inUAMArgs->Opt.pwDlg.userName,        			PSTR_LENGTH(inUAMArgs->Opt.pwDlg.userName)+1        			);        	SspDebugPrintHex(        			(char*)inUAMArgs->Opt.pwDlg.password,        			strlen((char*)inUAMArgs->Opt.pwDlg.password)        			);        	//        	//The password buffer in UAMArgs is always in C style form. The encryption        	//routines expect it to be in pascal style, so we convert it before we        	//send it on.        	//        	if (strlen((char*)inUAMArgs->Opt.pwDlg.password) != 0)        	{        		_c2pstr((char*)inUAMArgs->Opt.pwDlg.password);        	}			theError = UAM_LoginMSUAM(inUAMArgs);		}				if (theError != noErr)		{			//			//For whatever reason, we couldn't log into the server, handle the most			//basic errors and try to logon again by presenting the login dialog			//again. Otherwise, exit...			//						//			//Make sure in debug that the password field is cleared in			//an error condition.			//			Assert_(PSTR_LENGTH(inUAMArgs->Opt.pwDlg.password) == 0);						//			//03.11.02 MJC: Only put up the error dialog for real errors.			//			if (theError != userCanceledErr) {							UAM_ReportError(theError);			}							UAM_CloseSession(inUAMArgs);						switch(theError)			{				//				//Standard uam error codes that may occur during logon when				//we will want to allow the user to try again.				//				//				case afpNTPasswordExpired:				case afpPwdExpiredErr:				case afpUserNotAuth:				case afpParmErr:				case afpNTAccountDisabled:				case afpNTInvalidWorkstation:				case afpNTInvalidLogonHours:								//				//We use this error to signal to the uam that a non-critical error				//has occured during login, most likely due to the users uam				//configuration.				//				case userCanceledErr:									if (MS_UAMPwdDialog(inUAMArgs) != noErr)						return(userCanceledErr);					break;																default:					theLoop 	= false;					theError	= userCanceledErr;					break;			}		}		else		{			if ((gSupportedUAMs & (kMSUAM_V2_Supported | kMSUAM_V3_Supported)) && (!gGuestLogon))			{				//				//Check for password expiration at this point.				//				UInt32	theDaysTillExpiration = (((gExpirationTime / 60) / 60) / 24);												if (theDaysTillExpiration <= MINIMUM_DAYS_TILL_EXPIRATION)				{					//					//The password is going to expire within MINIMUM_DAYS_TILL_EXPIRATION,					//post the nofication dialog.					//										UAM_ChangePasswordNotificationDlg(theDaysTillExpiration);				}			}						if (UAM_KCAvailable())			{				//				//If the user is allowed to save their password and				//the keychain check box is checked, save the current				//credentials to the keychain.				//				#ifdef UAM_TARGET_CARBON				if (UAM_GetDialogControlValue(gDialog, DITEM_Keychain) > 0)				#else				if (UAM_GetCValue(gDialog, DITEM_Keychain) > 0)				#endif				{					theError = UAM_KCSavePassword(									inUAMArgs->Opt.auth.userName, 									inUAMArgs->Opt.auth.password,									gServerName					);										if ((theError != noErr) && (theError != userCanceledErr))					{						if (theError == errKCDuplicateItem)						{							Int16 theResponse;														//							//A duplicate item exists, see if the user wants							//to replace it.							//							                            UAM_StandardAlert(                                        uamErr_KeychainEntryExistsMessage,                                        uamErr_KeychainEntryExistsExplanation,                                        &theResponse);														if (theResponse == kAlertStdAlertOKButton)							{								//								//The user asked us to replace the item. Try one								//more time to add the keychain item.								//								theError = UAM_KCDeleteItem(												inUAMArgs->Opt.auth.userName,												gServerName								);																if (theError == noErr)								{									theError = UAM_KCSavePassword(													inUAMArgs->Opt.auth.userName, 													inUAMArgs->Opt.auth.password,													gServerName									);																		if (theError != noErr)									{										//										//We errored out, nothing to do but report it.										//										UAM_ReportError(theError);									}								}							}						}						else						{							UAM_ReportError(theError);						}												//						//We do not want to pass back any keychain error codes to						//the AFP client!						//						theError = noErr;					}								}			}						//			//04.27.01: We don't need this buffer that contained the password			//in cleartext form anymore. Clear them for better security.			//03.02.02 MJC - Convert to use RtlSecureZeroMemory().			//			RtlSecureZeroMemory(				inUAMArgs->Opt.auth.password,				PSTR_LENGTH(inUAMArgs->Opt.pwDlg.password)+1				);						theLoop = false;		}			}while(theLoop);		return(theError);}#pragma mark-// ---------------------------------------------------------------------------//		� UAM_CloseSession()// ---------------------------------------------------------------------------//	Close a session on the AFP server.OSStatus UAM_CloseSession(UAMArgs *inUAMArgs){	OSStatus theError;		    theError = UAM_CALLBACK_ONEPARAM(                    inUAMArgs->callbacks->CloseSessionUPP,                    kCloseSessionProcInfo,                    inUAMArgs->sessionRefNum);                        inUAMArgs->sessionRefNum = 0;		return(theError);}// ---------------------------------------------------------------------------//		� UAM_OpenSession()// ---------------------------------------------------------------------------//	Open a session on the AFP server.OSStatus UAM_OpenSession(	UAMArgs 		*inUAMArgs,							UAMMessage		*inMessage,							unsigned char	*inCmdBuffer,							UInt32			inCmdBufferSize,							unsigned char	*inReplyBuffer,							UInt32			inReplyBufferSize	){	OSStatus theError;		Assert_(inUAMArgs		!= NULL);	Assert_(inMessage		!= NULL);	Assert_(inCmdBuffer		!= NULL);	Assert_(gServerAddress	!= NULL);		//	//Note that inReplyBuffer can be null.	//		inMessage->commandCode		= kOpenSession;	inMessage->cmdBuffer		= inCmdBuffer;	inMessage->cmdBufferSize	= inCmdBufferSize;	inMessage->replyBuffer		= inReplyBuffer;	inMessage->replyBufferSize	= inReplyBufferSize;	inMessage->completion		= NULL;	inMessage->contextPtr		= NULL;	    theError = UAM_CALLBACK_THREEPARAM(                    inUAMArgs->callbacks->OpenSessionUPP,                    kOpenSessionProcInfo,                    gServerAddress,                    NULL,                    inMessage);    	//	//Even if theError == noErr, the parameter block's result	//param still might hold an error code!	//	return((theError == noErr) ? inMessage->result : theError);}// ---------------------------------------------------------------------------//		� UAM_LoginGuest()// ---------------------------------------------------------------------------//	Log into an AFP server as a guest.OSStatus UAM_LoginGuest(UAMArgs *inUAMArgs){	Ptr				theCmdPtr;	UInt32			theCmdSize;	OSStatus		theError;	UAMMessage		theMessage;		Assert_(inUAMArgs != NULL);	Assert_(PSTR_LENGTH(gAFPVersion) != 0);		theCmdPtr = (Ptr)&gCmdBuffer[0];		*theCmdPtr	= kFPLogin;	theCmdPtr	+= sizeof(Byte);		StringPush_(gAFPVersion, (StringPtr)theCmdPtr);	StringPush_(PSTR_GuestLogin, (StringPtr)theCmdPtr);			theCmdSize = theCmdPtr - ((Ptr)&gCmdBuffer);		theError = UAM_OpenSession(	inUAMArgs,								&theMessage,								gCmdBuffer,								theCmdSize,								NULL,								0				);		DbgPrint_((DBGBUFF, "OpenSession() returned %d in UAM_LoginGuest()", (int)theError));								if (theError == noErr) {		inUAMArgs->sessionRefNum = theMessage.sessionRefNum;	}		return(theMessage.result);}// ---------------------------------------------------------------------------//		� UAM_LoginMSUAM()// ---------------------------------------------------------------------------//	Log into an AFP server using the .AFPTranslator (through the UAM).OSStatus UAM_LoginMSUAM(UAMArgs *inUAMArgs){	OSStatus			theError = noErr;	Ptr					theCmdPtr;	UInt32				theCmdSize;	Str32				theWSName;	Str64				theUserName;	char				theDomainName[DNLEN+1];	UAMMessage			theMessage;			Assert_(inUAMArgs != NULL);	Assert_(PSTR_LENGTH(gAFPVersion) != 0);			//	//11.10.99: Clear out the command buffer just to be safe.	//	ZeroMemory(gCmdBuffer, sizeof(gCmdBuffer));	theCmdPtr = (Ptr)gCmdBuffer;	UAM_PStrCopy(inUAMArgs->Opt.auth.userName, theUserName);				//	//09.28.00: If the user name is blank, then we create a random user name	//so as to attempt a login as "guest".	//	if (PSTR_LENGTH(theUserName) == 0)	{		UAM_PStrCopy("\pGst&^^", theUserName);	}			if (inUAMArgs->callbacks != NULL)	{		UAM_GetWorkStationName(theWSName);				//		//This can be 0, but shouldn't be, catch only for debugging.		//		Assert_(PSTR_LENGTH(theWSName) != 0);				//		//Build the AFP command structure for a login.		//				*theCmdPtr   = kFPLogin;		theCmdPtr	+= sizeof(Byte);				//		//Stuff the AFP command block with our info.		//		StringPush_(gAFPVersion, (StringPtr)theCmdPtr);						//		//Check UAM version that the server supports, use the latest...		//		if (gSupportedUAMs & kMSUAM_V3_Supported)		{			DbgPrint_((DBGBUFF, "Using MS3.0 Encryption"));			StringPush_(PSTR_EncryptedLogin3_0, (StringPtr)theCmdPtr);		}		else if (gSupportedUAMs & kMSUAM_V2_Supported)		{			DbgPrint_((DBGBUFF, "Using MS2.0 Encryption"));			StringPush_(PSTR_EncryptedLogin2_0, (StringPtr)theCmdPtr);		}		else		{			//			//Check and see if the user wants to allow this weak authentication			//to take place.			//			if (gUAMPreferences.flags & UAM_PREFS_REQUIRE_STRONG_ENCRYPTION)			{				//				//Nope, the auth strength is too weak for the user to				//stomach. Exit gracefully.				//				UAM_StandardAlert(						uamErr_AuthenticationMessage,						uamErr_AuthTooWeak,						NULL						);								//				//Since we're returned right from here, we need to				//zero out the password the user entered.				//					RtlSecureZeroMemory(						inUAMArgs->Opt.pwDlg.password,						PSTR_LENGTH(inUAMArgs->Opt.pwDlg.password)+1						);										return(userCanceledErr);			}						DbgPrint_((DBGBUFF, "Using MS1.0 Encryption"));			StringPush_(PSTR_EncryptedLogin1_0, (StringPtr)theCmdPtr);		}				StringPush_(theUserName, (StringPtr)theCmdPtr);						//		//11.23.99 MJC - Only copy the workstation name if 		//there is one. If there is no workstation name for the Mac, then		//we pad the end of the buffer with 2 bytes of 0's for NT4 SP6.		//		if (theWSName[0] > 0)		{			StringPush_(theWSName, (StringPtr)theCmdPtr);		}		else if (	((gSupportedUAMs & kMSUAM_V2_Supported) == 0) &&					((gSupportedUAMs & kMSUAM_V3_Supported) == 0)	)		{			//			//If the encryption version is 1.0, then we're talking to an NT 4.0			//or 3.51 file server.			//						//10.17.01			//NOTE: We'll never get here now since UAM_GetWorkStationName()			//always returns a name.			//						*theCmdPtr++ = 0x00;			*theCmdPtr++ = 0x00;		}				//		//For MS3.0 authentication, we need to do a little extra work by		//supplying the server name, domain name, user name and workstation		//name in C-style strings to the continue function.		//		if (gSupportedUAMs & (kMSUAM_V2_Supported | kMSUAM_V3_Supported))		{			Str32	theServerName;			//			//Copy the pascal-style server name so we can convert it to a			//C-style string (null terminated).			//			UAM_PStrCopy(gServerName, theServerName);						//			//Convert all p-strings to c-strings.			//			_p2cstr(theServerName);			_p2cstr(theUserName);			_p2cstr(theWSName);						//			//See if the user name contains a domain name. If not, then pass			//0 as the domain name.			//			memset(theDomainName, 0, sizeof(theDomainName));						if (UAM_ExtractDomainName((char*)theUserName, theDomainName, sizeof(theDomainName)) == TRUE)			{				//				//Since a domain name was included, we need to weed out the				//user name.				//				UAM_ExtractUserName((char*)theUserName, (char*)theUserName, sizeof(theUserName));			}		}			//		//The command block must end on an even boundary!		//		if ((theCmdPtr - (Ptr)gCmdBuffer) % 2)			*theCmdPtr++ = 0x00;										//		//We need to get the command buffer size so we can pass it along.		//		theCmdSize = theCmdPtr - ((Ptr)gCmdBuffer);				//		//We can't be bigger than the max AFP command allowed.		//		Assert_(theCmdSize <= kMaxAFPCommand);				//		//Zero out the parameter block.		//		RtlSecureZeroMemory(&theMessage, sizeof(UAMMessage));						theError = UAM_OpenSession(	inUAMArgs,									&theMessage,									gCmdBuffer,									theCmdSize,									gReplyBuffer,									sizeof(gReplyBuffer)	);				//		//The error returned should always be afpAuthContinue unless the		//server is rejecting the username.		//		if (	(theError == noErr) 				||				(theError == afpAuthContinue) 		)		{			//			//Save the session reference number to the open AFP			//session we have with the server.			//			inUAMArgs->sessionRefNum = theMessage.sessionRefNum;						BlockMove(gReplyBuffer, &gMSUAMReply, sizeof(MSUAMLoginReplyBlock));						//			//We need to handle the authentication differently for v3.0 versus			//v2.0. The previous 2 versions use LM Hash for security, v3 uses			//NTLMv2.			//			if (gSupportedUAMs & (kMSUAM_V2_Supported | kMSUAM_V3_Supported))			{				theError = UAM_LoginContinueMS20(								inUAMArgs,								(char*)theUserName,								(char*)theDomainName,								(char*)theWSName								);			}			else			{							theError = UAM_LoginContinueMS10(inUAMArgs);			}		}		else		{			//			//The server refused the connection based on the UAM used or			//just the user name.			//			DbgPrint_((DBGBUFF, "OpenSession() returned %d in UAM_LoginMSUAM(), we're bailing...", (int)theError));						//			//The username was rejected most likely. We need to zero out			//the password field for security.			//			RtlSecureZeroMemory(				inUAMArgs->Opt.pwDlg.password,				PSTR_LENGTH(inUAMArgs->Opt.pwDlg.password)+1				);		}	}		return(theError);}// ---------------------------------------------------------------------------//		� UAM_PascalStringToCString()// ---------------------------------------------------------------------------//	Convert a pascal password into a c-style string for LMv2 authentication.//void UAM_PascalStringToCString(	const Str255	inPascal,	BOOL			inUpcase,	char*			outCStyle,	Size			inCBufferSize	){	Str255		thePascal; //Temporary storage		//	//Make a copy so we don't disrupt the original.	//	UAM_PStrCopy(inPascal, thePascal, sizeof(thePascal));	if (inUpcase) {			UpperString(thePascal, true);	}	//	//02.15.02 MJC: Replace strcpy() with strsafe routine.	//	StringCbCopy(outCStyle, inCBufferSize, _p2cstr(thePascal));}// ---------------------------------------------------------------------------//		� UAM_LoginContinueMS20()// ---------------------------------------------------------------------------//	Continue the login when authenticating using MS2.0 or MS3.0.//OSStatus UAM_LoginContinueMS20(	UAMArgs*	inUAMArgs,	char*		inUserName,	char*		inDomainName,	char*		inWorkstationName			){	char		thePassword[UAM_MAX_LMv2_PASSWORD+1];	PUAMLogin2	theCmdPtr;	OSStatus	theError = memFullErr;	UInt32		theTimeTillExpiration;		DbgPrint_((DBGBUFF, "Enter UAM_LoginContinueMS20()"));		//	//Make sure the UI is doing it's job and limiting the password to	//only UAM_MAX_LMv2_PASSWORD characters long.	//	if (PSTR_LENGTH(inUAMArgs->Opt.auth.password) > UAM_MAX_LMv2_PASSWORD)	{		Assert_(0);		return(afpParmErr);	}			//	//Convert the pascal password into a c-style string we can use.	//	//01.16.02: Pass maximum string buffer size to conversion routine.	//	UAM_PascalStringToCString(			inUAMArgs->Opt.auth.password,			false,			thePassword,			sizeof(thePassword)			);					//	//Clear out the command buffer just to be safe.	//	ZeroMemory(gCmdBuffer, sizeof(gCmdBuffer));		//	//Build the AFP command block that we'll send to the server.	//	theCmdPtr			= (PUAMLogin2)gCmdBuffer;	theCmdPtr->command	= kFPLoginCont;	theCmdPtr->pad		= '\0';		HRESULT					hResult;	ULONG					theNegotiateFlags;	ULONG					theChallengeMessageLength		= 0;	CHALLENGE_MESSAGE*		theChallengeMessage				= NULL;	ULONG					theAuthenticateMessageLength	= 0;	AUTHENTICATE_MESSAGE*	theAuthenticateMessage			= NULL;	USER_SESSION_KEY		theUserSessionKey;		//	//Build a "fake" challenge message that we can pass to the NTLMv2 routines.	//We do this since the server does not send us a real NTLM challenge message.	//	hResult = MacSspGenerateChallengeMessage(					gMSUAMReply.serverChallenge,					&theChallengeMessageLength,					&theChallengeMessage					);		if (SUCCEEDED(hResult))	{		theNegotiateFlags = NTLMSSP_NEGOTIATE_UNICODE		|							NTLMSSP_NEGOTIATE_ALWAYS_SIGN	|							NTLMSSP_NEGOTIATE_NTLM2			|							NTLMSSP_NEGOTIATE_128			|							NTLMSSP_TARGET_TYPE_SERVER;						hResult = MacSspHandleNtlmv2ChallengeMessage(						inUserName,						inDomainName,						inWorkstationName,						thePassword,						theChallengeMessageLength,						theChallengeMessage,						&theNegotiateFlags,						&theAuthenticateMessageLength,						&theAuthenticateMessage,						&theUserSessionKey						);				if (SUCCEEDED(hResult))		{			Assert_(theAuthenticateMessage != NULL);						//			//We have to swap the authentication bytes here as			//MacSspHandleNtlmv2ChallengeMessage() puts the message in			//Windows byte order (littl endian).			//			SspSwapAuthenticateMessageBytes(theAuthenticateMessage);						memcpy(				theCmdPtr->UAMInfo,				((char*)theAuthenticateMessage) + theAuthenticateMessage->LmChallengeResponse.Buffer,				theAuthenticateMessage->LmChallengeResponse.Length				);								theError = UAM_SendAFPRequestToServer(							inUAMArgs,							gCmdBuffer,							sizeof(UAMLogin2),							(UInt8*)&theTimeTillExpiration,							sizeof(UInt32)							);				//			//The reply buffer contains the password expiration time.			//			if (theError == noErr)			{				theTimeTillExpiration = swaplong(theTimeTillExpiration);				gExpirationTime = theTimeTillExpiration;			}		}	}		//	//The challenge message and authenticate message pointers are allocated	//in MacSspHandleNtlmv2ChallengeMessage() and we need to dispose of them.	//		if (theChallengeMessage)	{		delete theChallengeMessage;	}		if (theAuthenticateMessage)	{		delete theAuthenticateMessage;	}	    return(theError);}// ---------------------------------------------------------------------------//		� UAM_SendAFPRequestToServer()// ---------------------------------------------------------------------------//	Send the challenge response back to the server.//OSStatus UAM_SendAFPRequestToServer(	UAMArgs*			inUAMArgs,	unsigned char*		inChallengeResponseBuffer,	SInt16				inChallengeResponseBufferSize,	UInt8*				ioReplyBuffer,	SInt16				inReplyBufferSize	){	UAMMessage		theMessage;	OSStatus		theError;		//	//We need to zero out the parameter block.	//	ZeroMemory(&theMessage, sizeof(UAMMessage));		theMessage.sessionRefNum	= inUAMArgs->sessionRefNum;	theMessage.commandCode		= kSendRequest;	theMessage.cmdBuffer		= inChallengeResponseBuffer;	theMessage.cmdBufferSize	= inChallengeResponseBufferSize;	theMessage.replyBuffer		= ioReplyBuffer;	theMessage.replyBufferSize	= inReplyBufferSize;	theMessage.completion		= NULL;	theMessage.contextPtr		= NULL;	    theError = UAM_CALLBACK_ONEPARAM(                    inUAMArgs->callbacks->SendRequestUPP,                    kSendRequestProcInfo,                    &theMessage);    	//	//The actual error code may lerk in either place	//	theError = (theError == noErr) ? theMessage.result : theError;			//	//For debugging we do this so we know where the error came from.	//    DbgPrint_((DBGBUFF, "SendRequest() returned %d in UAM_SendAFPRequestToServer()", (int)theError));				return(theError);	}// ---------------------------------------------------------------------------//		� UAM_LoginContinueMS10()// ---------------------------------------------------------------------------//	Continue the login for v1 authentication. ////	The following command block is built.////	|---------------------|//	|     AFP Command     | <- One Byte//	|---------------------|//	|	    Filler        |//	|---------------------|//	| Encrypted Password  |//	/                     ///	|---------------------|OSStatus UAM_LoginContinueMS10(UAMArgs *inUAMArgs){ 	char			thePassT[UAM_CLRTXTPWDLEN+1];	OSStatus		theError;	UInt32			theTimeTillExpiration;    PUAMLogin		theCmdPtr;        DbgPrint_((DBGBUFF, "Enter UAM_LoginContinueMS10()"));		//	//04.27.01: This should never happen as the UI should not allow any password	//to be over our limit. But, we check to make sure.	//	if (PSTR_LENGTH(inUAMArgs->Opt.auth.password) > UAM_CLRTXTPWDLEN)	{		//		//If we were to proceed from here, we would crash as our		//password buffers are not big enough.		//		Assert_(0);		return(afpParmErr);	}		//	//Copy the password as we're going to munge it up and we don't want	//to munge the original, case sensitive, copy.	//	UAM_PascalStringToCString(			inUAMArgs->Opt.auth.password,			true,			thePassT,			sizeof(thePassT)			);			//	//Map extended characters to the correct values for NT.	//	if (!UAM_MapCharactersIntoHostSet(thePassT, gMSUAMReply.serverExtCharMapTable))	{		DbgPrint_((DBGBUFF, "UAM_MapCharactersIntoHostSet() failed"));		return(afpUserNotAuth);	}		//	//11.10.99: Clear out the command buffer just to be safe.	//	ZeroMemory(gCmdBuffer, sizeof(gCmdBuffer));					theCmdPtr			= (PUAMLogin)gCmdBuffer;	theCmdPtr->command	= kFPLoginCont;	theCmdPtr->pad		= '\0';				//	//Get the encrypted LmOwf password using the encrypting routines.	//		if (!UAM_GetEncryptedLmOwfPassword(				thePassT,				gMSUAMReply.serverChallenge,				theCmdPtr->UAMInfo))	{		Assert_(0);		return(afpMiscErr);	}		//	//Be a good security citizen and zero out the password buffer	//that we don't need anymore.	//	RtlSecureZeroMemory(thePassT, sizeof(thePassT));		//	//The encrypted OWF should never be null.	//	Assert_(strlen(theCmdPtr->UAMInfo) > 0);		theError = UAM_SendAFPRequestToServer(					inUAMArgs,					gCmdBuffer,					sizeof(UAMLogin),					(UInt8*)&theTimeTillExpiration,					sizeof(UInt32)					);	//	//The reply buffer contains the password expiration time.	//	if (theError == noErr)	{		theTimeTillExpiration = swaplong(theTimeTillExpiration);		gExpirationTime = theTimeTillExpiration;	}					return(theError);}// ---------------------------------------------------------------------------//		� UAM_ChangePasswordForMS10()// ---------------------------------------------------------------------------//	Change the user's password on the server.////	->	inUAMArgs		The UAM arguments.//	->	inNewPwd		The new password requested by the user.////	Returns: An error code or noErr.////	|---------------------|//	|     AFP Command     | <- One Byte//	|---------------------|//	|	  New pwd len     |//	|---------------------|//	|     UAM String      |//	/                     ///	|---------------------|//	|      User Name      |//	/					  ///	|---------------------|//  | Old Pwd (Encrypted) |//	/					  ///	|---------------------|//  | New Pwd (Encrypted) |//	/					  ///	|---------------------|OSStatus UAM_ChangePasswordForMS10(UAMArgs *inUAMArgs, StringPtr inNewPwd){		OSStatus	theError;	Ptr			theCmdPtr;	short		theCmdSize;	char		oldPassword[UAM_CLRTXTPWDLEN+1];	char		newPassword[UAM_CLRTXTPWDLEN+1];	Assert_(inNewPwd != NULL);	Assert_(inUAMArgs != NULL);		//	//Check to make sure we don't go over our maximum	//buffer size. The UI should prevent this, but...	//	if ((PSTR_LENGTH(inUAMArgs->Opt.auth.password) > UAM_CLRTXTPWDLEN)	||		(PSTR_LENGTH(inNewPwd) > UAM_CLRTXTPWDLEN)						)	{		//		//Our buffers aren't big enough, a password that is too big was passed.		//		Assert_(0);		return(afpParmErr);	}		//	//Convert everything to C strings and uppercase them as well.	//	UAM_PascalStringToCString(			inUAMArgs->Opt.auth.password,			true,			oldPassword,			sizeof(oldPassword)			);				UAM_PascalStringToCString(			inNewPwd,			true,			newPassword,			sizeof(newPassword)			);			//	//Log into the server. Note that if the password is expired, the server	//will let us make one AFP call, that being FPChangePassword over	//the opened session.	//	theError = UAM_LoginMSUAM(inUAMArgs);			if ((theError != noErr) 				&&		(theError != afpNTPasswordExpired)	&&		(theError != afpPwdExpiredErr)		)	{		//		//If we couldn't login or the password wasn't expired, we		//cannot continue.		//		return(theError);	}		ZeroMemory(gCmdBuffer, sizeof(gCmdBuffer));	theCmdPtr = (Ptr)gCmdBuffer;		*theCmdPtr++ = afpChangePwd;	*theCmdPtr++ = strlen(newPassword);	 	StringPush_(PSTR_EncryptedLogin1_0, theCmdPtr);	if ((theCmdPtr - ((Ptr)gCmdBuffer)) % 2)	{		*theCmdPtr++ = 0x00;	}	StringPush_(inUAMArgs->Opt.auth.userName, theCmdPtr);		//	//Make sure the buffer ends on an even boundary before the next block.	//	if ((theCmdPtr - ((Ptr)gCmdBuffer)) % 2)	{		*theCmdPtr++ = 0x00;	}			theCmdSize 	= theCmdPtr - ((Ptr)gCmdBuffer);	theError 	= afpNTPasswordProcessFailure;	if ( (UAM_MapCharactersIntoHostSet(oldPassword, gMSUAMReply.serverExtCharMapTable)) &&		 (UAM_MapCharactersIntoHostSet(newPassword, gMSUAMReply.serverExtCharMapTable)) 	   )	{		//		//Call the magic NTLM routine to encrypt the passwords into one		//single buffer.		//		if (UAM_GetDoubleEncryptedLmOwfPasswords(oldPassword, newPassword, theCmdPtr))		{					//			//Make a couple last calculations so we can determine the size of the CB.			//						theCmdSize += (kOneWayEncryptedArgSize * 2);            			theError = UAM_SendAFPRequestToServer(							inUAMArgs,							gCmdBuffer,							theCmdSize,							gReplyBuffer,							kMaxAFPCommand							);		}	}		DBGPrintIfOSErr_((int)theError);		//	//We're done with the session, so close it out and return any error codes.	//	UAM_CloseSession(inUAMArgs);		return(theError);}// ---------------------------------------------------------------------------//		� UAM_ChangePasswordForMS20()// ---------------------------------------------------------------------------//	Change the user's password on the server using Microsoft V2.0. This new//	function was required so we can pass the actual password encrypted//	over the wire to the server. The server needs the password so it can//	update the stored clear text password on the DS (Domain Controller).////	->	inUAMArgs		The UAM arguments.//	->	inNewPwd		The new password requested by the user.////	Returns: An error code or noErr.////	|--------------------------------|//	|           AFP Command          | <- One Byte//	|--------------------------------|//	|	        New pwd len          |//	|--------------------------------|//	|           UAM String           | <- Always PSTR_EncryptedLogin2_0//	/                     			 ///	|--------------------------------|//	|           User Name      		 | <- Variable length//	/					  			 ///	|--------------------------------|//  | PENCRYPTED_NT_OWF_PASSWORD 	 | <- Variable length//	/					             ///	|--------------------------------|//  | PSAMPR_ENCRYPTED_USER_PASSWORD | <- Variable length//	/					             ///	|--------------------------------|//OSStatus UAM_ChangePasswordForMS20(UAMArgs *inUAMArgs, const StringPtr inNewPwd){		OSStatus	theError;	Ptr			theCmdPtr;	short		theCmdSize;	char		oldPassword[UAM_CLRTXTPWDLEN+1];	char		newPassword[UAM_CLRTXTPWDLEN+1];	char		newStdPassword[UAM_CLRTXTPWDLEN+1];		SAMPR_ENCRYPTED_USER_PASSWORD	theNewEncryptedWithLm;	ENCRYPTED_NT_OWF_PASSWORD		theOldLmOwfEncryptedWithNewLm;		Assert_(inNewPwd != NULL);	Assert_(inUAMArgs != NULL);			//	//Check to make sure we don't go over our maximum	//buffer size. The UI should prevent this, but...	//	if ((PSTR_LENGTH(inUAMArgs->Opt.auth.password) > UAM_CLRTXTPWDLEN)	||		(PSTR_LENGTH(inNewPwd) > UAM_CLRTXTPWDLEN)						)	{		//		//Our buffers aren't big enough, a password that is too big was passed.		//		Assert_(0);		return(afpParmErr);	}		//	//Convert everything to C strings and uppercase them as well. DO NOT	//uppercase newStdPassword as this is passed as is over the wire.	//	UAM_PascalStringToCString(			inUAMArgs->Opt.auth.password,			true,			oldPassword,			sizeof(oldPassword)			);				UAM_PascalStringToCString(			inNewPwd,			true,			newPassword,			sizeof(newPassword)			);				UAM_PascalStringToCString(			inNewPwd,			false,			newStdPassword,			sizeof(newStdPassword)			);		//	//Log into the server. Note that if the password is expired, the server	//will let us make one AFP call, that being FPChangePassword over	//the opened session.	//	theError = UAM_LoginMSUAM(inUAMArgs);			if ((theError != noErr) 				&&		(theError != afpNTPasswordExpired)	&&		(theError != afpPwdExpiredErr)		)	{		//		//If we couldn't login or the password wasn't expired, we		//cannot continue.		//		return(theError);	}		//	//Start with a clean slate in our command buffer.	//	ZeroMemory(gCmdBuffer, sizeof(gCmdBuffer));		theCmdPtr = (Ptr)gCmdBuffer;		*theCmdPtr++	= afpChangePwd;	*theCmdPtr++	= strlen(newPassword);		StringPush_(PSTR_EncryptedLogin2_0, theCmdPtr);	StringPush_(inUAMArgs->Opt.auth.userName, theCmdPtr);		//	//Make sure the ptr is aligned on an even boundary at this point	//	if ((theCmdPtr - (Ptr)gCmdBuffer) % 2)	{		DbgPrint_((DBGBUFF, "Aligning for even boundary (size = %d)", (theCmdPtr - (Ptr)gCmdBuffer)));		*theCmdPtr++ = 0x00;	}		//	//We doctor up the error code here since the only error that	//SampEncryptLmPasswords() returns is INVALID_FUCTION. We assume	//failure here.	//	theError = afpNTPasswordProcessFailure;		//	//Map the extended characters from Mac to Windows. NOTE that we do it for all	//3 buffers because newPassword and newStdPassword are really different!	//	if ( (UAM_MapCharactersIntoHostSet(oldPassword, gMSUAMReply.serverExtCharMapTable)) &&		 (UAM_MapCharactersIntoHostSet(newPassword, gMSUAMReply.serverExtCharMapTable))	&&		 (UAM_MapCharactersIntoHostSet(newStdPassword, gMSUAMReply.serverExtCharMapTable)) 		 	   )	{		RtlSecureZeroMemory(&theNewEncryptedWithLm, sizeof(SAMPR_ENCRYPTED_USER_PASSWORD));		RtlSecureZeroMemory(&theOldLmOwfEncryptedWithNewLm, sizeof(ENCRYPTED_NT_OWF_PASSWORD));				DbgPrint_((DBGBUFF, "Old password = %s", oldPassword));		DbgPrint_((DBGBUFF, "New password = %s", newPassword));		DbgPrint_((DBGBUFF, "New case sensitive = %s", newStdPassword));						//		//Call the magic function with will encrypt the password(s) for us.		//		theError = MacSspSampEncryptLmPasswords(						oldPassword,						newPassword,						newStdPassword,						&theNewEncryptedWithLm,						&theOldLmOwfEncryptedWithNewLm						);				if (SUCCEEDED(theError))		{			//			// Copy the ENCRYPTED_NT_OWF_PASSWORD into the command buffer.			//			DataPush_(&theOldLmOwfEncryptedWithNewLm, sizeof(ENCRYPTED_NT_OWF_PASSWORD), theCmdPtr);											//			//Copy the SAMPR_ENCRYPTED_USER_PASSWORD into the command buffer.			//			DataPush_(&theNewEncryptedWithLm, sizeof(SAMPR_ENCRYPTED_USER_PASSWORD), theCmdPtr);										//			//Make a last minute calculation so we can determine the size of the CB.			//			theCmdSize = theCmdPtr - (Ptr)(&gCmdBuffer);						Assert_(theCmdSize <= kMaxAFPCommand);			DbgPrint_((DBGBUFF, "Change password v2 cmd size = %d", theCmdSize));						theError = UAM_SendAFPRequestToServer(							inUAMArgs,							gCmdBuffer,							theCmdSize,							gReplyBuffer,							kMaxAFPCommand							);		}	}		DBGPrintIfOSErr_((int)theError);		//	//We're done with the session, so close it out and return any error codes.	//	UAM_CloseSession(inUAMArgs);		return(theError);}// ---------------------------------------------------------------------------//		� UAM_ChangePasswordForMS30()// ---------------------------------------------------------------------------//	Change the user's password on the server using Microsoft V3.0. //OSStatus UAM_ChangePasswordForMS30(	UAMArgs* 		inUAMArgs,	const StringPtr inNewPwd){	char		theNewPassword[UAM_MAX_LMv2_PASSWORD+1];	char		theOldPassword[UAM_MAX_LMv2_PASSWORD+1];	Ptr			theCmdPtr;	SInt16		theCmdSize;	OSStatus	theError = memFullErr;			SFM_PASSWORD_CHANGE_MESSAGE	theChangeMessage;		//	//Log into the server. Note that if the password is expired, the server	//will let us make one AFP call, that being FPChangePassword over	//the opened session.	//	theError = UAM_LoginMSUAM(inUAMArgs);			if ((theError != noErr) 				&&		(theError != afpNTPasswordExpired)	&&		(theError != afpPwdExpiredErr)		)	{		//		//If we couldn't login or the password wasn't expired, we		//cannot continue.		//		return(theError);	}		//	//Convert the pascal passwords into c-style strings we can use and	//map extended characters to the correct values for NT.	//	UAM_PascalStringToCString(			inUAMArgs->Opt.auth.password,			false,			theOldPassword,			sizeof(theOldPassword)			);		UAM_PascalStringToCString(			inNewPwd,			false,			theNewPassword,			sizeof(theNewPassword)			);		//	//Start with a clean slate in our command buffer.	//	ZeroMemory(gCmdBuffer, sizeof(gCmdBuffer));		theCmdPtr = (Ptr)gCmdBuffer;		*theCmdPtr++	= afpChangePwd;	*theCmdPtr++	= strlen(theNewPassword);		StringPush_(PSTR_EncryptedLogin3_0, theCmdPtr);	StringPush_(inUAMArgs->Opt.auth.userName, theCmdPtr);		//	//Make sure the ptr is aligned on an even boundary at this point	//	if ((theCmdPtr - (Ptr)gCmdBuffer) % 2) {			*theCmdPtr++ = 0x00;	}		StringCbCopy(		(char*)theChangeMessage.Signature,		sizeof(theChangeMessage.Signature),		SFM_CHANGE_PASSWORD_SIGNATURE		);		theChangeMessage.cbMessage	= sizeof(theChangeMessage);	theChangeMessage.cbMessage	= swaplong(theChangeMessage.cbMessage);	theChangeMessage.Version	= SFM_CHANGE_PASSWORD_VERSION;	theChangeMessage.Version	= swaplong(theChangeMessage.Version);		theError = MacSspSamiEncryptCStringPasswords(					theOldPassword,					theNewPassword,					&theChangeMessage.NewPasswordEncryptedWithOldNt,					&theChangeMessage.OldNtOwfPasswordEncryptedWithNewNt					);						if (SUCCEEDED(theError))	{		//		//For Macintosh, since we can only have a maximum password length of		//64 bytes, we only send the second half of the encrypted NT password.		//This allows us to leave extra room in our request packet to handle		//a 64 byte long domain\user name.		//				SFM_PASSWORD_CHANGE_MESSAGE_SHORT	theShortMsg;				StringCbCopy(			(char*)theShortMsg.Signature,			sizeof(theShortMsg.Signature),			(char*)theChangeMessage.Signature			);				theShortMsg.cbMessage 	= sizeof(SFM_PASSWORD_CHANGE_MESSAGE_SHORT);		theShortMsg.cbMessage	= swaplong(theShortMsg.cbMessage);		theShortMsg.Version		= theChangeMessage.Version;				memcpy(			&theShortMsg.NewPasswordEncryptedWithOldNt,			(((unsigned char*)&theChangeMessage.NewPasswordEncryptedWithOldNt))+			(sizeof(theChangeMessage.NewPasswordEncryptedWithOldNt)/2),			sizeof(theShortMsg.NewPasswordEncryptedWithOldNt)			);					memcpy(			&theShortMsg.OldNtOwfPasswordEncryptedWithNewNt,			&theChangeMessage.OldNtOwfPasswordEncryptedWithNewNt,			sizeof(theShortMsg.OldNtOwfPasswordEncryptedWithNewNt)			);				DataPush_(&theShortMsg, sizeof(theShortMsg), theCmdPtr);								//		//Make a last minute calculation so we can determine the size of the CB.		//		theCmdSize = theCmdPtr - (Ptr)(&gCmdBuffer);				Assert_(theCmdSize <= kMaxAFPCommand);		DbgPrint_((DBGBUFF, "Change password v3 cmd size = %d", theCmdSize));				theError = UAM_SendAFPRequestToServer(						inUAMArgs,						gCmdBuffer,						theCmdSize,						gReplyBuffer,						kMaxAFPCommand						);	}		DBGPrintIfOSErr_((int)theError);		//	//We're done with the session, so close it out and return any error codes.	//	UAM_CloseSession(inUAMArgs);	return(theError);}