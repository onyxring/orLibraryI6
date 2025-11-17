!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.07.09 orParse
! A utility object with routines useful for writing code which affects parsing.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! This extension, classified as a "utility", provides tooling for other extensions to
! use and offers no specific features by itself.
!
! It provides functions wrapped into objects.  Specifically:
!	orParse contains routines useful for writing code which affects parsing.
!--------------------------------------------------------------------------------------
! Revision History
! 2024.07.09	Initial Creation
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orUtilParser_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUtil";
#include "orUtilNum";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orUtilParser_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orUtilParser_STAGE  < LIBRARY_STAGE);
   #undef      orUtilParser_STAGE  ;
   Constant    orUtilParser_STAGE  LIBRARY_STAGE;
   #ifdef      orUtilParser_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orUtilParser...";#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
    object _orParser
    !-- excludeObjFromDisambiguation is a helper for the parsing hooks like ext_chooseObjects.
    !   It prevents an object from consideration when constructing a list of items to disambiguate (e.g., "Which do you mean...");
    !   More than just making it less likely to be selected, it removes it from consideration entirely; however, it does not resize 
	!   the match list, which is important for routines iterating over it.
    with excludeObjFromDisambiguation[obj i;
		for (i=0 : i<number_matched : i++) {
        	if (match_list-->i == obj) {
				match_list-->i =-1;
				match_scores-->i=0;
			}
    	}
		return 0;
	]
    !-- removeMatchPos is a helper for the disambiguation hooks like ext_adjudicate.  Conceptually, it is similar to 
    !   excludeObjFromDisambiguation but it actually removes the entry in the match_list.
	,	removeMatchPos[pos 
			t;
		for(t=pos:t<number_matched:t++) {
			match_list-->t= match_list-->(t+1);
			match_scores-->t= match_scores-->(t+1);
		}
		number_matched--;
	]

;

#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
