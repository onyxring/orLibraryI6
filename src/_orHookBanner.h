!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.12.28 orHookBanner
! Extends the standard library's banner routine, adding a user-defined value to append
! to the bottom of the output and orLib versioning too.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! Revises the game banner, using the orBannerText global, if defined, to add custom
! text to the banner and displaying the version information of the orLibrary.
! Additionally, it supports a new extension "hook," ext_bannerText, for extensions to
! use.
!--------------------------------------------------------------------------------------
! Revision History
! 2001.07.03	Initial Creation
! ...			History from orLibrary v1 snipped away.
! 2024.03.23	Refactored for the orLibrary version 2.
! 2024.12.28	orPrint support or orBannerText.
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orHookBanner_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES

!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orHookBanner_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orHookBanner_STAGE  < LIBRARY_STAGE);
   #undef      orHookBanner_STAGE  ;
   Constant    orHookBanner_STAGE  LIBRARY_STAGE;
   #ifdef      orHookBanner_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orHookBanner..."; #endif;
!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);
	replace Banner _oldBanner;
	property individual ext_bannerText;
#endif; !--Before Parser
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
	default orLibVersion "undefined";
	default orLibDateStamp "undefined";
	default orBannerText 0;
	[ Banner e;
		_oldBanner();

		#ifdef orLibVersion;
		print "orLibrary for I6 ",(string)orLibVersion," (",(string)orLibDateStamp,") ";
		style underline; print "compiled for ";
		#ifdef TARGET_GLULX; print "GLULX"; #ifnot; print "Z-Machine"; #endif;
		style roman;
		new_line;
		#endif;

		objectloop(e in LibraryExtensions && e provides ext_bannerText)
			PrintOrRun(e, ext_bannerText);

		if(orBannerText~=0) {
			switch(metaclass(orBannerText)){
				routine: orBannerText();
				string:
				#ifdef orPrint_STAGE;
					orPrint(orBannerText);
				#ifnot;
					print (string) orBannerText;
				#endif;
			}
		}
		new_line;
	];
#endif; !--AFTER_VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE