!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2025.12.31 utorString
! Unit Tests for the utorString orLibrary extension.
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
default        utorString_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUnitTest";
#include "orString";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        utorString_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       utorString_STAGE  < LIBRARY_STAGE);
   #undef      utorString_STAGE  ;
   Constant    utorString_STAGE  LIBRARY_STAGE;
   #ifdef      utorString_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   utorString...";#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);

global str;
!--TODO: expand these tests. 
orUnitTest "orString" 
   with tests [; !tests trim(), trimLeft(), trimRight(), and print()
         str=util.orStr.new("              sky is            "); 
         print "The "; 
         str.trim().print(); 
         print " blue.";
      ]  "The sky is blue."  !implicitly tests contains()
;

#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
