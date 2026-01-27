!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2025.12.31 utorReview
! Unit Tests for the utorReview orLibrary extension.
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
#ifndef        utorReview_STAGE;
default        utorReview_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUnitTest";
#include "orReview";
!--------------------------------------------------------------------------------------
#endif;
#ifnot;
#ifndef        utorReview_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       utorReview_STAGE  < LIBRARY_STAGE);
   #undef      utorReview_STAGE  ;
   Constant    utorReview_STAGE  LIBRARY_STAGE;
   #ifdef      utorReview_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   utorReview...";#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
   object utorReviewStart "orReview Room" has light with description "Where orReview tests are run.";

   object -> table_rvw "table" with name 'table', description "The wooden table is old and stained.";
   
orUnitTest "utorReview" 
   with tests [; PlayerTo(utorReviewStart, 3); ] noTest 
      "review table" "have not already examined"   
      "x table" "is old and stained"   
      "review table" "is old and stained"   
;

#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
