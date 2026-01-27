!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2025.12.31 utorNpc
! Unit Tests for the utorNpc orLibrary extension.
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
#ifndef        utorNpc_STAGE;
default        utorNpc_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUnitTest";
#include "orNpc";
!--------------------------------------------------------------------------------------
#endif;
#ifnot;
#ifndef        utorNpc_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       utorNpc_STAGE  < LIBRARY_STAGE);
   #undef      utorNpc_STAGE  ;
   Constant    utorNpc_STAGE  LIBRARY_STAGE;
   #ifdef      utorNpc_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   utorNpc...";#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
   attribute dancer;

   object utorNpcStart "orNpc Room" has light with description "Where orNpc tests are run.",
      w_to utorNpcOther;
    
   orNPC -> butler "butler" 
      with name 'man' 'butler',
      description "The butler ignores you, preoccupied with other things.",
      doNothing[;
         if(canPlayerWitness()==false) rfalse;
         print (string)random("The butler clears his throat.",
            "The butler gives a small sigh.",
            "The butler looks around as though searching for something.");
      ];
   object utorNpcOther "Other orNpc room" has light with description "Where more orNpc tests are run.",
         e_to utorNpcStart;

	orNPC -> boogieMan "Boogie Man" has dancer
      with description "Altogether different than his cousin, the Boogey Man."
      ,  name 'man' 'boogie'
      ,  getSkillPriority[skl; 
            if(skl==orNpcDanceSkill) return 99;
            return skl.defaultPriority;
         ];

   	
   orNpcSkill orNpcDanceSkill 
      with perform[npc; 
            print (The)npc, " dances in a way that impresses everyone present.";
         ]
   ,  defaultPriority 1
   ,  canPerform[npc; return canPlayerWitness(npc); ]
   ,  appliesToCharacter[npc; 
         return (npc has dancer);  	
      ];

orUnitTest "utorNpc" 
   with tests [; PlayerTo(utorNpcStart, 3); ] noTest
               "z" "The butler"  
               "z" "!Boogie Man"
               "w" "!The buttler"
               "z" "Boogie Man"
               "e" "The butler"
               "z" "!Boogie Man"
;

#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
