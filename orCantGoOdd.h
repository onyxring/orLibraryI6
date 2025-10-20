!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.08.10 orCantGoOdd
! A modification to the library's cant_go behavior to distinguish between "normal"
! directions and "odd" directions (such as up, down, in, and out).
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! Default treatment of the cant_go property is a bit odd...
!
!	You are in a small forest, surrounded by trees.  Only to the north
!	is there a passage.
!
!	>s
!	Trees block travel in that direction.
!
!	>in
!	Trees block travel in that direction.
!
!	>down
!	Trees block travel in that direction.
!
!	>up
!	Trees block travel in that direction.
!
! The orCantGoOdd extension causes your game to ignore the cant_go property if the
! direction attempted is up, down, in, or out.  Instead, the value of the cantGoOdd
! property is printed, if provided, or the newly added default message (#Go, 13)
! otherwise.
!--------------------------------------------------------------------------------------
! Revision History
! 2003.02.08	Initial Creation
! 2024.08.10	Refactored for the orLibrary version 2.
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orCantGoOdd_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orHookVerbs";
#include "_orInfExt";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orCantGoOdd_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orCantGoOdd_STAGE  < LIBRARY_STAGE);
   #undef      orCantGoOdd_STAGE  ;
   Constant    orCantGoOdd_STAGE  LIBRARY_STAGE;
   #ifdef      orCantGoOdd_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orCantGoOdd..."; #endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_PARSER);
	property individual cant_go_odd;

	orInfExt with ext_messages[;
				Go: if(lm_n==13) print_ret "It",(string)IS__TX," not possible to go that way.";
			]
		,	ext_goSubNoDir[thedir;
				if(noun==u_obj or d_obj or in_obj or out_obj){ !--if one of the odd directions
					if(actor~=player) rfalse; !--TODO: should we intervine if its an NPC trying to walk into walls?  CanPlayerWitness()?
					if(location provides cant_go_odd && location.cant_go_odd)
						PrintOrRun(location, cant_go_odd);
					else
						L__M(##GO,13,thedir);
					rtrue;
				}
				rfalse;
			]
	;
#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE

