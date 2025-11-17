!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.05.06 utReferByContents
! Unit Tests for the orReferByContentes orLibrary extension.
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
default        utReferByContents_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "orTest";
#include "orReferByContents";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        utReferByContents_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       utReferByContents_STAGE  < LIBRARY_STAGE);
   #undef      utReferByContents_STAGE  ;
   Constant    utReferByContents_STAGE  LIBRARY_STAGE;
   #ifdef      utReferByContents_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   utReferByContents...";#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
   object utReferByContentsRoom "orReferByContents Room" has light with description "As good a place as any";

   orReferByContents bottle1 "bottle" selfobj
   with name 'bottle'
   has open container;

   Object "marshmellows" bottle1 with name 'marshmellows', has edible pluralname;

   orReferByContents bottle2 "bottle" selfobj with name 'bottle'
   has open container;

   Object "marbles" bottle2 has pluralname with name 'marbles';

   orTestCollection orReferByContentsTests;
		  orTest ->  with setup[; move player to utReferByContentsRoo;],
         command "look",
		  	assert[rsp;
            rtrue;
		  	];

#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
