!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.07.19 orFirstImpressions
! Add "first impressions" to object descriptions, which are not usually repeated during
! subsequent examinations.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! Defines the preImpression and postImpression properties which, if defined, are
! printed before and after a description, the first time the player sees it and
! subsequently ignored.
!
! Complementary extensions:
! *	orReview : If included, the REVIEW verb will show this initial text.
! * orPrint  : If included, the preImpression and postImpression text can contain print
!				patterns.
!--------------------------------------------------------------------------------------
! Revision History
! 2001.08.13	Initial Creation
! ...			History from orLibrary v1 snipped away.
! 2024.07.19	Refactored for the orLibrary version 2.
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orFirstImpressions_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orHookVerbs";
#include "_orTrackSeen";
#include "_orInfExt";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orFirstImpressions_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orFirstImpressions_STAGE  < LIBRARY_STAGE);
   #undef      orFirstImpressions_STAGE  ;
   Constant    orFirstImpressions_STAGE  LIBRARY_STAGE;
   #ifdef      orFirstImpressions_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orFirstImpressions...";#endif;
!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);
	property individual preImpression;
   	property individual postImpression;
#endif; !--Before Parser
!======================================================================================
! AFTER PARSER
#iftrue (LIBRARY_STAGE == AFTER_PARSER);
	orInfExt with ext_preDescription[;
		if(noun==0||action==##Look or ##Go) noun=location;
		if(noun hasnt beenSeen && noun provides preImpression && noun.preImpression) {

			if(metaclass(noun.preImpression)==Routine)
				PrintOrRun(noun,preImpression,true);
			else{
				#ifdef orPrint_STAGE;
					orPrint(noun.preImpression);
				#ifnot;
					print (string)noun.preImpression;
				#endif;
			}
		}
		rfalse;
	]
	,	ext_postDescription[;
		if(noun==0||action==##Look or ##Go) noun=location;
		if(noun hasnt beenSeen && noun provides postImpression && noun.postImpression) {

			if(metaclass(noun.postImpression)==Routine) PrintOrRun(noun,postImpression,true);
			else{
				#ifdef orPrint_STAGE;
					orPrint(noun.postImpression);
				#ifnot;
					print (string)noun.postImpression;
				#endif;
			}

		}
		rfalse;
	]
;

#endif; !--After Parser
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE