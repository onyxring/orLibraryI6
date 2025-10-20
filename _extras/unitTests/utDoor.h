!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.05.06 utDoor
! Unit Tests for the orDoor orLibrary extension.
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
default        utDoor_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "orTest";
#include "orDoor";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        utDoor_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       utDoor_STAGE  < LIBRARY_STAGE);
   #undef      utDoor_STAGE  ;
   Constant    utDoor_STAGE  LIBRARY_STAGE;
   #ifdef      utDoor_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   utDoor...";#endif;
!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);

#endif; !--Before Parser
!======================================================================================
! AFTER PARSER
#iftrue (LIBRARY_STAGE == AFTER_PARSER);

#endif; !--After Parser
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);

#ifndef orDoor; class orDoor; #endif;

    object orDoorRoomStart "room one" has light with description "As good a place as any.", n_to theOrDoor;
    object orDoorRoomLast "room two" has light with description "A better place than some.", s_to theOrDoor;

    orDoor theOrDoor "a door" with name 'door';


   orTestCollection orDoorTests;
		  orTest ->  with setup[; move player to orDoorRoomStart;],
         command "n",
		  	assert[rsp;
		  		if(rsp.contains("room two")==false) return "Failed to move to expected location.";
		  	];

		  orTest ->  with command "s",
		  	assert[rsp;
		  		if(rsp.contains("room one")==false) return "Failed to return to starting location.";
		  	];

#endif; !--After VERBLIB
!======================================================================================
! AFTER Grammar
#iftrue (LIBRARY_STAGE == AFTER_GRAMMAR);

#endif; !--After Grammar
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
