!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2025.12.31 utorExits
! Unit Tests for the utorExits orLibrary extension.
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
default        utorExits_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUnitTest";
#include "orExits";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        utorExits_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       utorExits_STAGE  < LIBRARY_STAGE);
   #undef      utorExits_STAGE  ;
   Constant    utorExits_STAGE  LIBRARY_STAGE;
   #ifdef      utorExits_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   utorExits...";#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
   object utorExitsStart "orExits Room" has light with description "Where orExits tests are run."
      ,  n_to laboratory_exts
      ,  w_to study_exts
      ,  e_to kitchen_exts
      ,  s_to "the wall"
   ;
   object laboratory_exts;
   object study_exts;
   object kitchen_exts;
   
orUnitTest "utorExits" 
   with tests [; PlayerTo(utorExitsStart, 3); ] noTest
      "exits" "The only exits lay north, east, and west."
;

#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
