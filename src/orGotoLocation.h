!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.08.12 orGotoLocation
! Adds a verb for players to "go to <location>".
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! Revision History
! 2024.08.12	Initial Creation
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
#ifndef        orGotoLocation_STAGE;
default        orGotoLocation_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orTrackSeen";
#include "orUtilMapPathFinder";
#include "orPlayerCommandQueue";
!--------------------------------------------------------------------------------------
#endif;
#ifnot;
#ifndef        orGotoLocation_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orGotoLocation_STAGE  < LIBRARY_STAGE);
   #undef      orGotoLocation_STAGE  ;
   Constant    orGotoLocation_STAGE  LIBRARY_STAGE;
   #ifdef      orGotoLocation_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orGotoLocation...";#endif;
!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);
	Global orPathFinderAvoidUnvisitedLocations = true;
#endif; !--Before Parser
!======================================================================================
! AFTER PARSER
#iftrue (LIBRARY_STAGE == AFTER_PARSER);
orInfExt with ext_messages[;
		gotoLocation: if(lm_n==1) print_ret "You can't recall how to return to that location.";
	]
;
#endif; !--After Parser
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
[gotoLocationSub i;
	if(util.orMap.pathFinder.determinePath(location, noun)<0){
		return L__M(##gotoLocation, 1);
	}
	util.orMap.pathFinder.convertPathToDirections(location);
	
	for(i=0: i < util.orMap.pathFinder.getPathLength() : i++){ 
		if(util.orArray.get(util.orMap.pathFinder,path,i)<0) continue; !-- a -1 is creeping in during path determination.  This ignores it for now, but... TODO: look into why this is happening.
		playerCommands.pushAction(##go, util.orArray.get(util.orMap.pathFinder,path,i));
	}	
];

[VisitedRoom o;
	switch(scope_stage){
			1: return false;
			2: objectloop(o && metaclass(o) == object && parent(o)==0 && (o has beenSeen|| orPathFinderAvoidUnvisitedLocations==false) ){
					PlaceInScope(o);
				}
				rtrue;
			3: "You don't recall that location."; 
		}
	rfalse;
];
#endif; !--After VERBLIB
!======================================================================================
! AFTER Grammar
#iftrue (LIBRARY_STAGE == AFTER_GRAMMAR);
   extend  'go' * 'to' scope=VisitedRoom -> gotoLocation;
#endif; !--After Grammar
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
