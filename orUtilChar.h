!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.04.14 orChar
! Various utility functions for dealing with Characters.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
! License:
!--------------------------------------------------------------------------------------
! This extension, classified as a "utility", provides tooling for other extensions to
! use and provides no specific features by itself.
!
! It provides functions focused on characters (letters, digits, and symbols rather than
! story characters), wrapped into the orChar object.
!--------------------------------------------------------------------------------------
! Revision History
! 2024.04.14	Initial creation.
!======================================================================================
! Extension Framework management
#ifndef orExtensionFramework_STAGE;
default        orUtilChar_STAGE  0;
!======================================================================================
! INCLUDE DEPENDENCIES
#include "_orUtil";

!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orUtilChar_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orUtilChar_STAGE  < LIBRARY_STAGE);
   #undef      orUtilChar_STAGE  ;
   Constant    orUtilChar_STAGE  LIBRARY_STAGE;
   #ifdef      orUtilChar_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orUtilChar...";#endif;

!======================================================================================
! AFTER VERBLIB
!--------------------------------------------------------------------------------------
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);

	object _orChar
		with isAlpha[c;
				if(c>='a' && c<='z') rtrue;
				if(c>='A' && c<='Z') rtrue;
				rfalse;
			]
		,	isNumeric[c;
				if(c>='0' && c<='9') rtrue; rfalse;
			]
		,	isAlphaNumeric[c;
				if(self.isAlpha(c)==true) rtrue;
				if(self.isNumeric(c)==true) rtrue;
				rfalse;
			]
		,	isLower[c;
				if(c>='a' && c<='z') rtrue; rfalse;
			]
		,	isUpper[c;
				if(c>='A' && c<='Z') rtrue; rfalse;
			]
		,	toUpper[c;
				if(self.isLower(c)) c=UpperCase(c);
				return c;
			]
		,	toLower[c;
				if(self.isUpper(c)) c=LowerCase(c);
				return c;
			]
	;
	#endif; !--AFTER VERBLIB
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE


