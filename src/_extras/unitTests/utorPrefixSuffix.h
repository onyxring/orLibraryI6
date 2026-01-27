!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2025.12.31 utorPrefixSuffix
! Unit Tests for the utorPrefixSuffix orLibrary extension.
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
#ifndef        utorPrefixSuffix_STAGE;
default        utorPrefixSuffix_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUnitTest";
#include "orPrefixSuffix";
!--------------------------------------------------------------------------------------
#endif;
#ifnot;
#ifndef        utorPrefixSuffix_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       utorPrefixSuffix_STAGE  < LIBRARY_STAGE);
   #undef      utorPrefixSuffix_STAGE  ;
   Constant    utorPrefixSuffix_STAGE  LIBRARY_STAGE;
   #ifdef      utorPrefixSuffix_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   utorPrefixSuffix...";#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
   object utorPrefixSuffixStart "orPrefixSuffix Room" has light with description "Where orPrefixSuffix tests are run.";

   object -> mrsRobinson "Mrs. Robinson" has proper 
      with description "Loved by Beatles. And Jesus.",
      name 'mrs' 'robinson';

orUnitTest "utorPrefixSuffix" 
   with tests [; PlayerTo(utorPrefixSuffixStart, 3); ] noTest
   "x Mrs. Robinson" "!That's not a verb I recog"
;

#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
