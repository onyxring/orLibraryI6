!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2025.12.31 utorUniqueMultiMessage
! Unit Tests for the utorUniqueMultiMessage orLibrary extension.
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
#ifndef        utorUniqueMultiMessage_STAGE;
default        utorUniqueMultiMessage_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUnitTest";
#include "orUniqueMultiMessage";
!--------------------------------------------------------------------------------------
#endif;
#ifnot;
#ifndef        utorUniqueMultiMessage_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       utorUniqueMultiMessage_STAGE  < LIBRARY_STAGE);
   #undef      utorUniqueMultiMessage_STAGE  ;
   Constant    utorUniqueMultiMessage_STAGE  LIBRARY_STAGE;
   #ifdef      utorUniqueMultiMessage_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   utorUniqueMultiMessage...";#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
   class marble_umm with name 'marble' 'marbles//p', short_name "marble", plural "marbles";
   class candy_umm with name 'candy' 'candies//p', short_name "candy",  plural "candies";
   class rock_umm with name 'rock' 'rocks//p', short_name "rock", plural "rocks";
   
   object utorUniqueMultiMessageStart "orUniqueMultiMessage Room" has light with description "Where orUniqueMultiMessage tests are run.";

   object -> bowl_umm "bowl" with name 'bowl' has open container;
       marble_umm ->->; 
       marble_umm ->->; 
       marble_umm ->->;
       marble_umm ->->; 
       marble_umm ->->;
       candy_umm ->->; 
       candy_umm ->->;
       rock_umm ->->;

   orUnitTest "utorUniqueMultiMessage" 
       with tests [; PlayerTo(utorUniqueMultiMessageStart, 3); ] noTest
         "take all from bowl" [val
                                    retval; 
                                  retval=retval+self.assertContains("5 marbles : Removed."); 
                                  retval=retval+self.assertContains("2 candies : Removed."); 
                                  retval=retval+self.assertContains("rock : Removed."); 
                                  return retval==3;
                              ]
    ;


#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
