// ===========================================================================//	UAMMain.cp 	� 1997-2001 Microsoft Corp. All rights reserved.// ===========================================================================//	Main unit for Microsoft User Authentication Method.////	Notes://	--------------------------------------------------------------------------//// 	Version History:// 	===========================================================================//	Version 5.0.8://	04.26.01	MJC	- Code cleanup. Combined UAMNetwork and UAMDSNetwork into//					  one file (UAMNetwork.c & UAMNetwork.h)//					- UAM_ChangePasswordV2(): Now check for zero length old//					  and new passwords before upper casing.//					- Now use common LM hash code for login and changepassword v2.//					- Zero password buffers when done with them (security fix).//					- Now clear password field when login error occurs.//	05.01.01		- Fixed bug where over-long user names could be pasted//					  into username field.//					- Can now tab out of user name field when name is max length.//					- Now clear password when failure occurs in UAM_LoginMSUAM().//	07.02.01		- Fixed bug where password was being cleared in UAM_LoginContinue()//					  before I saved it to the keychain.//	10.18.01		- Completed MacOS X v1.0 UAM.//					- Support added for NTLMv2 auth and change password.//  Version 1.0.1://	01.21.02		- Fix: Keychain passwords couldn't exceed 8 bytes.//					- Fix: Close session after attempted login w/bad password.//					- Fix: Now conform to Apple's Aqua UI guidelines.//					- Now check buffer length for all string copies.//	Version 1.0.2://	02.18.02		- User can now choose to allow only strong authentication.//					- Added preference file support for above change.//					- Finally moved OSX builds to CodeWarrior!//	03.01.02		- Many changes for security push////	Version 1.0.3://	05.31.02		- Fixed bug where we weren't dealing with all the new//					  AFP URL type (eg. afp://username:password@server/share).// ===========================================================================#include "UAMMain.h"#include "UAMDebug.h"#include "UAMUtils.h"#include "UAMDialogs.h"#include "UAMNetwork.h"#include "UAMDLOGUtils.h"#include "UAMKeychain.h"#include "UAMPswdField.h"#include "UAMPrefs.h"////Global variables are declared here//Str32				gServerName;Str64				gUserName;Boolean				gContextInited;Boolean				gGuestLogon;Boolean				gSupportsChngPwd;DialogPtr			gDialog;Str32				gAFPVersion;long				gSupportedUAMs;ModalFilterUPP		gPwdDialogFilter;UserItemUPP			gLineItem;Str32				gUAMVersionString;Str32				gZoneName;UInt32				gExpirationTime		= 0;struct sockaddr*	gServerAddress 		= NULL;EventCallbackPtr	gEventCallbackUPP 	= NULL;Boolean				gTriedKeychain		= false;extern UAM_PREFERENCES	gUAMPreferences;#pragma export on// ---------------------------------------------------------------------------//		� main()// ---------------------------------------------------------------------------//	This is the main entry point for our UAM. This function is passed a//	pointer to a UAMArgs struct. This struct contains the function selector,//	call-backs, and many other things we need to do our stuff.EXTERN_API( OSStatus ) UAMMain(UAMArgs *inUAMArgs){    OSStatus theResult = noErr;                switch(inUAMArgs->command)    {        case kUAMOpen:            theResult = MS_UAMOpen(inUAMArgs);            break;                        case kUAMClose:            MS_UAMClose();            break;                        case kUAMPWDlog:            theResult = MS_UAMPwdDialog(inUAMArgs);            break;                        case kUAMLogin:            theResult = UAM_UAMLogin(inUAMArgs);            break;                        case kUAMVSDlog:            break;                case kUAMChgPass:        case kUAMChgPassDlg:            DbgPrint_((DBGBUFF, "Change password dialog must be implemented"));            theResult = kNotForUs;            break;                                default:            //            //If we get here then we were asked to handle a routine that            //we don't support. Return the appropriate error code.            //                                                DbgPrint_((DBGBUFF, "Unsupported function selector in MSUAM main() (%d)", inUAMArgs->command));            theResult = kNotForUs;            break;    }        return(theResult);}#pragma export off// ---------------------------------------------------------------------------//		� MS_UAMOpen()// ---------------------------------------------------------------------------//	This is called by the device package. It is not a required function but//	we use it to initialize our UAM code. Note that when we encounter an//	error we don't make an effort to clean up. Instead we return userCanceledErr//	in which case our UAMClose function will be called by AppleShare Client.OSStatus MS_UAMOpen(UAMArgs *inUAMArgs){	short theUAMConfig = 0;        DbgPrint_((DBGBUFF, "MS_UAMOpen() enter..."));			//	//Get the name of the server we want to log into.	//	UAM_PStrCopy(inUAMArgs->Opt.open.objectName, gServerName, sizeof(gServerName));		//	//Copy the zone name for. If it's NULL, then we	//don't have a zone name.	//	if (inUAMArgs->Opt.open.zoneName != NULL)	{		//		//01.16.02: Pass maximum length to string copy routine.		//		UAM_PStrCopy(inUAMArgs->Opt.open.zoneName, gZoneName, sizeof(gZoneName));	}	else    {        DbgPrint_((DBGBUFF, "No AppleTalk zone name passed to UAM"));		gZoneName[0] = 0;    }								gContextInited 		= false;		//Been through PwdDialog before?		gGuestLogon 		= false;	//Is guest our logon choice?	gDialog				= NULL;		//So we can see if we really got it.	gPwdDialogFilter	= NULL;	gLineItem			= NULL;	gAFPVersion[0]		= 0;	gUserName[0]		= 0;	gServerAddress		= inUAMArgs->Opt.open.srvrAddress;	gEventCallbackUPP	= inUAMArgs->callbacks->EventCallbackUPP;			gTriedKeychain		= false;		//	//Get the MSUAM preferences from the prefs file.	//	ZeroMemory(&gUAMPreferences, sizeof(UAM_PREFERENCES));	UAM_RetrieveUAMPreferenceData(&gUAMPreferences);	UAM_KCInitialize(inUAMArgs);        gPwdDialogFilter	= NewModalFilterUPP((ModalFilterUPP)MS_PwdDialogFilter);    gLineItem			= NewUserItemUPP((UserItemUPP)UAM_FrameDialogItem);	//	//Get the AFP version and the default user name. This function finds	//a match which is the highest AFP version supported by both the client	//and server.	//	UAM_GetAFPVersionString(				inUAMArgs->Opt.open.srvrInfo,			inUAMArgs->callbacks,			gAFPVersion,			gUserName 			);		//	//gUserName can be null, we just capture here during debugging to	//ensure we're getting the name properly.	//	Assert_(PSTR_LENGTH(gUserName) != 0);	Assert_(PSTR_LENGTH(gAFPVersion) != 0);		if (PSTR_LENGTH(gAFPVersion) == 0)	{		//		//No AFPVersion, no logon...		//				UAM_ReportError(uamErr_NoAFPVersion);		return(userCanceledErr);	}						gSupportsChngPwd = ((inUAMArgs->Opt.open.srvrInfo->fFlags & kSupportsChngPswd) != 0);													//	//Get the list of supported UAMs from a utility routine. This data	//is necessary in the password dialog code.	//								UAM_GetSupportedUAMS(			inUAMArgs,            gAFPVersion,			&gSupportedUAMs			);		//	//We should never get here if the following is false, but we	//check just to be on the safe side.	//	if (	((gSupportedUAMs & kMSUAMSupported) 	== 0)	&&			((gSupportedUAMs & kMSUAM_V2_Supported) == 0)	) 	{		Assert_(0);				UAM_ReportError(afpBadUAM);		return(userCanceledErr);	}		UAM_VersionString(gUAMVersionString);        //    //Set the maximum allowed password length. This may be changed and reset    //later by the change password UI.    //    UAM_SetMaximumPasswordLength(false);    	//	//This is how we tell AppleShare what our UAM supports. We have	//our own password dialog, we support change password, and we	//use our own change password dialog.	//		theUAMConfig |= BIT_0;	//Custom login dialog	theUAMConfig |= BIT_2;	//We support change password	theUAMConfig |= BIT_3;	//Custom change password dialog		inUAMArgs->result = theUAMConfig;			return(noErr);}// ---------------------------------------------------------------------------//		� MS_UAMClose()// ---------------------------------------------------------------------------//	Like UAMOpen, UAMClose has no specific purpose as defined by the device//	manager. We use it to clean up our allocated storage and globals.void MS_UAMClose(void){    DbgPrint_((DBGBUFF, "MS_UAMClose() enter..."));        //    //Release memory allocated for the password edit stuff.    //    UAM_CleanupPasswordFieldItems();        if (gDialog != NULL)    {        //        //If we put up our login dialog, get rid of it.        //        DisposeDialog(gDialog);            }        if (gPwdDialogFilter)	DisposeRoutineDescriptor(gPwdDialogFilter);    if (gLineItem)			DisposeRoutineDescriptor(gLineItem);}// ---------------------------------------------------------------------------//		� MS_TestTextEntryLength()// ---------------------------------------------------------------------------//	We don't allow entries in the login dialog to be of any length. This//	function tests the length of the entered text in an edit field. It will//	modify the event record appropriately to inhibit processing of the//	keystroke if it will take the field over the limit.Boolean MS_TestTextEntryLength(	DialogPtr 		inDialog,	EventRecord*	inEvent,	short 			inItem,	short			inMaxLengthAllowed){	Str255	theString;	Boolean	theResult 	= false;        	if (GetDialogKeyboardFocusItem(inDialog) == inItem)	{            UAM_GetDialogEditText(inDialog, inItem, theString);                        if (PSTR_LENGTH(theString) >= inMaxLengthAllowed)            {                SysBeep(1);                        inEvent->what 		= nullEvent;                inEvent->message	= 0;                theResult 	  		= true;                                //                //If the string length is currently too long, then                //we have to chop off the end of the string to the                //proper length.                //                if (PSTR_LENGTH(theString) > inMaxLengthAllowed)                {                    //                    //Easy way out, pascal style strings have length                    //byte in front. Just adjust it.                    //                    theString[0] = inMaxLengthAllowed;                                        //                    //Now set the text in the edit field with the new string.                    //                    UAM_SetDialogEditText(inDialog, inItem, theString);                }            }	}		return(theResult);}// ---------------------------------------------------------------------------//		� MS_PwdDialogFilter()// ---------------------------------------------------------------------------//	Filter function for the password dialog. We have this so we can capture//	command keys and keep length requirements for the user name in the login //	dialog.pascal Boolean MS_PwdDialogFilter(DialogRef inDialog, EventRecord *inEvent, short *inItem){	short	theCode;	Str255	theString;	Boolean	theResult = false;    SInt16	theMaxUserNameLength;        //    //For MS2.0 and 3.0 we allow 64 character passwords. 32 characters    //for Microsoft V1.0.    //    theMaxUserNameLength = (gSupportedUAMs & (kMSUAM_V2_Supported | kMSUAM_V3_Supported)) ?                                UAM_USERNAMELEN_V3 : UAM_USERNAMELEN;			if ((inEvent->what == keyDown) || (inEvent->what == autoKey))	{		theCode = (inEvent->message & charCodeMask);        		//		//04.30.01: Don't allow cmd selections when autoKey'ing.		//		if ((inEvent->modifiers & cmdKey) && (inEvent->what != autoKey))		{			switch(theCode)			{				case 'g':				case 'G':					*inItem   = DITEM_GuestRadio;					theResult = true;					break;								case 'r':				case 'R':					*inItem   = DITEM_RegRadio;					theResult = true;					break;									case 's':				case 'S':					*inItem	  = DITEM_ChangePwd;					theResult = true;					break;													case 'a':				case 'A':					*inItem   = DITEM_Keychain;					theResult = true;					break;								//				//Handle edit commands from the user. We don't allow any				//editing commands in the password field. This mimicks				//Apple's own UAM's.				//								case 'c':				case 'C':				case 'v':				case 'V':				case 'x':				case 'X':                    theResult = true;                    break;									default:					break;			}		}		else		{			//			//Don't allow more than UAM_USERNAMELEN maximum characters in edit field.			//						if (GetDialogKeyboardFocusItem(inDialog) == DITEM_UserName)			{				UAM_GetDialogEditText(inDialog, DITEM_UserName, theString);								switch(theCode)				{					case UAMKey_BackDel:					case UAMKey_Left:					case UAMKey_Right:					case UAMKey_Return:					case UAMKey_Enter:					case UAMKey_Escape:					case UAMKey_Tab:						break;											default:                        if (PSTR_LENGTH(theString) >= theMaxUserNameLength)                        {                            SysBeep(1);                                                    inEvent->what = nullEvent;                            theResult 	  = true;                        }						break;				}			}		}	}	else	{		if (gEventCallbackUPP)		{			//			//If we're not handling the event ourselves, then call the			//event callback which gives AS and the Chooser a chance			//to update it's windows, etc.			//			            #ifndef UAM_TARGET_CARBON            CallUniversalProc(gEventCallbackUPP, kEventCallbackProcInfo, inEvent);            #else			gEventCallbackUPP(inEvent);            #endif		}	}        if (theResult == FALSE)    {        theResult = StdFilterProc(inDialog , inEvent, inItem);    }		return(theResult);}// ---------------------------------------------------------------------------//		� MS_UAMPwdDialog()// ---------------------------------------------------------------------------//	This is where we put up our password dialog. The buffers pointed to by//	'inUserName' and 'inPassword' end up getting passed directly to the//	UAMLogin function.////	The buffer passed for the user name and password is 64 bytes long. Don't//	use more than that! OSStatus MS_UAMPwdDialog(UAMArgs *inUAMArgs){	short			theItem, x;	Str255			theStr;	OSStatus		theError	= noErr;	Boolean			theLoop 	= true;        DbgPrint_((DBGBUFF, "MS_UAMPwdDialog() enter..."));		Assert_(gPwdDialogFilter 	!= NULL);	Assert_(gLineItem 			!= NULL);			//	//Determine which user name to use, the default or the	//one supplied by the client (if any). gUserName is filled	//in originally during the UAMOpen call.	//	if (PSTR_LENGTH(inUAMArgs->Opt.pwDlg.userName) != 0)	{		//		//01.16.02: Pass maximum length to string copy routine.		//		UAM_PStrCopy(inUAMArgs->Opt.pwDlg.userName, gUserName, sizeof(gUserName));	}		//	//If we already tried the keychain and failed, we don't want	//to try again or we'll loop forever. Give the user a chance	//to enter the correct name and password.	//	//NOTE: We check to see if the cmd key is down, if it is, then	//we bypass the keychain stuff alltogether. Maybe the user wants	//to change his password!?!?!	//	if ((gTriedKeychain == false) && (UAM_KCAvailable()) && (!UAM_KeyDown(KEY_Command)))	{		gTriedKeychain = true;				if ( (PSTR_LENGTH(inUAMArgs->Opt.pwDlg.userName) 	!= 0)	&&			 (strlen((char*)inUAMArgs->Opt.pwDlg.password)	!= 0)	)		{			//			//We were supplied a username and password by the AFP			//client. This means the user clicked a keychain entry.			//			goto exit;		}		else 		{            DbgPrint_((DBGBUFF, "Checking keychain for stored credentials..."));			theError = UAM_KCFindAppleSharePassword(							gUserName,							inUAMArgs->Opt.pwDlg.password,							gServerName,							NULL			);						if (theError == noErr)			{				DbgPrint_((DBGBUFF, "Credentials found in the keychain!"));								//				//Fill in the user name for the UAMArgs.				//				if (PSTR_LENGTH(inUAMArgs->Opt.pwDlg.userName) == 0)				{					UAM_PStrCopy(gUserName, inUAMArgs->Opt.pwDlg.userName);				}								//				//The password must always passed back in UAMArgs as a C string.				//				_p2cstr(inUAMArgs->Opt.pwDlg.password);				//				//A password was found so try to logon.				//				goto exit;			}			else if (	(theError != errKCItemNotFound)		&&						(theError != userCanceledErr)		)			{				//				//Only report "real" errors.				//				UAM_ReportError(theError);			}            else            {                DbgPrint_((DBGBUFF, "No credentials found in the keychain!"));            }		}	}	else if ((UAM_KCAvailable()) && (UAM_KeyDown(KEY_Command)))	{		//		//If the user is holding the cmd key down, then we don't want to		//try the keychain the next time through either.		//		gTriedKeychain = true;	}				//	//Display the server name in the dialog title text	//which is located at the top of the dialog. This must be	//done even if we've been here before.	//		ParamText(gServerName, NULL, NULL, NULL);		//	//If we haven't been through here before, then we need to do	//all the prep work.	//			if (!gContextInited)	{		gDialog = GetNewDialog(DLOG_Login, NULL, (WindowPtr)-1);        		if (gDialog == NULL)		{            DbgPrint_((DBGBUFF, "Failed to get logon dialog resource!"));            			//			//If we couldn't get the dialog, then we're either out			//of memory or the resource couldn't be found.			//						theError = MemError();			if (theError == noErr)				theError = ResError();			if (theError == noErr)				theError = resNotFound;						UAM_ReportError(theError);			return(userCanceledErr);		}				//		//Setup our custom user item that displays the divider line at the        //top of the dialog window.        //			UAM_SetupDialogUserItem(gDialog, DITEM_Line, gLineItem, userItem);        				//		//Set up the default user name and password (if any). If a user name		//exists, then make the password field the active field ready for input.		//        		//		//If we've not been here before, then we want to use the user name		//entered in the Sharing Setup Control Panel (or Chooser).		//				if (PSTR_LENGTH(gUserName) != 0)		{			UAM_SetDialogEditText(gDialog, DITEM_UserName, gUserName);            UAM_MakeEditItemFocus(gDialog, DITEM_UserName);		}		else		{			UAM_HiliteDialogControlItem(gDialog, 1, 255);		}		        //        //Initialize the password edit control so it behaves properly.        //        UAM_InitializeDialogPasswordItem(gDialog, DITEM_Password);		//		//Now we set up the guest and registered user radio buttons and the		//change password button as determined by UAM_GetServerInfo().		//				if (!gSupportsChngPwd)        {            //            //Change password is not supported by the server for this user. We            //gray out the button to make it unselectable.            //			UAM_HiliteDialogControlItem(gDialog, DITEM_ChangePwd, 255);		}				if (!(gSupportedUAMs & kGuestSupported))		{			//			//No guest support, we don't need the guest radio button.			//			UAM_HiliteDialogControlItem(gDialog, DITEM_GuestRadio, 255);		}				//		//Set the initial radio for the default/current login method.		//				if (gGuestLogon)		{			UAM_SetDialogControlValue(gDialog, DITEM_GuestRadio, 1);			UAM_SetDialogControlValue(gDialog, DITEM_RegRadio, 	 0);						UAM_HiliteDialogControlItem(gDialog, DITEM_ChangePwd, 255);						for (x = DITEM_FirstHideItem; x <= DITEM_LastHideItem; x++) {				HideDialogItem(gDialog, x);			}						UAM_HiliteDialogControlItem(gDialog, 1, 0);		}		else {			UAM_SetDialogControlValue(gDialog, DITEM_RegRadio, 1);		}        		//		//If the client is not allowed to save password for this server,		//then we gray out the keychain checkbox.		//		if (UAM_KCAvailable() == false)		{			UAM_HiliteDialogControlItem(gDialog, DITEM_Keychain, 255);		}		else if (gTriedKeychain)		{            UAM_SetPasswordText(gDialog, DITEM_Password, "\p");            UAM_MakePasswordItemFocusItem(gDialog, DITEM_Password);		}        		//		//Set the encryption type checkbox to the proper value.		//		if (gUAMPreferences.flags & UAM_PREFS_REQUIRE_STRONG_ENCRYPTION)		{			UAM_SetDialogControlValue(gDialog, DITEM_StrongEncryption, 1);		}        SetDialogDefaultItem(gDialog, DITEM_OK);        SetDialogCancelItem(gDialog, DITEM_Cancel);				//		//This flag lets up know that we've initialized our login dialog		//and that we don't need to do it again when/if we come here again.		//				gContextInited	= true;                SetPortDialogPort(gDialog);        ShowWindow(GetDialogWindow(gDialog));	}	else	{		UAM_SetDialogEditText(gDialog, DITEM_UserName, gUserName);        UAM_SetPasswordText(gDialog, DITEM_Password, "\p");				//		//Hilite the password selection.		//		UAM_MakePasswordItemFocusItem(gDialog, DITEM_Password);	}		do	{		ModalDialog(gPwdDialogFilter, &theItem);				switch(theItem)		{			case DITEM_OK:								gGuestLogon 	= (UAM_GetDialogControlValue(gDialog, DITEM_GuestRadio) != 0);				theError		= noErr;				theLoop 		= false;								if (gGuestLogon)				{					inUAMArgs->Opt.pwDlg.userName[0] = 0;					inUAMArgs->Opt.pwDlg.password[0] = 0;				}				else				{										UAM_GetPasswordText(	gDialog,                                            DITEM_Password,                                            inUAMArgs->Opt.pwDlg.password	);										UAM_GetDialogEditText(	gDialog,                                            DITEM_UserName,                                            inUAMArgs->Opt.pwDlg.userName	);                    //                    //Don't allow blank passwords for 2 reasons:                    //                    //	1. It is not secure and user can use Apple UAM for this.                    //	2. This covers a security hole that is generated after changing                     //	   passwords on these servers using Apple's clear text UAM.                    //                    if (PSTR_LENGTH(inUAMArgs->Opt.pwDlg.password) == 0)                    {                        UAM_StandardAlert(                                uamErr_PasswordMessage,                                uamErr_NoBlankPasswordsAllowed,                                NULL                                );                                                        //                        //Continue the login dialog loop so the user can make changes.                        //                        theLoop = true;                    }                                        //                    //The password must always be in C style string form.                    //                    _p2cstr(inUAMArgs->Opt.pwDlg.password);				}				//				//Only save the preferences if the user hits "Connect".				//				UAM_SaveUAMPreferenceData(&gUAMPreferences);				break;							case DITEM_Cancel:				//				//VERSION 5.0: To force cancellation, we pass userCanceledError(-128)				//back to the Chooser. The old UAM would pass back dsForcedQuit which				//is the wrong value. This would cause an error dialog when cancelling.				//				DbgPrint_((DBGBUFF, "Cancel selected in login dialog."));                				theError	= userCanceledError;				theLoop 	= false;				break;						case DITEM_GuestRadio:				//				//Set up the controls in the dialog for guest login. We don't				//need the user name and password items, so hide them from				//the user. We must explicitly enable the 'OK' button since				//it may have been disabled by the gate stuff.				//								if (UAM_IsDialogControlActive(gDialog, DITEM_GuestRadio))				{					UAM_SetDialogControlValue(gDialog, DITEM_GuestRadio, 1);					UAM_SetDialogControlValue(gDialog, DITEM_RegRadio, 	 0);															for (x = DITEM_FirstHideItem; x <= DITEM_LastHideItem; x++) {						HideDialogItem(gDialog, x);					}				}				break;						case DITEM_RegRadio:				//				//Now we need all the items back that were hidden above, make				//them visible.				//				if (UAM_GetDialogControlValue(gDialog, DITEM_RegRadio) <= 0)				{					UAM_SetDialogControlValue(gDialog, DITEM_GuestRadio, 0);					UAM_SetDialogControlValue(gDialog, DITEM_RegRadio, 	 1);										for (x = DITEM_FirstHideItem; x <= DITEM_LastHideItem; x++) {						ShowDialogItem(gDialog, x);					}																		UAM_GetDialogEditText(gDialog, DITEM_UserName, theStr);                    UAM_MakeEditItemFocus(gDialog, DITEM_UserName);										if ((gSupportsChngPwd) && (theStr[0] != 0)) {						UAM_HiliteDialogControlItem(gDialog, DITEM_ChangePwd, 0);					}				}				break;							case DITEM_ChangePwd:				UAM_GetPasswordText(	gDialog,                                        DITEM_Password,                                        inUAMArgs->Opt.pwDlg.password	);				UAM_GetDialogEditText(	gDialog,                                        DITEM_UserName,                                        inUAMArgs->Opt.pwDlg.userName	);								theError = UAM_ChangePwd(inUAMArgs);                				switch(theError)				{					case CHNGPSWD_USER_CANCELED:						break;										case CHNGPSWD_UPDATE_KEYCHAIN:						//						//We need to re-add the keychain item with the						//correct password. Flag it by checking the box.						//						UAM_SetDialogControlValue(gDialog, DITEM_Keychain, 1);												//						//Just fall on through and handle the normal case.						//											case CHNGPSWD_NOERR:						//						//Set the password field and buffer with the new password in case						//we end back here later.						//												UAM_SetPasswordText(gDialog, DITEM_Password, inUAMArgs->Opt.pwDlg.password);							                    //	                    //The password must always be in C style string form.	                    //	                    _p2cstr(inUAMArgs->Opt.pwDlg.password);						theError = noErr;						theLoop  = false;						break;											default:						UAM_ReportError(theError);												//						//Make sure in debug that the password field is cleared in						//an error condition.						//						Assert_(PSTR_LENGTH(inUAMArgs->Opt.pwDlg.password) == 0);												//						//We do this here so we clear the password field like we should						//do whenever there is a failure.						//						UAM_SetPasswordText(gDialog, DITEM_Password, "\p");												//						//Because we use ParamText() we must manually force an update						//of the dialog or things won't redraw properly.						//						#ifndef UAM_TARGET_CARBON						InvalRect(&gDialog->portRect);						#endif						break;				}							//				//Must reset our user's name since UAM_ChangePwd() uses ParamText()				//to set some strings of it's own.				//								ParamText(gServerName, NULL, NULL, NULL);				break;										case DITEM_Keychain:				UAM_ToggleDialogControl(gDialog, DITEM_Keychain);				break;						case DITEM_StrongEncryption:				//				//Update the required encryption dynamically.				//				UAM_ToggleDialogControl(gDialog, DITEM_StrongEncryption);								if (UAM_GetDialogControlValue(gDialog, DITEM_StrongEncryption) > 0)				{					gUAMPreferences.flags |= UAM_PREFS_REQUIRE_STRONG_ENCRYPTION;				}				else				{					gUAMPreferences.flags &= ~UAM_PREFS_REQUIRE_STRONG_ENCRYPTION;										UAM_StandardAlert(						uamErr_WARNINGMessage,						uamErr_UsingWeakAuthentication,						NULL						);				}				break;			default:				break;		}			}while(theLoop);exit:    DbgPrint_((DBGBUFF, "Exit MS_UAMPwdDialog() return code = %d", (int)theError));	return(theError);}