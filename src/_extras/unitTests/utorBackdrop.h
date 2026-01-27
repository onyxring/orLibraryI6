!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2025.12.31 utorBackdrop
! Unit Tests for the utorBackdrop orLibrary extension.
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
#ifndef        utorBackdrop_STAGE;
default        utorBackdrop_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUnitTest";
#include "orBackdrop";
!--------------------------------------------------------------------------------------
#endif;
#ifnot;
#ifndef        utorBackdrop_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       utorBackdrop_STAGE  < LIBRARY_STAGE);
   #undef      utorBackdrop_STAGE  ;
   Constant    utorBackdrop_STAGE  LIBRARY_STAGE;
   #ifdef      utorBackdrop_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   utorBackdrop...";#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
   object utorBackdropStart "orBackdrop Room" has light 
      with description "Where orBackdrop tests are run."
   ,  w_to clearing
   ,  backdrops 'hand' 'plant' 'plants' bdDefault bdDefault bdEnd;

   object clearing "Clearing1" has light
      with  description "Thick plants surround you. A hand, rising from the bushes to the west beckons to you."
      ,  w_to clearing2
      ,  backdrops 'plant' 'plants' 
               "The plants are thick, mostly impenetrable."
               bdDefault bdEnd
            'hand' 
               "It gestures for you to follow it the west."
               "The hand is elusive." bdEnd
   ;
  object clearing2 "Clearing2" has light
   with description "Thick plants surround you. A hand, rising from the bushes to the west beckons for you to follow.",
      w_to "You attempt to go West but end up going no where.",
      backdrops 'hand' 
            "It gestures for you to follow it to the west."
            bdDefault
            bdEnd
         'plant' 'plants' 
            "The plants are thick, mostly impenetrable."
            [; if (action==##Enter) <<go w_obj>>; ]  
            bdEnd;

   object motherNature "Mother Nature" clearing2 has proper
      with react_before[;
         take: 
               if(orBackdropEngine.getKeyword(clearing2)=='plant'){ 
                  move player to utorBackdropStart;
                  "~No,~ says the goddess, wagging a finger. ~There are consequences for trying to steal my children.";
               }
            ];

orUnitTest "utorBackdrop" 
   with tests [; PlayerTo(utorBackdropStart, 3); ] noTest 
      "x plants" "That's not something you need to refer to"   
      "w. x plants" "The plants are thick"
      "x hand" "It gestures"
      "take hand" "The hand is elusive"
      "take plants" "That's not something you need to refer to"
      "w. enter plants" "You attempt to go"
      "take plants" "wagging a finger"
;

#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
