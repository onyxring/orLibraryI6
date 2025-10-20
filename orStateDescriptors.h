!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.07.03 orStateDescriptors
! A framework for implementing text that describes object state, such as "(providing light)".
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License:
!--------------------------------------------------------------------------------------
! During inventory and room descriptions, the standard library interogates objects and
! prints qualifying text such as "(providing light)" or "(closed, empty and providing
! light)".  This module implements an extensible framework for defining new qualifiers
! as well as redefining or removing existing qualifiers.
!
! This is accomplished through the use of routines which are registered with at least
! one of two orRoutinesList objects, "orWaePartFormatting" and "orWaeFullFormatting".
!
! This module eliminates the use of Messages "ListMiscellany #7-17".  These may be
! repurposed for your own uses.
!--------------------------------------------------------------------------------------
! Revision History
! 2003.03.15	Initial Creation (ORWAE_Formatting)
! 2024.07.03	Refactored for the orLibrary version 2.
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orStateDescriptors_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orHookWriteAfterEntry";
#include "_orRoutineList";
#include "_orInfExt";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orStateDescriptors_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orStateDescriptors_STAGE  < LIBRARY_STAGE);
   #undef      orStateDescriptors_STAGE  ;
   Constant    orStateDescriptors_STAGE  LIBRARY_STAGE;
   #ifdef      orStateDescriptors_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orStateDescriptors...";#endif;
!======================================================================================
! AFTER PARSER
#iftrue (LIBRARY_STAGE == AFTER_PARSER);
orInfExt with ext_messages[;
		ListMiscellany:switch(lm_n){
							1: print "closed";
								rtrue;
							2: print "empty";
								rtrue;
							3: print "providing light";
								rtrue;
							4: print "being worn";
								rtrue;
							5: print "locked";
								rtrue;
							6: print "open";
								rtrue;
						}
					]
		, ext_sdPartInvBit[o count;
			if (c_style & PARTINV_BIT ~= 0) {
				count=orStateDescriptors.runAccumulate(waePartRoutines,o,true);
				if(count==0) rfalse;
				orStateDescriptors.setCount(count);
				print " (";

				if(orStateDescriptors.suppressSingleItemWhich==0 && count==1) print (string)WHICH__TX,(isorare)o," ";

				orStateDescriptors.runAccumulate(waePartRoutines, o);
				print ")";
				rtrue;
			}
			rfalse;
		]
		, ext_sdFullInvBit[ o count;
			if (c_style & FULLINV_BIT ~= 0) {
				if (o.invent ~= 0){
					inventory_stage=2;
					if (RunRoutines(o,invent)~=0) {
						if (c_style & NEWLINE_BIT ~= 0) new_line;
						rtrue;
					}
				}
				count=orStateDescriptors.runAccumulate(waeFullRoutines, o,true);
				if(count==0) rfalse;
				orStateDescriptors.setCount(count);
				print " (";

				if(orStateDescriptors.suppressSingleItemWhich==0 && count==1) print (string)WHICH__TX,(isorare)o," ";

				orStateDescriptors.runAccumulate(waeFullRoutines, o);
				print ")";
				rtrue;
			}
			rfalse;
		]
		, ext_initialise[;
			orStateDescriptors.addRoomDescriptionHandler(sdClosedContainer );
			orStateDescriptors.addRoomDescriptionHandler(sdEmptyContainer );
			orStateDescriptors.addRoomDescriptionHandler(sdLight);
			orStateDescriptors.addInventoryHandler(sdWorn);
			orStateDescriptors.addInventoryHandler(sdClosedContainer );
			orStateDescriptors.addInventoryHandler(sdOpenContainer );
			orStateDescriptors.addInventoryHandler(sdLockedContainer );
			orStateDescriptors.addInventoryHandler(sdEmptyContainer );
			orStateDescriptors.addInventoryHandler(sdLight);
		]
;
#endif; !--after PARSER

!======================================================================================
! AFTER VerbLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
system_file;

orRoutineList orStateDescriptors
	with waePartRoutines 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
	,	waeFullRoutines 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
	,	betweenCalls[prop hasState; prop=prop; !suppress warning
			if(hasState==false) return;
			self.remainingCount=self.remainingCount-1;
			if(self.remainingCount>1) print COMMA__TX;
			if(self.remainingCount==1) print (SerialComma) self.totalCount, (string) AND__TX;
		]
	,	addRoomDescriptionHandler[r; self.add(waePartRoutines,r);]
	,	addInventoryHandler[r; self.add(waeFullRoutines,r);]
	,	removeRoomDescriptionHandler[r; self.remove(waePartRoutines,r);]
	,	removeInventoryHandler[r; self.remove(waeFullRoutines,r);]
	,	setCount[count; self.totalCount=self.remainingCount=count;]
	,	totalCount 0
	,	remainingCount 0
	,	suppressSingleItemWhich 0
;

!--New formatting routines ---------------------------------------------------------------
[sdWorn obj suppress retval;
	retval=(obj has worn);
	if(suppress==false && retval==true) L__M(##ListMiscellany, 4);
	return retval;
];
[sdLight obj suppress retval;
	retval=(obj has light && location hasnt light && parent(actor) hasnt light);
	if(suppress==false && retval==true) L__M(##ListMiscellany, 3);
	return retval;
];
[sdClosedContainer obj suppress retval;
	retval=(obj has container && obj hasnt open);
	if(suppress==false && retval==true) L__M(##ListMiscellany, 1);
	return retval;
];
[sdOpenContainer obj suppress retval;
	retval=(obj has container && obj has openable && obj has open);
	if(suppress==false && retval==true) L__M(##ListMiscellany, 6);
	return retval;
];
[sdLockedContainer obj suppress retval;
	retval=(obj has openable && obj hasnt open && obj has lockable && obj has locked);
	if(suppress==false && retval==true) L__M(##ListMiscellany, 5);
	return retval;
];
[sdEmptyContainer obj suppress c retval;
	if(obj has container && obj has open or transparent){
	    objectloop(c in obj) {
			if (c hasnt concealed && c hasnt scenery) {
				retval = true; break;
			}
		}
	}

	if(suppress==false && retval==true) L__M(##ListMiscellany, 2);
	return retval;
];

#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE