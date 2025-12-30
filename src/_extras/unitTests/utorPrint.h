!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! YYYY.MM.DD utorPrint
! Unit Tests for the utorPrint orLibrary extension.
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
default        utorPrint_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUnitTest";
#include "orPrint";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        utorPrint_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       utorPrint_STAGE  < LIBRARY_STAGE);
   #undef      utorPrint_STAGE  ;
   Constant    utorPrint_STAGE  LIBRARY_STAGE;
   #ifdef      utorPrint_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   utorPrint...";#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
object utorPrintStart "<REPLACEWITHNAME> Room" has light with description "Where orPrint tests are run.";


object -> fathernaru "Father Naru";
object -> gotie "Gotie";
object -> spirit "Jacob" has proper;
object -> scrooge "Ebenezer" has proper;
object -> robot "robot";
object -> box_op "box";
object -> toy_op "toy";

orUnitTest "utorPrint" 
   with tests [; 
            PlayerTo(utorPrintStart, 3);
            orPrint("Weller says: ~$bold;Now$roman is the time for all $italics;good men$roman to come to the aid of their country.~");
            orPrint("$upper(~7~ is truly prime. (~2~ and ~3~ are only honorarily so$))");
            actor=fathernaru; 
            noun=gotie; 
            orPrint("$actor blesses $noun.");
            orPrint("$name:1 materializes to stop $2.", spirit, scrooge);
            orPrint("$numb:1;$numbText:2;$numb:3", 3,4,5);
            orPrint("The number $numb:1 is $$$hex:1 in hex.", 257);
            orPrint("You pick up $1 rock$s:1;.", 1);
            orPrint("You pick up $1 rock$s:1;.", 3);
            actor=robot;
            noun=box_op;
            second=toy_op;
            orPrint("$The:actor $the:noun $A:second $a:second");
            orPrint("$up(upper) and $lo(LOWER).");
      ]   [val retval count; 
         count++; retval=retval+self.assertContains("Weller says: ~Now is the time for all good men to come to the aid of their country.~"); !--doesnt actually test style, but at least verifies the style print patterns don't print
         count++; retval=retval+self.assertContains("~7~ IS TRULY PRIME. (~2~ AND ~3~ ARE ONLY HONORARILY SO)");
         count++; retval=retval+self.assertContains("Father Naru blesses Gotie.");
         count++; retval=retval+self.assertContains("Jacob materializes to stop Ebenezer.");
         count++; retval=retval+self.assertContains("3four5");
         #ifdef TARGET_ZCODE;
            count++; retval=retval+self.assertContains("The number 257 is $0101 in hex.");
         #ifnot;
            count++; retval=retval+self.assertContains("The number 257 is $00000101 in hex.");
         #endif;
         count++; retval=retval+self.assertContains("You pick up 1 rock.");
         count++; retval=retval+self.assertContains("You pick up 3 rocks.");
         count++; retval=retval+self.assertContains("The robot the box A toy a toy");
         count++; retval=retval+self.assertContains("UPPER and lower.");

         return retval==count;
      ]  
      !--TODO: Add test cases from the rest of orPrint
;

#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
