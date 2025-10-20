!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2025.08.29 orRemoteLook
! Enhances support for looking at neighboring locations.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! Using this module, a player who is presented with the description of a room similar
! to the following:
!
!     You are in the Dining Room.  To the North is the Laboratory.
!
! can have the ability to use any of the following:
!
!     look at laboratory
!     examine laboratory
!
! The description of a neighboring room is determined by the following logic:
!
!	1) if the current room has a direction-description property defined, then
!		 that is used. The following are direction-description properties:
!
!				n_look
!				s_look
!				e_look
!				w_look
!				nw_look
!				sw_look
!				ne_look
!				se_look
!				u_look
!				d_look
!
!	2) Failing the above, the room being looked at is searched for the
!		'remote_description' property.  If found, then that is used.
!
!	3) Alternately, the room being looked at is checked for the property
!		'describe_as_if_present' to be set to true.  If so, then the normal
!		room descriptions are called as though the character were actually
!		present in the room.
!
!	4) Finally, if the "Look <Direction>" form of this command was used, and
!		all of the above attempts to determine a remote description fail, then
!		current room is checked for the "cant_look" property which is printed.
!
! Additionally, for completness, the commands "examine room" and "look around" have
! been implimented along with minor variations.
!
! Room visibility is generally determined by a direct (or indirect, in the case of
! a door) connection in the game's map, but an additional property "visible_from"
! can be defined for a room to list other rooms from which it can be seen.
!--------------------------------------------------------------------------------------
! Revision History
! 2002.01.03	Initial Creation (actually earlier, but this is the only date stamp
!					still available without having to search archived code).
! ...			Snipped away the rest of the ORLibrary 1.x history.
! 2025.02.25	Ported to orLib 2.0.  Made a few breaking changes, such as refactoring
!				property names dropping support for the describe_as_if_present property
!				entirely.
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orRemoteLook_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "orUtilMap";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orRemoteLook_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orRemoteLook_STAGE  < LIBRARY_STAGE);
   #undef      orRemoteLook_STAGE  ;
   Constant    orRemoteLook_STAGE  LIBRARY_STAGE;
   #ifdef      orRemoteLook_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orRemoteLook...";#endif;
!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);
	system_file;
	property individual n_look; !--for individual compass look properties
    property individual s_look;
    property individual e_look;
    property individual w_look;
    property individual u_look;
    property individual d_look;
    property individual ne_look;
    property individual nw_look;
    property individual se_look;
    property individual sw_look;
    property individual in_look;
    property individual out_look;
    property individual cant_look;
    property individual cant_look_alt;
	property individual remoteClosed;
	property individual remoteDescription;

#endif; !--Before Parser
!======================================================================================
! AFTER PARSER
#iftrue (LIBRARY_STAGE == AFTER_PARSER);

#endif; !--After Parser
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
	object _olr_init LibraryExtensions
		with ext_messages[;
			LookDirection:
			switch(lm_n){
				!1: print "It",(string)IS__TX," too dark to see that way.^";
				1: print "What lies in that direction is unclear.^";
				2: print (The)lm_o," ",(isorare)lm_o," closed and I cannot see through it.^";
				!4: print (ig)CICant(actor)," see anything in that direction.^[Warning: obsolete message referenced (LookDirecton#4 from orRemoteLook).]^^";
				!5: print (ig)CIVerb(actor,"did","do","does")," not see anything like that.^";
			}
	];

#endif; !--After VERBLIB
!======================================================================================
! AFTER Grammar
#iftrue (LIBRARY_STAGE == AFTER_GRAMMAR);

	[lookDirectionSub
			prop dir adj;

		dir=noun;

		switch(noun){
			n_obj: prop= n_look;
			s_obj: prop= s_look;
			e_obj: prop= e_look;
			w_obj: prop= w_look;
			nw_obj: prop= nw_look;
			sw_obj: prop= sw_look;
			ne_obj: prop= ne_look;
			se_obj: prop= se_look;
			u_obj: prop= u_look;
			d_obj: prop= d_look;
		}

		!---1) If current room provides a direction description, then use that
		if(prop~=0 && parent(actor) provides prop && parent(actor).prop~=0) {

			printorrun(parent(actor),prop);
			return;
		}

		!---2) use the standard library's compass_look property if it exists...
		if (location provides compass_look && location.compass_look(noun)) rtrue;

		!---3) Try to resolve the direction into a destination...
		if(parent(actor) provides (dir.door_dir)) adj = parent(actor).(dir.door_dir);

		if(adj~=0 && metaclass(adj)==object) { !--it is an object, not just a string
	 		if(adj has door){ !---if actually a door, so try to see the room behind it
				if(adj has open or transparent){ !--- open door?  made of glass?
					adj=valueorrun(adj,door_to);
				}
				else {
					if(adj provides remoteClosed)
						printorrun(adj, remoteClosed);
					else
						L__M(##LookDirection,2,dir);
					return;
				}
			}
			! at this point we are assured a room, so redirect to that if it has a remoteDescrioption property
			if(adj provides remoteDescription) <<examineAdjacentRoom adj>>;

		}


		!--Lastly, the cant_look properties
		if(dir==u_obj or d_obj or in_obj or out_obj){
			if(parent(actor) provides cant_look_alt && parent(actor).cant_look_alt~=0) {
				util.orRef.resolvePrint(parent(actor),cant_look_alt, dir);
				new_line;
				return;
			}
		}
		else{ !--cardinal and intercardinal directions
			if(parent(actor) provides cant_look && parent(actor).cant_look~=0) {
				util.orRef.resolvePrint(parent(actor),cant_look, dir);
				new_line;
				return;
			}
		}
		!--failing all of the above, use the standard library's default message...
		L__M(##Look,7);
	];
	[examineAdjacentRoomSub;
		if(noun provides remoteDescription) {
			if(metaclass(noun.remoteDescription)==routine)
				noun.remoteDescription(util.orMap.getDirectionToAdjacentObject(location, noun));
			else
				print(string)noun.remoteDescription;
			new_line;
			return;
		}
	];
	Extend "look" first !--needed to usurp the new compasslook functionality in the standard library
		*	-> Look
		*	noun=ADirection -> LookDirection
		*	'to' noun=ADirection -> LookDirection
	;
	Extend "examine" first
		*	noun=ADirection -> LookDirection
	;
	! 	[CurrentRoom;
! 		if(scope_stage~=2)rfalse;
! 		PlaceInScope(parent(actor));
! 		rtrue;
! 	];
! 	Extend "examine" first
! 		*	-> Look
! 		*	noun=ADirection -> LookDirection
! 	;
! 	Extend "examine" last
! 		*	'room'	-> Look
! 		*	scope=CurrentRoom -> Look
! 		*	scope=AdjoiningRoom -> LookRoom
! 	;

! 	Extend "look" first !--needed to usurp the new compasslook functionality in the standard library
! 		*	-> Look
! 		*	noun=ADirection -> LookDirection
! 		*	'to' noun=ADirection -> LookDirection
! 	;
! 	Extend "look" last
! 		*	'around' -> Look
! 		*	'at' 'room' -> Look
! 		*	'outside'/'inside'/'out'/'in'/'through'/'thru' door ->LookDoor
! 		*	'at' scope=CurrentRoom -> Look
! 		*	'at' scope=AdjoiningRoom ->LookRoom
! 	;
! 	Extend "search" last
! 		*	scope=CurrentRoom -> Look
! 		*	'room' -> Look
! 		*	topic -> SearchNothing
! 	;
! 	[SearchNothingSub; L__M(##Miscellany,30);];

#endif; !--After Grammar
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE

! !======================================================================================
! ! C O D E   section (for code that falls between VERBLIB and GRAMMAR)

! 	[DescribeRoomRemotely o SuppressCantSee sva svl; ! Describe a given room (Look without being in the room)
! 		if(o hasnt light) {
! 			if(SuppressCantSee==false) L__M(##LookDirection,1);
! 			rfalse;
! 		}
! 		if(o provides remote_description){
! 			printorrun(o,remote_description);
! 			rtrue;
! 		}
! 		if(o provides describe_as_if_present && valueorrun(describe_as_if_present)==true){
! 			printorrun(o,description);
! 			svl=location;
! 			sva=parent(actor);
! 			location=o;
! 			move actor to o;
! 			MoveFloatingObjects();
! 			Locale(o); !now describe the contents of the room
! 			location=svl;
! 			move actor to sva;
! 			MoveFloatingObjects();
! 			rtrue;
! 		}
! 		if(SuppressCantSee==false) L__M(##LookDirection,2);
! 		rfalse;
! 	];

! !======================================================================================
!    #endif; #ifnot; #ifndef orRemoteLook_DONEGRAMMAR; constant orRemoteLook_DONEGRAMMAR; #ifdef orRemoteLook_DONEGRAMMAR; #endif; !--suppress warning
! !  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
! ! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
! !======================================================================================
! 	[AdjoiningRoom	d r;
! 		switch(scope_stage)
! 		{
! 			1:	rfalse;
! 			2:	objectloop(d in compass) !for each possible direction
! 				{
! 					if(parent(actor) provides d.door_dir && metaclass(parent(actor).(d.door_dir))==object)
! 					{
! 						r=parent(actor).(d.door_dir);
! 						if(r has door)
! 						{
! 							if(r has open or transparent)
! 								r=valueorrun(r,door_to);
! 							else
! 								r=nothing;
! 						}
! 						if(r~=nothing) PlaceInScope(r);
! 					}
! 				}
! 				!--An Adjoining room may not actually be reachable, just nearby and therefore visible...
! 				objectloop(r provides visible_from){
! 					for(d=0:d<(r.#visible_from/WORDSIZE):d++){
! 						if(r.&visible_from-->d==scopeceiling(actor))
! 							PlaceInScope(r);
! 					}
! 				}
! 				rtrue;
! 			3: L__M(##LookDirection,5);
! 		}
! 	];
! 	[DetermineRoomDirection rm d r;
! 		objectloop(d in compass) !for each possible direction
! 		{
! 			if(parent(actor) provides (d.door_dir))
! 			{
! 				r=valueorrun(parent(actor),(d.door_dir));
! 				if(r has door)
! 				{
! 					if(r has open or transparent)
! 						r=valueorrun(r,door_to);
! 					else
! 						r=nothing;
! 				}
! 				if(r==rm) return d;
! 			}
! 		}
! 		return 0;
! 	];
! 	[LookDoorSub o;
! 		o=ValueORRun(noun,door_to);
! 		noun=DetermineRoomDirection(o);
! 		LookDirectionSub();
! 		rtrue;
! 	];
! 	[LookRoomSub o;
! 		if(parent(actor)~=noun)
! 		{
! 			o=DetermineRoomDirection(noun);
! 			if(o~=0){ !--if a neighboring room, then describe via direction sub (which checks line of sight)
! 				noun=o;
! 				LookDirectionSub();
! 				rtrue;
! 			} !--otherwise, room must be visible but inaccessible (like a play's stage as seen from a balcony).  Fall through and describe.
! 		}
! 		DescribeRoomRemotely(noun);  !describe the specified room
! 	];

! 	! [LookDirectionSub o prop;
! 	! 	!---1) If current room provides a distance description, then use that
! 	! 	prop=-1;
! 	! 	switch(noun)
! 	! 	{
! 	! 		n_obj:prop=n_look;
! 	! 		s_obj:prop=s_look;
! 	! 		e_obj:prop=e_look;
! 	! 		w_obj:prop=w_look;
! 	! 		nw_obj:prop=nw_look;
! 	! 		sw_obj:prop=sw_look;
! 	! 		ne_obj:prop=ne_look;
! 	! 		se_obj:prop=se_look;
! 	! 		u_obj:prop=u_look;
! 	! 		d_obj:prop=d_look;
! 	! 	}
! 	! 	if(prop~=-1){
! 	! 		if(parent(actor) provides prop && parent(actor).prop~=0) {
! 	! 			printorrun(parent(actor),prop);
! 	! 			return;
! 	! 		}
! 	! 	}
! 	! 	!---2) use the standard library's compasslook property if it exists...
! 	! 	if (location provides compasslook && location.compasslook(noun)) rtrue;
! 	! 	!---3) Verify that the destination of the direction looked is a room and not a string or a routine (we don't want to risk running code that usually runs only when a player moves and may print text)
! 	! 	if(prop~=-1 && parent(actor) provides (noun.door_dir) && metaclass((parent(actor)).(noun.door_dir))==object) {
! 	! 		o=(parent(actor)).(noun.door_dir);
! 	! 		if(o has door){ !---if actually a door then verify that we can see the room behind
! 	! 			if(o has open or transparent){ !--- open door?  made of glass?
! 	! 				o=valueorrun(o,door_to);
! 	! 			}
! 	! 			else {
! 	! 				L__M(##LookDirection,3,o);
! 	! 				return;
! 	! 			}
! 	! 		}
! 	! 		!--- 3) at this point we are assured a room, so try to describe it
! 	! 		prop=false;
! 	! 		if(parent(actor) provides cant_look && parent(actor).cant_look~=0) prop=true; !--we should suppress any default messages if we cannot describe the room because we have an alternate default message supplied
! 	! 		if(DescribeRoomRemotely(o,prop)==true) return; !Describe the room, (suppressing any errors if we have a cant_look option left...)
! 	! 		!--failed to describe the room remotely.
! 	! 		if(prop==true) printorrun(parent(actor),cant_look); !--If we have a cant_look option availible, then we supressed error messsages show should run it now.
! 	! 		return;
! 	! 	}
! 	! 	!--check the cant_look property
! 	! 	if(parent(actor) provides cant_look && parent(actor).cant_look~=0) {
! 	! 		printorrun(parent(actor),cant_look);
! 	! 		return;
! 	! 	}
! 	! 	!--failing all of the above, use the standard library's version...
! 	! 	noun.description(); !--this will perform the same check as step 2 above, but if we got this far then it shouldn't make a difference
! 	! ];

! ! 	[CurrentRoom;
! ! 		if(scope_stage~=2)rfalse;
! ! 		PlaceInScope(parent(actor));
! ! 		rtrue;
! ! 	];
! ! 	Extend "examine" first
! ! 		*	-> Look
! ! 		*	noun=ADirection -> LookDirection
! ! 	;
! ! 	Extend "examine" last
! ! 		*	'room'	-> Look
! ! 		*	scope=CurrentRoom -> Look
! ! 		*	scope=AdjoiningRoom -> LookRoom
! ! 	;

! ! 	Extend "look" first !--needed to usurp the new compasslook functionality in the standard library
! ! 		*	-> Look
! ! 		*	noun=ADirection -> LookDirection
! ! 		*	'to' noun=ADirection -> LookDirection
! ! 	;
! ! 	Extend "look" last
! ! 		*	'around' -> Look
! ! 		*	'at' 'room' -> Look
! ! 		*	'outside'/'inside'/'out'/'in'/'through'/'thru' door ->LookDoor
! ! 		*	'at' scope=CurrentRoom -> Look
! ! 		*	'at' scope=AdjoiningRoom ->LookRoom
! ! 	;
! ! 	Extend "search" last
! ! 		*	scope=CurrentRoom -> Look
! ! 		*	'room' -> Look
! ! 		*	topic -> SearchNothing
! ! 	;
! ! 	[SearchNothingSub; L__M(##Miscellany,30);];

! !======================================================================================
! #endif; #endif; #endif;#endif;
! !======================================================================================


