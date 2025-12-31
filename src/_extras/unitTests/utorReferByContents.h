!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2025.12.31 utorReferByContents
! Unit Tests for the utorReferByContents orLibrary extension.
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
default        utorReferByContents_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUnitTest";
#include "orReferByContents";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        utorReferByContents_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       utorReferByContents_STAGE  < LIBRARY_STAGE);
   #undef      utorReferByContents_STAGE  ;
   Constant    utorReferByContents_STAGE  LIBRARY_STAGE;
   #ifdef      utorReferByContents_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   utorReferByContents...";#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
   object utorReferByContentsStart "orReferByContents Room" has light with description "Where orReferByContents tests are run.";

   orReferByContents bottle1 "bottle"  with name 'bottle' 
   has open container; 

   object "marshmallows" bottle1 has edible pluralname
   with name 'marshmallows'
   ,  ignoreActions ##Eat ##Drink;

   orReferByContents bottle2 "bottle"  with name 'bottle' 
   has open container; 

   object "marbles" bottle2 has pluralname with name 'marbles';

orUnitTest "utorReferByContents" 
   with tests [; 
         PlayerTo(utorReferByContentsStart, 3); 
         move bottle1 to selfobj; 
         move bottle2 to selfobj;
      ] noTest
      "drop marbles" [val retval;
                        retval=self.assertDoesNotContain("first taking");
                        retval=retval+self.assertContains("Dropped");
                        return retval==2;
                  ]
      "i" "!marbles"
      "eat marshmallows" "Not bad"
      "drop all" noTest !cleanup for next test
;

#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
