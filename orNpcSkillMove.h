!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2025.03.23 orNpcSkillMove
! An orNpc skill to teach NPCs to move around the map.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! Revision History
! 2001.12.01	Initial Creation
!	...			snipped away history from orLibrary v1
! 2025.03.23	Converted to orLibrary 2.0
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orNpcSkillMove_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "orNpc";
#include "orUtilRef";
#include "orUtilMapPathFinder";
#include "_orPronoun_English";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orNpcSkillMove_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orNpcSkillMove_STAGE  < LIBRARY_STAGE);
   #undef      orNpcSkillMove_STAGE  ;
   Constant    orNpcSkillMove_STAGE  LIBRARY_STAGE;
   #ifdef      orNpcSkillMove_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orNpcSkillMove...";#endif;
!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);
	property individual movePath;
	property individual moveArrived;
	property individual followTarget;

	Constant MOVE_PATTERN_REVERSE	0;
	Constant MOVE_PATTERN_REPEAT 	1;
	Constant MOVE_PATTERN_SINGLE 	2;
	Constant MOVE_PATTERN_HALT 		3;

#endif; !--Before Parser
!======================================================================================
! AFTER PARSER
#iftrue (LIBRARY_STAGE == AFTER_PARSER);
	orInfExt with ext_messages[n x1 x2; x1=lm_o; x2=lm_s; n=lm_n;
			Go:	switch(n){
					7:  print"^";
						CSubjectVerb(actor,false,false,"depart",0,"departs","departed");
						print " to ",(the)noun;  !using noun as a direction, because the standard library's call to this message does not pass the direction.
						if(second~=0) print " (following ",(the)second,")";
						".";

					8:	print"^";
						CSubjectVerb(actor,false,false,"arrive",0,"arrives","arrived");
						print " from ",(the)(util.orMap.reverseDirObj(x1));
						if(x2~=0) print " (following ",(me)x2,")";
						".";
				}
		];
