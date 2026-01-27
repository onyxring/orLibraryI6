!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2025.12.31 utorCantGoOdd
! Unit Tests for the utorCantGoOdd orLibrary extension.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! Revision History
! 2024.05.06	Initial Creation
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
#ifndef        utorCantGoOdd_STAGE;
default        utorCantGoOdd_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUnitTest";
#include "orCantGoOdd";
!--------------------------------------------------------------------------------------
#endif;
#ifnot;
#ifndef        utorCantGoOdd_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       utorCantGoOdd_STAGE  < LIBRARY_STAGE);
   #undef      utorCantGoOdd_STAGE  ;
   Constant    utorCantGoOdd_STAGE  LIBRARY_STAGE;
   #ifdef      utorCantGoOdd_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   utorCantGoOdd...";#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
   object utorCantGoOddStart "orCantGoOdd Room" has light 
   with description "You are in a small forest."
   ,    cant_go "Trees block travel in that direction."
   ,    cant_go_odd "Reality blocks travel in that direction.";

orUnitTest "utorCantGoOdd" 
   with tests [; PlayerTo(utorCantGoOddStart, 3); ] noTest 
      "w" "Trees block"   
      "up" "Reality blocks"   
;

#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
