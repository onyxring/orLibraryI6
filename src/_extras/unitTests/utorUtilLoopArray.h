!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! YYYY.MM.DD utorUtilLoopArray
! Unit Tests for the utorUtilLoopArray orLibrary extension.
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
default        utorUtilLoopArray_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUnitTest";
#include "orUtilLoopArray";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        utorUtilLoopArray_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       utorUtilLoopArray_STAGE  < LIBRARY_STAGE);
   #undef      utorUtilLoopArray_STAGE  ;
   Constant    utorUtilLoopArray_STAGE  LIBRARY_STAGE;
   #ifdef      utorUtilLoopArray_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   utorUtilLoopArray...";#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);

object bowl with  listOfFruit 0 "apple" "orange" "banana" "peach"; 

orUnitTest "utorUtilLoopArray" 
   with tests [i el; 
            for(i=0:i<4:i++){  !-- print up to 4 elements in the array 
               el = util.orLoopArray.getNext(bowl, listOfFruit); 
               print (string)el," "; 
            } 
         ] [val; 
               if(val.contains("apple") && 
                  val.contains("orange") && 
                  val.contains("banana") && 
                  val.contains("peach")) rtrue;
               rfalse;
         ]
;

#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
