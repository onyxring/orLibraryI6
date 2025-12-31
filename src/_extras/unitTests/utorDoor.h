!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2025.12.31 utorDoor
! Unit Tests for the utorDoor orLibrary extension.
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
default        utorDoor_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUnitTest";
#include "orDoor";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        utorDoor_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       utorDoor_STAGE  < LIBRARY_STAGE);
   #undef      utorDoor_STAGE  ;
   Constant    utorDoor_STAGE  LIBRARY_STAGE;
   #ifdef      utorDoor_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   utorDoor...";#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
   object utorDoorStart "The Study" has light with description "Where orDoor tests are run.",  w_to SteelDoor_ordr; 
   object Lab_ordr "The Laboratory" has light with description "...", e_to SteelDoor_ordr; 
   orDoor SteelDoor_ordr "steel door" with name 'steel' 'door';

orUnitTest "utorDoor" 
   with tests [; PlayerTo(utorDoorStart, 3); ] noTest 
      "open door" "You open the steel door."     
      "w" "The Laboratory" 
;

#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
