!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.07.03 orRoutineList
! An object that can maintain a list of routines and run them in order like
! additive properties.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License:
!--------------------------------------------------------------------------------------
! This extension, classified as a "utility", provides tooling for other extensions to
! use and provides no specific story features by itself.
!
! The ORRoutineList class exposes methods for adding and removing routines
! and calling them in different ways. For instance,  runAccumulate() will call all
! registered routines and add up their results.  runUntil() will do the same thing
! but stop when the specified value is returned by one of the registered routines.
! This is similar by design to additive properties.  Similarly, the runUntilNot()
! routine will run the routines until one of them does *not* return the specified value.
!--------------------------------------------------------------------------------------
! Revision History
! 2001.12.10	Initial Creation
! ...			History from orLibrary v1 snipped away.
! 2024.07.03	Refactored for the orLibrary version 2.

!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orRoutineList_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "orUtilArray";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orRoutineList_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orRoutineList_STAGE  < LIBRARY_STAGE);
   #undef      orRoutineList_STAGE  ;
   Constant    orRoutineList_STAGE  LIBRARY_STAGE;
   #ifdef      orRoutineList_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orRoutineList..."; #endif;
!======================================================================================
! AFTER PARSER
#iftrue (LIBRARY_STAGE == AFTER_PARSER);
   class orRoutineList
		with runAccumulate[prop one two three
					retval rv t;
				retval=0;
				for(t=0:t<util.orArray.getLength(self,prop):t++){
					if(util.orArray.get(self,prop,t)~=0) rv=((util.orArray.get(self,prop,t))(one,two, three));
					retval=retval+rv;
					self.betweenCalls(prop, rv, one, two, three);
				}

				return retval;
			]
		,	runUntil[prop untilval one two three
					retval t;
				retval=0;
				for(t=0:t<util.orArray.getLength(self,prop):t++){
					if(util.orArray.get(self,prop,t)~=0)
						retval=((self.get(prop,t))(one, two,three));
					if(retval==untilval) return retval;
					self.betweenCalls(prop, retval, one, two, three);
				}
				return retval;
			]
		,	runWhile[prop whileval one two three
					retval t;
				retval=0;
				for(t=0:t<util.orArray.getLength(self,prop):t++){
					if(util.orArray.get(self,prop,t)~=0)
						retval=((self.get(prop,t))(one, two,three));
					if(retval~=whileval) return retval;
					self.betweenCalls(prop, retval, one, two, three);
				}
				return retval;
			]
		,	betweenCalls[;return;]
		,	add[prop r; util.orArray.append(self,prop,r); return r;]
		,	remove[prop r; util.orArray.removeValue(self,prop, r);]
	;

#endif; !--After Parser
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
