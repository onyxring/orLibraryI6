!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.08.09 orDistinctRead
! Generally, the commands Read and Examine are treated the same and are mapped to the
! same verb which simply displays the description.  This routine adds a behavior to
! makes a distinction between reading and just looking at an object.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! orDistinctRead relies upon a property called "readDescription".  If present,
! then the value of this property will be displayed in response to the READ command.
! If not, then a message is printed explaining that the object "cannot be read".
!
! If the readDescription equals description (the property name, not the text) then the READ
! verb will be redirected to EXAMINE.  This selectively mimics the normal behavior of
! the standard library.
!--------------------------------------------------------------------------------------
! Revision History
! 2002.01.03	Initial Creation
! ...			History from orLibrary v1 snipped away.
! 2024.06.19	Refactored for the orLibrary version 2
!--------------------------------------------------------------------------------------
! TODO: add a default message rather than a hard-coded response
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orDistinctRead_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES

!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orDistinctRead_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orDistinctRead_STAGE  < LIBRARY_STAGE);
   #undef      orDistinctRead_STAGE  ;
   Constant    orDistinctRead_STAGE  LIBRARY_STAGE;
   #ifdef      orDistinctRead_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orDistinctRead..."; #endif;

!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);
	property individual readDescription;
#endif; !before parser
!======================================================================================
! AFTER GRAMMAR
#iftrue (LIBRARY_STAGE == AFTER_GRAMMAR);
	object with read_value;
	Extend "read" replace
		* noun		->Read
	;
	[ReadSub;
		if(noun provides readDescription) {
			if(noun.readDescription==description) return ExamineSub();
			printorrun(noun,readDescription);
		} else {
			print (The)noun," cannot be read.^";
		}
	];
#endif; !--After GRAMMAR
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
