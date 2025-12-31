!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2025.12.31 utorGotoLocation
! Unit Tests for the utorGotoLocation orLibrary extension.
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
default        utorGotoLocation_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUnitTest";
#include "orGotoLocation";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        utorGotoLocation_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       utorGotoLocation_STAGE  < LIBRARY_STAGE);
   #undef      utorGotoLocation_STAGE  ;
   Constant    utorGotoLocation_STAGE  LIBRARY_STAGE;
   #ifdef      utorGotoLocation_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   utorGotoLocation...";#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
   !object utorGotoLocationStart "orGotoLocation Room" has light with description "Where orGotoLocation tests are run.";

   object laboratory_goloc "Laboratory" has light
   with name 'lab' 'laboratory',
   description "...",
   s_to livingRoom_goloc;

   object kitchen_goloc "Kitchen" has light
   with name 'kitchen',
   description "...",
   w_to livingRoom_goloc;

   object livingRoom_goloc "Living Room" has light
   with name 'living' 'room',
   description "...",
   w_to study_goloc, n_to laboratory_goloc, e_to kitchen_goloc;

   object study_goloc "Study" has light
   with name 'study' 'office',
   description "...",
   e_to livingRoom_goloc, w_to bathroom_goloc, n_to closet_goloc;

   object closet_goloc "Closet" has light
   with name 'closet' 'wardrobe'
   description "...",
   w_to study_goloc;

   object bathroom_goloc "Bathroom" has light
   with name 'bath' 'bathroom',
   description "...",
   e_to study_goloc;


orUnitTest "utorGotoLocation" 
   with tests [; PlayerTo(laboratory_goloc, 3); ] noTest
      "s. go west. w" "Bathroom"
      "go to laboratory" "Laboratory"
      "go to kitchen" "don't recall that location"
      [; 
         orPathFinderAvoidUnvisitedLocations = false;
         playerCommands.pushCommand("go to kitchen");
      ] "Kitchen"
;

#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
