!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! YYYY.MM.DD utorNameable
! Unit Tests for the utorNameable orLibrary extension.
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
default        utorNameable_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUnitTest";
#include "orNameable";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        utorNameable_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       utorNameable_STAGE  < LIBRARY_STAGE);
   #undef      utorNameable_STAGE  ;
   Constant    utorNameable_STAGE  LIBRARY_STAGE;
   #ifdef      utorNameable_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   utorNameable...";#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
   object utorNameableStart "orNameable Room" has light with description "Where orNameable tests are run.";
   orNameable -> sword "sword" with name 'sword', 
      description "Sharp and pointy.";

orUnitTest "utorNameable" 
   with tests [; PlayerTo(utorNameableStart, 3); ] noTest
      "name the sword ~Excaliber~" "begin to think"
      "x Excaliber" "Sharp and pointy"
      "refer to the sword as Durandal" "begin to think" 
      "x Excaliber" "can't see any such"
      "x Durandal" "Sharp and pointy"
;

#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
