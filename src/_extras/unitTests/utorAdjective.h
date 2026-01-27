!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2025.12.31 utorAdjective
! Unit Tests for the utorAdjective orLibrary extension.
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
#ifndef        utorAdjective_STAGE;
default        utorAdjective_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUnitTest";
#include "orAdjective";
!--------------------------------------------------------------------------------------
#endif;
#ifnot;
#ifndef        utorAdjective_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       utorAdjective_STAGE  < LIBRARY_STAGE);
   #undef      utorAdjective_STAGE  ;
   Constant    utorAdjective_STAGE  LIBRARY_STAGE;
   #ifdef      utorAdjective_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   utorAdjective...";#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
   object utorAdjectiveStart "orAdjective Room" has light with description "Where orAdjective tests are run.";

      object -> glass_adj "drinking glass" with name 'glass', 
      adjective 'drinking', description "for drinking";

      object -> marble_adj "glass marble" with name 'marble', 
      adjective 'glass', description "for rolling";

      object -> glass_table_adj "glass table" with name 'table', 
      adjective 'glass', description "flat and glass";

      object -> marble_table_adj "marble table" with name 'table', 
      adjective 'marble', description "flat and marble";


   orUnitTest "utorAdjective" 
      with tests [; 
         orAdjective.mode=PREFER_NOUN; 
         PlayerTo(utorAdjectiveStart, 3);
      ] noTest 
      "x glass" "for drinking"   
      "x marble" "for rolling"   
      "x table" "the glass table or the marble table?"   
      "x drinking" "for drinking"   
      [;orAdjective.mode=REQUIRE_NOUN;] noTest
      "x drinking" "can't see any such thing"
      "drop all" noTest !--clean up for other unit tests
   ;

#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
