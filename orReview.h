!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 orReview
! An implementation of the Review Command.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! The REVIEW command acts as a meta version of LOOK.  That is, it
! enables the player to review descriptions without the passages of time.
! This covers both objects and room descriiptions. The following transcript, occuring
! without the number of moves increasing, demonstrates.  It assumes that the player
! has already examined the table and the room, but not the chair.
!
!    >REVIEW
!	 Room
!    A basic room.
!
!    You can see a table and chair here.
!
!    >REVIEW TABLE
!    The wooden table is old and stained.
!
!    >REVIEW CHAIR
!    You have not already examined the chair.
!
!--------------------------------------------------------------------------------------
! Revision History
! 2004.02.07	Initial Creation
! ...			History from orLibrary v1 snipped away.
! 2024.07.10	Refactored for the orLibrary version 2.
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orReview_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES

!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orReview_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orReview_STAGE  < LIBRARY_STAGE);
   #undef      orReview_STAGE  ;
   Constant    orReview_STAGE  LIBRARY_STAGE;
   #ifdef      orReview_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orReview...";#endif;
!======================================================================================
! AFTER GRAMMAR
#iftrue (LIBRARY_STAGE == AFTER_GRAMMAR);

	Verb meta 'review'
		* -> Review
		* noun -> ReviewObj
	;
	[ ReviewSub;
		give location ~beenSeen;
		LookSub();
		give location beenSeen;
	];
	[ ReviewObjSub;
		if(noun hasnt beenSeen) {
			print "You have not already examined ",(the)noun,".^";
			return;
		}
		give noun ~beenSeen;
		ExamineSub();
		give noun beenSeen;
	];
#endif; !--After Grammar
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE