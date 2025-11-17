!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.03.29 orBetterChoice
! Give the parser better decision making skills when considering objects.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! Give the parser better decision making skills when considering objects.
! For example, objects already carried are given less priority during a "Take" action
! and objects that are edible are given more priority for the "Eat command".  Additionally,
! if the object has the property chooseobject, this is called to offer an object-specific
! return.
!--------------------------------------------------------------------------------------
! Revision History
! 2003.01.02	Initial Creation
! ...			History from orLibrary v1 snipped away.
! 2024.05.11	Refactored for the orLibrary version 2.
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orBetterChoice_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orHookStandardLibrary";
#include "orUtilParser";
#include "_orInfExt";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orBetterChoice_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orBetterChoice_STAGE  < LIBRARY_STAGE);
   #undef      orBetterChoice_STAGE  ;
   Constant    orBetterChoice_STAGE  LIBRARY_STAGE;
   #ifdef      orBetterChoice_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orBetterChoice..."; #endif;
!======================================================================================
! AFTER PARSER
#iftrue (LIBRARY_STAGE == AFTER_PARSER);
	  property individual chooseobject;

	  orInfExt with ext_chooseobjects[ obj code val retval;

		if(obj provides chooseobject) {
			retval = obj.chooseobject(code);  !--return -1 to continue to normal logic
			if(retval~=-1) return retval;
		}

		if(code<2) return 0; !--return default for "all" check
		!--parser is looking for a hint...
		for(val=0:val<pcount:val++) if(pattern-->val==obj) return util.orParser.excludeObjFromDisambiguation(obj); !--lets defer to another object if this one was already referred to in the input (avoids "PUT WATER IN WATER")

		switch(action_to_be){
			##take, ##remove:
					if(obj has static)  !--try not to consider objects which cant be moved
						return util.orParser.excludeObjFromDisambiguation(obj);

					if(ObjectScopedBySomething(obj)~=0)  !--lets also try not to grab objects that are brought into scope by something else (add_to_scope).
					 	return util.orParser.excludeObjFromDisambiguation(obj);

					if(obj in actor) !--if we are trying to take something, then lets not consider things that we already are holding
						return util.orParser.excludeObjFromDisambiguation(obj);

			##drop: if((obj in actor) == false) return util.orParser.excludeObjFromDisambiguation(obj); !--if we are trying to drop something, then lets only consider things that we are holding
			##show: if((obj in actor) == false) return util.orParser.excludeObjFromDisambiguation(obj); !--TODO: this could be changed if we had a "point to" verb, which handed things we weren't holding.
			##throwAt: if((obj in actor) == false) return 0;
			##eat: if(obj has edible) return 5; !--lets prefer edible things to non-edible things...
		}
		return 0;
	];
#endif; !--AFTER parser
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE

