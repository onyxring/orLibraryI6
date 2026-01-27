!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2025.12.31 utorRemoteLook
! Unit Tests for the utorRemoteLook orLibrary extension.
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
#ifndef        utorRemoteLook_STAGE;
default        utorRemoteLook_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUnitTest";
#include "orRemoteLook";
!--------------------------------------------------------------------------------------
#endif;
#ifnot;
#ifndef        utorRemoteLook_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       utorRemoteLook_STAGE  < LIBRARY_STAGE);
   #undef      utorRemoteLook_STAGE  ;
   Constant    utorRemoteLook_STAGE  LIBRARY_STAGE;
   #ifdef      utorRemoteLook_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   utorRemoteLook...";#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
   object utorRemoteLookStart "Your studyRoom" has light with description "There's a doorway to the east...",
   e_look "You see a doorway.",
   u_look "The ceiling is open, exposing the rafters.",
   e_to octaRoom,
   cant_look "You see the rest of the wall.",
   compass_look [ obj;
         if (obj == n_obj or s_obj or w_obj) 
            "You see the wall.";
   ];

   object octaRoom "Octagon Room" has light
   with description "Paths would lead in all eight directions three directions, but only three are available (east, west, and north)."
   ,  remoteDescription[dir; 
         "The Octagon Room lies to the ", 	(name)dir,".";
      ]
   ,  n_to bluRm, e_to redRm, w_to utorRemoteLookStart
   ;
   object bluRm "Blue Room" has light with s_to octaRoom, description "A blue room.";
   object redRm "Red Room" has light with w_to octaRoom, description "A red room.";

orUnitTest "utorRemoteLook" 
   with tests [; PlayerTo(utorRemoteLookStart, 3); ] noTest 
      "look east" "a doorway"
      "look up" "The ceiling is open" 
      "look west" "see the wall"
      "look nw" "see the rest of the wall"
      "e. e. look west" "lies to the west"
      "w. n. look south" "lies to the south"
;

#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
