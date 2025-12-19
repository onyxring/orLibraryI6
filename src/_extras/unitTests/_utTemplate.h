!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! YYYY.MM.DD ut<REPLACEWITHNAME>
! Unit Tests for the ut<REPLACEWITHNAME> orLibrary extension.
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
default        ut<REPLACEWITHNAME>_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUnitTest";
#include "<REPLACEWITHNAME>";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        ut<REPLACEWITHNAME>_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       ut<REPLACEWITHNAME>_STAGE  < LIBRARY_STAGE);
   #undef      ut<REPLACEWITHNAME>_STAGE  ;
   Constant    ut<REPLACEWITHNAME>_STAGE  LIBRARY_STAGE;
   #ifdef      ut<REPLACEWITHNAME>_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   ut<REPLACEWITHNAME>...";#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
   object ut<REPLACEWITHNAME>Start "<REPLACEWITHNAME> Room" has light with description "Where <REPLACEWITHNAME> tests are run.";

orUnitTest "ut<REPLACEWITHNAME>" with setup [; PlayerTo(ut<REPLACEWITHNAME>Start, 3); ] "x me"
  , test 0 
    "good-looking."   
;

#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
