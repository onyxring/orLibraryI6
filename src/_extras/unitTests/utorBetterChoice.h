!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! YYYY.MM.DD utorBetterChoice
! Unit Tests for the utorBetterChoice orLibrary extension.
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
default        utorBetterChoice_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUnitTest";
#include "orBetterChoice";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        utorBetterChoice_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       utorBetterChoice_STAGE  < LIBRARY_STAGE);
   #undef      utorBetterChoice_STAGE  ;
   Constant    utorBetterChoice_STAGE  LIBRARY_STAGE;
   #ifdef      utorBetterChoice_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   utorBetterChoice...";#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
   object utorBetterChoiceStart "orBetterChoice Room" has light with description "Where orBetterChoice tests are run.";

   object -> dish "big candy dispenser" 
      has supporter static 
      with name 'candy' 'dispenser', add_to_scope handle;

   object handle "candy dispenser's handle" has static 
      with  name 'candy' 'dispenser^s' 'handle';

   object chocolate "chocolate candy" dish 
      with name 'chocolate' 'candy' has edible;

   object toy "plastic candy toy" dish 
      with name 'plastic' 'candy' 'toy';

   object torch "fixed torch" with name 'torch' 'fixed',
      description "It's a torch on the wall.",
      chooseObject[code;
      if (code == 1 && action_to_be == ##Take or ##Remove) return 2;
      return -1;
      ];

orUnitTest "utorBetterChoice" 
   with tests [; PlayerTo(utorBetterChoiceStart, 3); ]  noTest 
      "take candy" "!big candy dispenser"
      "take all candy" noTest  
      "drop candy" "!big candy dispenser"   
      "drop all candy" noTest
      "eat candy" "Not bad."
      [; 
         move torch to location; 
         playerCommands.pushCommand("take all");
      ] "!fixed torch: Taken"       
;

#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
