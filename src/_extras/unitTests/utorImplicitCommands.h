!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2025.12.31  
! Unit Tests for the utorImplicitCommands orLibrary extension.
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
#ifndef        utorImplicitCommands_STAGE;
default        utorImplicitCommands_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUnitTest";
#include "orImplicitCommands";
!--------------------------------------------------------------------------------------
#endif;
#ifnot;
#ifndef        utorImplicitCommands_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       utorImplicitCommands_STAGE  < LIBRARY_STAGE);
   #undef      utorImplicitCommands_STAGE  ;
   Constant    utorImplicitCommands_STAGE  LIBRARY_STAGE;
   #ifdef      utorImplicitCommands_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   utorImplicitCommands...";#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
   object utorImplicitCommandsStart "orImplicitCommands Room" has light with description "Where orImplicitCommands tests are run.";
   
   object -> banana "banana" has edible with name 'banana',
      each_turn[; "A fruit fly hovers nearby.";]; 

   object -> box_ic "box" has openable container transparent 
      with name 'glass' 'box'
      ,  description "a glass box";
      
   object -> -> key "gold key" with name 'gold' 'key';

   object _implicitOpenBeforeTake LibraryExtensions
	   with ext_generateImplicitCommands[;     
         if(action == ##take && parent(noun) has openable && 		
            parent(noun) hasnt open) { 
            queueImplicitPrerequisite(##open, parent(noun));
                  rtrue;
         }
      ];
   
orUnitTest "utorImplicitCommands" 
   with tests [; PlayerTo(utorImplicitCommandsStart, 3); turns=0;] noTest 
      "eat banana" "(first taking the banana)"
      "score" "2 turns"
      "take key" "Taken."
      "drop all" noTest !--clean up for the next test
;

#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
