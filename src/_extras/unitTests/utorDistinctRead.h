!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! YYYY.MM.DD utorDistinctRead
! Unit Tests for the utorDistinctRead orLibrary extension.
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
default        utorDistinctRead_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUnitTest";
#include "orDistinctRead";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        utorDistinctRead_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       utorDistinctRead_STAGE  < LIBRARY_STAGE);
   #undef      utorDistinctRead_STAGE  ;
   Constant    utorDistinctRead_STAGE  LIBRARY_STAGE;
   #ifdef      utorDistinctRead_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   utorDistinctRead...";#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
   object utorDistinctReadStart "orDistinctRead Room" has light with description "Where orDistinctRead tests are run.";
   
   object -> counch "couch" with name 'couch'
   ,  description "A brown leather couch with words scratched into the fabric."
	,  readDescription "It says ~Kilroy was here~";

   object -> poster "poster" with name 'poster'
   ,  description "It depicts an acrobat just over the text ~Come see the greatest show on Earth!~"
   ,  readDescription description;

   orUnitTest "utorDistinctRead" 
      with tests [; PlayerTo(utorDistinctReadStart, 3); ] noTest 
         "x couch" "brown leather"   
         "read couch" "Kilroy"   
         "x poster" "greatest show"
         "read poster" "greatest show"
   ;

#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
