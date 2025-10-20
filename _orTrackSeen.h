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
default        orTrackSeen_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orInfExt";
#include "_orHookStandardLibrary";
!--------------------------------------------------------------------------------------
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

! Note: this is hardly an extension at all; however, this functionality is used by at
! least two other extensions which actually do something with the tracking mechanism.
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
    orInfExt with ext_afterAction[;
            if(action==##Examine && noun) give noun beenSeen;
            if(action==##Look) give location beenSeen;

        ];
#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
