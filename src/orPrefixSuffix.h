!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 ORPrefixSuffix [Z, GLULX]
! Allows the use of prefixes and suffixes that are followed by periods (for example,
! "Dr. Bob" or "Col. Mustard" or "Dan Jr." or "Mr. Armstrong")
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! When creating an object with a period in the name -- like "Mrs. Robinson" --  the
! player often has issues.  For instance:
!
!     >examine Mrs. Robinson
!
! will cause the parser to stop parseing the input at the period and treat it as two
! separate commands.  "examine mrs" and "Robinson."  Thus, assuming there is only one
! object which the word "Mrs" can refer to, then you will get the description followed
! by the the "That's not a verb I recognize" error message. The parser fails entirely
! if the prefix can refer to multiple objects ("Mrs. Robinson" standing next to
! "Mrs. Baker").
!
! This module scans the input and removes periods that follow general prefixes, such as
! "Mr" "Mrs" "Dr" "Col", etc...
!--------------------------------------------------------------------------------------
! Revision History
! 2002.10.13	Initial Creation
! ...			History from orLibrary v1 snipped away.j
! 2024.03.1	Refactored for the orLibrary version 2.
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orPrefixSuffix_STAGE 0;
!--------------------------------------------------------------------------------------
! INCLUDED DEPENDENCIES
#include "_orInfExt";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orPrefixSuffix_STAGE	; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orPrefixSuffix_STAGE < LIBRARY_STAGE);
   #undef      orPrefixSuffix_STAGE ;
   Constant    orPrefixSuffix_STAGE LIBRARY_STAGE;
   #ifdef      orPrefixSuffix_STAGE ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orPrefixSuffix...";#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
	orInfExt with ext_beforeparsing[ pos lstwrd curwrd;
		wn=1;
		while(wn<=num_words){ !--search for a period
			curwrd = NextWord();
			pos=WordAddress(wn-1);
			if (curwrd == THEN1__WD && lstwrd=='lt' or 'bro' or 'capt' or 'msgr' or 'st' or 'sen' or 'cmdr' or 'col' or 'dr' or 'mrs' or 'mr' or 'ms' or 'rev' or 'sgt' or 'jr' or 'sr') {
				0->pos=' '; !--turn the period into a space
			}
			lstwrd=curwrd;
		}
		Tokenise__(buffer, parse);
		rfalse; !
	];
#endif; !--AFTER_Verblib
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE