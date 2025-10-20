!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.07.03 orTransition
! orTransition() is best used as a special effect for dividing story parts.  It conveys
! to the player a transition between logical breaks in the game (for instance,
! chapters). It can be called after printing the final lines of a section, and before
! the first lines of the next.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License:
!--------------------------------------------------------------------------------------
! The general use of the routine is as follows:
!	1) Text of Section A is printed.
!	2) orTransition() prints a small text seperation then prompts for the player to
!		press space.
!	3) The screen is cleared and the text of Section B is then printed.
! Note:
! orTransition can take a single parameter for text to be used in the tranition.
! If no text is supplied the three dashes will be used.
!--------------------------------------------------------------------------------------
! Revision History
! 2002.05.12	Initial Creation
! ...			History from orLibrary v1 snipped away.
! 2024.07.03	Refactored for the orLibrary version 2.
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orTransition_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "orCenter";
#include "orUtilUi";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orTransition_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orTransition_STAGE  < LIBRARY_STAGE);
   #undef      orTransition_STAGE  ;
   Constant    orTransition_STAGE  LIBRARY_STAGE;
   #ifdef      orTransition_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orTransition...";#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
	system_file;
	[orTransition text prompt skipErase ch;
		new_line;
		new_line;
		if(text~=0) {
			orCenter(text);
			new_line;
			new_line;
		}
		if(prompt==0) prompt="Press any key to continue";
		orCenter(prompt);
		ch=util.orUI.pauseForInput();
		if(skipErase==false) util.orUI.eraseMain();
	];
#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE