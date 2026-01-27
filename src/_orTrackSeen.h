!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.07.10 orTrackSeen
! Track things which have been seen by the player.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! This extension, classified as a "utility", provides tooling for other extensions to
! use and provides no specific story features by itself.
!
! A piddly little utility to track if an object or room has been previously seen by
! the player.  Seen objects are given the beenSeen attribute.
!--------------------------------------------------------------------------------------
! Revision History
! 2024.07.10 Initial Creation
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
#ifndef        orTrackSeen_STAGE;
default        orTrackSeen_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orInfExt";
#include "_orHookStandardLibrary";
#include "_orHookInformLibrary";
#include "_orHookVerbs";
!--------------------------------------------------------------------------------------
#endif;
#ifnot;
#ifndef        orTrackSeen_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orTrackSeen_STAGE  < LIBRARY_STAGE);
   #undef      orTrackSeen_STAGE  ;
   Constant    orTrackSeen_STAGE  LIBRARY_STAGE;
   #ifdef      orTrackSeen_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orTrackSeen..."; #endif;
!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);
    attribute beenSeen;
#endif; !--before parser
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
    orInfExt with ext_afterAction[;
            if(action==##Examine && noun) give noun beenSeen;           
            if(action==##Look ) give location beenSeen; !in the context of rooms, beenSeen is redundant to the Standard Librdary's visited attribute
        ]
    ,   ext_afterGamePrologueNotify[;
            give location beenSeen; !when the location in initialise
	    ]
    ,   ext_playerToNotify[ newloc;
            give newloc beenSeen; 
	    ]
;
#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