#endif; !--After Parser
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
	orNpcSkill	_orNpcMoveEngine
		private _targetCanBeFollowed[npc target
					rm d;
				if(npc==0)	rfalse;
				objectloop(d in compass) { !loop through all adjacent rooms
					rm=util.orMap.locationOf(npc).(d.door_dir);
					if(rm==0 || metaclass(rm)~=object) continue; !--not a room
					if(util.orMap.locationOf(target)==rm) { !--if the target is in an adjacent room...

						if(target provides lastLocation){ !--if it's a moving NPC that we are following...
							if(target.lastLocation~=util.orMap.locationOf(npc)) rtrue; !--and the last place it was at is here, then we can follow it
							rfalse; !--it didn't just come from here, so we cannot follow it
						}
						rtrue;  !not a moving NPC, so let's not apply strict "following" rules; assume we can follow it because it is in a neighboring room
					}
				}
				rfalse; !--its not in an adjacent room
			]
		,	_getWanderDirection[npc
					rm d;

				util.orArray.clear(self,_tempArray);

				!loop through all directions and find the ones that lead to a valid room
				objectloop(d in compass) {
					rm=util.orMap.locationOf(npc).(d.door_dir);
					if(rm==0 or npc.lastLocation || metaclass(rm)~=object) continue; !--skip if no room or if its the room we just came from
					util.orArray.append(self,_tempArray,rm);
				}

				!if no place left to go, then try going back the way we came in
				if(util.orArray.getLength(self,_tempArray)==0 &&  npc.lastLocation~=0)  util.orArray.append(self, _tempArray, npc.lastLocation);

				if(util.orArray.getLength(self,_tempArray)==0) return; !--still no where to go, which theoretically should not happen in anything more than a one room game, but just in case...

				!select a random room from the list of valid options
				rm=util.orArray.get(self,_tempArray,random(util.orArray.getLength(self,_tempArray))-1);

				return util.orMap.getDirectionToAdjacentObject(npc, rm); !--return the direction to the room
			]
		,	_getTarget[npc
					retval;
				if(npc provides followTarget) retval=util.orRef.resolveValue(npc,followTarget);
				return retval;
			]
		,	_getCurrentDestination[npc;
				if(npc provides movePath) return util.orArray.get(npc,movePath, npc._movePointer); !--get the next room in the path
			]
		,	_advancePointer[npc;
				if((npc provides movePath)==false) return; !--if no path, then nothing to do{

				npc._movePointer=npc._movePointer+npc._moveIncrement;

				if(npc._movePointer<0 || npc._movePointer>=util.orArray.getLength(npc, movePath)) {

					switch(npc.movePattern){
						MOVE_PATTERN_REVERSE: npc._moveIncrement=-npc._moveIncrement; !--reverse direction
							npc._movePointer=npc._movePointer+npc._moveIncrement+npc._moveIncrement; !--undo the previous increment then move again in the new direction
							break;
						MOVE_PATTERN_REPEAT: npc._movePointer=0; !--if we are at the end of the path, then go back to the begining
							break;
						MOVE_PATTERN_SINGLE: npc._movePointer=0;
								npc.movePattern=MOVE_PATTERN_HALT; !--stop moving
							break;
					}
				}
			]
		with _tempArray 0 0 0 0 0 0 0 0 0 0 !--conceptually, this private; but orArray needs it visible to be able to work on it
		,	doesApplyToCharacter[npc; if(npc ofclass orNpcSkillMove) rtrue; rfalse;]
		,	defaultPriority 5
		,	canPerform[npc
					target;
				if(util.orRef.resolveValue(npc,movePattern)==MOVE_PATTERN_HALT) rfalse;
				target=self._getTarget(npc);
				if(target~=0 && self._targetCanBeFollowed(npc, target)) rtrue;
				if(self._getCurrentDestination(npc)) rtrue;
				if(util.orRef.resolveValue(npc,doesWander)==true) rtrue;
				rfalse;
			]
		,	perform[npc
					target dir rv;

				target=self._getTarget(npc);

				!--if following something, and have already found it, then call the notification routine, but do nothing else
				if(target~=0 && util.orMap.locationOf(target)==util.orMap.locationOf(npc)){
					if(npc provides moveTargetFound)
						rv=npc.moveTargetFound(target);

					if(rv==false) return;
					!--if return is true, then fall through and try the next option for determining where to go
				}

				!--if following something not found, then march toward it
				if(target~=0 && self._targetCanBeFollowed(npc, target)){
					dir=util.orMap.getDirectionToAdjacentObject(npc, target); !--get the direction to the target
					self.moveInDirection(npc, dir, target);
					return;
				}

				!--follow the path specified
				target=self._getCurrentDestination(npc);
				if(target~=0) {
					!march toward target

					target=util.orMap.pathFinder.determinePath(npc, target); !--determine the path to the next room
					if(target<0) return;

					dir=util.orMap.getDirectionToAdjacentObject(npc, target);
					if(dir~=0){

						self.moveInDirection(npc, dir);

						!if end, then change position and/or increment or notify complete
						if(util.orMap.locationOf(npc)==self._getCurrentDestination(npc)) {
							if(npc provides moveArrived)
								rv=npc.moveArrived(self._getCurrentDestination(npc));

							self._advancePointer(npc); !--advance the path pointer
						}
						return;
					}
				}
				!--if all else fails, then just wander around in a near random fashion
				if(npc provides doesWander && util.orRef.resolveValue(npc,doesWander)==true){
					self._getWanderDirection(npc);
					self.moveInDirection(npc, self._getWanderDirection(npc));
					return;
				}

			]
		,	moveInDirection[npc dir followTarget
					sv loc;
				loc=util.orMap.locationOf(npc);
				sv=keep_silent;
				if(canPlayerWitness(npc)==false)  keep_silent=2; !--suppress messages
					<go dir followTarget, npc>;
				keep_silent=sv; !--restore the original value

				if(util.orMap.locationOf(npc)~=loc) npc.lastLocation=loc; !--if we moved, then set the last location to the room we just came from
				if(canPlayerWitness(npc)) {
					sv=actor;
					actor=npc;
					L__M(##Go, 8, dir, followTarget);
					actor = sv;
				}
			]

	;

	class	orNpcSkillMove
		with _movePointer 0
		,	_moveIncrement 1
		,	movePattern MOVE_PATTERN_REVERSE
		,	lastLocation 0
		,	doesWander true
		,	moveInDirection[dir;
				_orNpcMoveEngine.moveInDirection(self, dir); !--call the engine to do the work
			]
	;
#endif; !--After VERBLIB
!======================================================================================
! AFTER Grammar
#iftrue (LIBRARY_STAGE == AFTER_GRAMMAR);

#endif; !--After Grammar
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE





! 	class ORNPC_movement
! 		with	act_move[abletest d;
! 					if(abletest==true) {
! 						if(self.follow_object~=0 && ScopeCeiling(self.follow_object)==ScopeCeiling(self)) rfalse;
! 						return ValueOrRun(self,can_move);
! 					}
! 					if(self.follow_object~=0){
! 						d=self.ajoining_room_to_direction(ScopeCeiling(self.follow_object));
! #ifdef USE_ORPathMaker;
! 						if(d==0 && valueorrun(self,tracker)==true){
! 							d=ORPathMaker.Determine_Path(self,self.follow_object);
! 							if(d<0)
! 								d=0;
! 							else
! 								d=self.ajoining_room_to_direction(d);
! 						}
! #endif;
! 						if(d~=0) {
! 							self.move_to(d);
! 							return;
! 						}
! 					}
! 					if(self.#path>0 && self.path~=0)
! 						self.move_path();
! 					else
! 						self.move_random();
! 				]
! 		,		tracker false
! 		,		can_move		true
! 		,		can_move_save	true
! 		,		priority_save	0
! 		,		can_move_to[obj; !--can move to or through given object
! 					if(metaclass(obj)~=object) return false;
! 					if(obj hasnt locked || (obj provides with_key && parent(obj.with_key)==self)) return true;
! 					return false;
! 				]
! 		,		halt[;
! 					!---Note, the below line should not call for the value of can move (as a routine)
! 					!---   since we are checking only to see if it is turned all the way off
! 					if(self.can_move~=false){
! 						self.can_move_save=self.can_move;
! 						self.can_move=false;
! 					}
! 				]
! 		,		unhalt[;
! 					!---Note, the below line should not call for the value of can move (as a routine)
! 					!---   since we are checking only to see if it is turned all the way off
! 					if(self.can_move==false){
! 						self.can_move=self.can_move_save;
! 						self.can_move_save=false;
! 					}
! 				]
! 		,		start_following[obj;
! 					self.unhalt();
! 					if(obj provides priority) {
! 						self.priority_save=self.priority;
! 						self.priority=obj.priority+1;
! 					}
! 					self.follow_object=obj;
! 				]
! 		,		stop_following[;
! 					self.priority=self.priority_save;
! 					self.follow_object=0;
! 				]
! 		,		follow_object	0
! 		,		path			0
! 		,		path_ptr		0
! 		,		path_ptr_inc	1
! 		,		reverse_at_path_end false
! 		,		last_location	0
! 		,		no_where_to_go	0
! 		,		missing_path_msg[;
! 					print (ig)CIVerb(self,"looked","look")," a little disoriented for a moment. ";
! 				]
! 		,		ajoining_room_to_direction[room t o; !---given an ajoining room, return the dir_obj the npc needs to travel to get there
! 					objectloop(t in compass) { !---t is the direction object
! 						o=parent(self).(t.door_dir);
! 						if(o~=0 && (o==room ||(o provides door_to && valueorrun(o,door_to)==room))) return (t.door_dir);
! 					}
! 					return 0;
! 				]
! 		,		FindLocationInPath[loc t;
! 					for(t=0:t<self.#path/WORDSIZE:t++){
! 						if(self.&path-->t==loc) return t;
! 					}
! 					return -1;
! 				]
! 		,		move_path [t; !called when an NPC is following a path rather than wandering at random
! 					!-- Get next item in list, ignore if needed
! 					t=self.&path-->(self.path_ptr);
! 					self.path_ptr=self.path_ptr+self.path_ptr_inc;	!-- move pointer to next position
! 					if(t==0 || t==scopeceiling(self)) return; !--return if empty element or the current location

! 					!-- translate to appropriate direction
! 					if(scopeceiling(t)~=compass) !if not a direction, then really is a room, so turn it into a direction
! 						t=self.ajoining_room_to_direction(t);
! 					else{ !--is a direction
! 						t=t.door_dir;
! 						if(self.path_ptr_inc<0) t=ReverseDirection(t);
! 					}

! 					if(t==0) !-- problem with path?
! 						self.missing_path();
! 					else
! 						self.move_to(t); !-- move in determined direction

! 					!-- handle if reached end of list
! 					if(self.path_ptr>=(self.#path/WORDSIZE) || self.path_ptr<0 )
! 						self.handle_end_of_path();
! 				]
! 		,		handle_end_of_path[;
! 					if(self.reverse_at_path_end==false) { !--option 1, wrap around to the begining (if not in retreace steps mode)
! 						self.path_ptr=0;
! 					}
! 					else{ !--option 2, reverse direction
! 						self.path_ptr_inc=self.path_ptr_inc*-1; !reverse direction
! 						self.path_ptr=self.path_ptr+(2*self.path_ptr_inc); !do twice (to undo the previous adjustemnt)
! 					}
! 				]
! 		,		missing_path[t; !--there is a problem with the path...
! 					if(self.missing_path_msg~=0 && CanPlayerWitness(self)==true) printorrun(self,missing_path_msg); !--print a message to indicate a problem with the path
! 					self.move_random(); !--wander instead
! 					t=self.FindLocationInPath(parent(self)); !--see if we can find the new location in the path we have been following
! 					if(t~=-1)self.path_ptr=t;
! 					return;
! 				]
! 		,		move_random !when wandering/exploring the npc will pick a random direction, that he did not just come from, and head that way
! 				[togo t o;
! 					!step one: get list of possible directions to travel
! 					npctemparray-->0=0;
! 					objectloop(t in compass) {
! 						o=parent(self).(t.door_dir);
! 						if(o==0 || metaclass(o)~=object) continue;
! 						if(o has door) o=valueorrun(o,door_to);
! 						if(	self.last_location~=o && self.can_move_to(o)==true) {
! 							npctemparray-->0=(npctemparray-->0)+1;
! 							npctemparray-->(npctemparray-->0)=t; !--save dir_obj that must be traveled
! 						}
! 					}
! 					!if no place left to go then try going back the way we came in
! 					if(npctemparray-->0==0)
! 						togo=self.ajoining_room_to_direction(self.last_location);
! 					else
! 						togo=(npctemparray-->random(npctemparray-->0)).door_dir;
! 					!---at this point, togo is either equal to the destination-direction, or is nothing
! 					if(togo)
! 						return self.move_to(togo);
! 					else
! 						if(self.no_where_to_go~=0) printorrun(self,no_where_to_go);

! 				]
! 		,		move_to [ direction obj; !---is called to actually generate the actions to move the NPC in the given direction
! 					self.last_location=parent(self);
! 					obj=valueorrun(parent(self),direction); !---get the destination
! 					if(obj has door) {
! 						if(obj has locked && obj.with_key in self) self.doverb(##Unlock, obj,obj.with_key);
! 						if(obj hasnt open) self.doverb(##Open, obj);
! 						obj=valueorrun(obj,door_to);
! 					}
! 					self.doverb(##go,DirectionObjFromDirection(direction));
! 					if(parent(self)==self.last_location) {
! 						!print "[ERROR:  npc was not able to travel in direction ",(ig)LanguageDirection(direction),".]";
! 						if(self.missing_path_msg~=0 && CanPlayerWitness(self)==true) printorrun(self,missing_path_msg); !--print a message to indicate a problem with the path
! 						return; !---didn't go anywhere...
! 					}
! 				]
! 	;

! !======================================================================================
! #endif; #ifnot; #ifndef LIBRARY_GRAMMAR; #ifndef ORNPC_movement_DONECODE;constant ORNPC_movement_DONECODE; #ifdef ORNPC_movement_DONECODE; #endif; !--suppress warning
! !  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
! ! C O D E   section (for code that falls between VERBLIB and GRAMMAR)
! !======================================================================================
! #ifndef bkt; !--support for bracketed parser messages, without dependancies
! 	[bkt o; print o;];
! 	[bkt_c o; ];
! 	[bkt_o o; ];
! #endif;

! 	object _onm_init LibraryExtensions
! 		with ext_messages[;
! 		FollowNPC:
! 			switch(lm_n){
! 				1: print (ig)CIVerb(actor,"began","begin")," to follow ",(TheMe)noun,", but ",(ig)vrb(actor,"seemed","seem")," to have lost ",(me)noun,". ";
! 					rtrue;
! 			}
! 		ContinueFollowingObject:
! 			switch(lm_n){
! 				1:	if(noun==actor){
! 						print (ig)CICant(actor)," follow ",(Myself)noun,". ";
! 					}
! 					else{
! 						print (ig)CICant(actor)," follow ",(TheMe)noun,". ";
! 					}
! 					print_ret (ig)CIHave(noun)," not gone anywhere. ";
! 				2: print_ret (ig)CIVerb(actor,"looked","look")," at ",(TheMe)noun," and then ",(ig)vrb(actor,"nodded","nod")," agreeably. ";
! 				3: print_ret (ig)CIVerb(actor,"shook","shake")," ",(my)actor," head. ~I don't follow,~ ",(ig)CIVerb(actor,"said","say"),". ";
! 			}
! StopVague:
! 					bkt_o();
! 					print "You'll need to be more specific about what you would like stopped.";
! 					bkt_c();
! 					new_line;
! 					rtrue;
! 		StopFollowing:
! 			switch(lm_n){
! 				1:	print_ret (ig)CIVerb(actor,"stopped","stop")," paying attention to ",(TheMe)noun," and ",(ig)vrb(actor,"looked","look")," away.";
! 				2:  print_ret (CIAm)actor," not following anything. ";
! 			}
! 		Halt:
! 			switch(lm_n){
! 				1:print_ret (CIAm)actor," not moving around. ";
! 				2:print_ret (ig)CIVerb(actor,"stood","stand")," still.";
! 			}
! 		Unhalt:
! 			switch(lm_n){
! 				1:print_ret (CIAm)actor," not standing particularly still. ";
! 				2:print_ret (ig)CIVerb(actor,"resumed","resume")," a more mobile posture. ";
! 			}
! 	];
! 	[DirectionObjFromDirection d;
! 		switch(d){
! 			in_to: d=in_obj;
! 			out_to: d=out_obj;
! 			u_to:d=u_obj;
! 			d_to:d=d_obj;
! 			n_to: d=n_obj;
! 			s_to: d=s_obj;
! 			e_to: d=e_obj;
! 			w_to: d=w_obj;
! 			nw_to: d=nw_obj;
! 			se_to: d=se_obj;
! 			ne_to: d=ne_obj;
! 			sw_to: d=sw_obj;
! 		}
! 		return d;
! 	];
! 	[FollowNPCSub t dir;
! 		t=DirectionFromActor(ScopeCeiling(noun));
! 		if(t~=0) {
! 			dir=DirectionObjFromDirection(t);
! 			if(actor==player)print "(Go ",(name)dir,")^";
! 			<Go dir>;
! 			if(actor~=player)ContinueFollowingObjectSub();
! 			return;
! 		}
! 		L__M(##FollowNPC,1); !--I don't see him
! 	];
! 	[ContinueFollowingObjectSub;
! 		if(actor==player || actor==noun){
! 			L__M(##ContinueFollowingObject,1); !--but the noun is right here
! 			return;
! 		}
! 		if(actor provides follow_object) {
! 			actor.start_following(noun);
! 			L__M(##ContinueFollowingObject,2); !--okay, I'll follow that...
! 			return;
! 		}
! 		L__M(##ContinueFollowingObject,3); !--no, I'm not going to follow that...
! 	];
! 	[NPCInNeighboringRoom o; !isolate NPCs in neighboring rooms that just left the actor's locations
! 		switch(scope_stage)
! 		{
! 			1: return false;
! 			2:	objectloop(o has animate && DirectionFromActor(ScopeCeiling(o))~=0) PlaceInScope(o);
! 				rtrue;
! 			3: "It is unclear who you want me to follow. ";
! 		}
! 	];
! 	[StopVagueSub;
! 		L__M(##StopVague,1);
! 		return false;
! 	];
! 	[StopFollowingSub;
! 		if(actor provides follow_object && actor.follow_object~=0){
! 			L__M(##StopFollowing,1,actor.follow_object);!--okay
! 			actor.stop_following();
! 		}
! 		else L__M(##StopFollowing,2);!--I'm not following anything

! 	];
! 	[HaltSub;
! 		if(actor==player || (actor provides can_move && valueorrun(actor,can_move)==0) || (actor provides halt)==false) return L__M(##Halt,1); !--not doing anything
! 		actor.halt();
! 		L__M(##Halt,2);
! 	];
! 	[UnHaltSub;
! 		if(actor==player || (actor provides can_move && valueorrun(actor,can_move)~=0) || (actor provides unhalt)==false) return L__M(##Unhalt,1); !--not really standing still as such
! 		actor.unhalt();
! 		L__M(##Unhalt,2);
! 	];
! 	!----------------------------------------------------------------------------------------
! 	!This routine determines if a given room is adjacent to the actor's current location.
! 	!If it is, then the direction the actor must travel to reach it is returned
! 	[DirectionFromActor room t o sc;
! 		objectloop(t in compass){
! 			sc=ScopeCeiling(actor);
! 			o=sc.(t.door_dir);
! 			if(o~=0 && metaclass(o)==object && o has door)o=valueorrun(o,door_to);
! 			if(o==room) return t.door_dir;
! 		}
! 		return 0;
! 	];
! 	system_file;
! 	object _npcmoveinit LibraryExtensions
! 		with ext_initialise[o;
! 					objectloop(o ofclass ORNPC_movement){
! 						if((o ofclass ORNPC_doverb)==false) print "^[ERROR: ORNPC_movement behavior requires ORNPC_doverb behavior. (object",(name)o,")]^";
! 						o.register_action(act_move);
! 					}
! 				]
! 	;
! !======================================================================================
! #endif; #ifnot; #ifndef ORNPC_movement_DONEGRAMMAR; constant ORNPC_movement_DONEGRAMMAR; #ifdef ORNPC_movement_DONEGRAMMAR; #endif; !--suppress warning
! !  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
! ! G R A M M A R   section (for code that follows the inclusion of GRAMMAR)
! !======================================================================================
! 	Verb "halt"
! 			*  -> Halt
! 	;
! 	Verb "remain"
! 			*  -> Halt
! 			* 'here' -> Halt
! 	;
! 	Verb "stay"
! 			*  -> Halt
! 			* 'here' -> Halt
! 			* 'still' -> Halt
! 	;
! 	Extend "stand" first
! 			* 'still'  -> Halt
! 			* 'here' -> Halt
! 	;
! 	Verb "unhalt"
! 			*  -> unhalt
! 	;
! 	Verb "follow"
! 			* scope=NPCInNeighboringRoom -> FollowNPC
! 			* noun -> ContinueFollowingObject
! 	;
! 	Verb "stop"
! 			*  -> StopVague
! 			* 'follow'/'following' -> StopFollowing
! 			* 'follow'/'following' creature -> StopFollowing
! 			* 'halt'/'halting'/'standing'/'remaining'/'staying' ->UnHalt
! 			* 'standing'/'remaining'/'staying' 'here'/'still' ->UnHalt
! 	;
! !======================================================================================
! #endif; #endif; #endif;#endif;
! !======================================================================================