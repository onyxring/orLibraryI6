!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2025.12.31 utorDeque
! Unit Tests for the utorDeque orLibrary extension.
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
default        utorDeque_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUnitTest";
#include "_orDeque";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        utorDeque_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       utorDeque_STAGE  < LIBRARY_STAGE);
   #undef      utorDeque_STAGE  ;
   Constant    utorDeque_STAGE  LIBRARY_STAGE;
   #ifdef      utorDeque_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   utorDeque...";#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
   
orDeque heroStk 
   with heroes 0 0 0 0 0 0 0 0 0 0
   ,	arrayPropName heroes;

orUnitTest "utorDeque" 
   with tests [; 
      heroStk.push("Aquaman"); 
      heroStk.push("Batman");
      heroStk.push("Flash");
      heroStk.push("Superman");
      heroStk.push("Wonder Woman");
      heroStk.push("Zatara");

      while(heroStk.getLength()>0) print(string)heroStk.pop()," ";
   ] "Zatara Wonder Woman Superman Flash Batman Aquaman" 
               
;

#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
