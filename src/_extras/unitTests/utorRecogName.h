!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2025.12.31 utorRecogName
! Unit Tests for the utorRecogName orLibrary extension.
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
#ifndef        utorRecogName_STAGE;
default        utorRecogName_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUnitTest";
#include "orRecogName";
!--------------------------------------------------------------------------------------
#endif;
#ifnot;
#ifndef        utorRecogName_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       utorRecogName_STAGE  < LIBRARY_STAGE);
   #undef      utorRecogName_STAGE  ;
   Constant    utorRecogName_STAGE  LIBRARY_STAGE;
   #ifdef      utorRecogName_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   utorRecogName...";#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
   object utorRecogNameStart "orRecogName Room" has light with description "Where orRecogName tests are run.";

   object -> smellyCat "dirty smelly cat" 
   with description "It stinks.";

   object -> brownDog "great brown dog" with name 'Spot',
     description "Happy and brown.";

orUnitTest "utorRecogName" 
   with tests [; PlayerTo(utorRecogNameStart, 3); ] noTest
      "x cat" "It stinks" 
      "x dog" "Happy and brown"
      "x spot" "Happy and brown"
      [;give brownDog noRecogName;] noTest
      "x spot" "Happy and brown"
      "x dog" "You can't see any"     
;

#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
