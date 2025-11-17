!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.07.04 orUtilMapPathFinder
!
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! The orUtilMapPathFinder extension can be used to determine a path between two locations.
! This functionality is exposed on the orMap helper object, which will be included if
! not already present.  Basic usage is:
!
!	orMap.pathFinder.find(bathroom,bedroom); !start = bathroom; dest = bedroom
!
! The start and dest parameters can be rooms or objects, including characters.
! The routines will resolve passage through doors.
!
! The return values for this routine are:
!
!	-1 : There is no connecting path between the two given objects.
!	-2 : Ran out of workspace while trying to calculate path.  Resolve this by increasing
!			the value of PATHDEPTH.
!	-3 : Although there WAS a path correctly determined, there was NOT enough space in
!			the path property to contain it.  TODO: does this fill in the first part of the path?
!	room obj : The first connected room which must be traveled to in order to finally reach
!			the destination.
!
! Additionally, once the path has been correctly determined, the length property contains
! the length and the path property contains the final path. TODO: the length property changes if we start using the helper code.
!
! There is a limited amount of scratch workspace used to determine a correct path.  If
! there is not enough space, the path will not be findable.  The constant PATHDEPTH
! is used to allocate this scratch space.  By default it is set to 30, however it can be
! defined at a greater value in the main program's source.
!
!TODO: changed orDeque to not require properties, but can't see where the calls are implemented here.
!--------------------------------------------------------------------------------------
! Revision History
! 2002.02.09	Initial Creation
! ...			History from orLibrary v1 snipped away.
! 2024.07.04	Refactored for the orLibrary version 2.
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orUtilMapPathFinder_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "orUtilMap";
#include "_orDeque";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orUtilMapPathFinder_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orUtilMapPathFinder_STAGE  < LIBRARY_STAGE);
   #undef      orUtilMapPathFinder_STAGE  ;
   Constant    orUtilMapPathFinder_STAGE  LIBRARY_STAGE;
   #ifdef      orUtilMapPathFinder_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orUtilMapPathFinder...";#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);

	!TODO convert this to use helpers rather than accessing the elements directly
	default PATHDEPTH 32;

	array __orPathFinder_room-->PATHDEPTH;
	array __orPathFinder_distance->PATHDEPTH;

	orDeque orPathFinder LibraryExtensions
		private _endPtr	0
		,		_curPtr	0
		,		_npc	orPathFinder !--self if not defined...strange, but it will work
		,		_add[room level;
					if(self._findRoomDistance(room)==-1) {
						__orPathFinder_room-->self._endPtr=room;
						__orPathFinder_distance->self._endPtr=level;
						self._endPtr++;
					}
				]
		,		_resolveDoor[obj last_room a p l al n retval;
					if(obj hasnt door) return obj;
					if(obj has locked && ((obj provides with_key)==false || (obj.with_key in self._npc)==false)) return 0; !--can it be opened by the character in question?
					!--depending on who wrote it, the door might look at parent(actor), parent(player), location, or actors_location.
					a=actor; p=player; l=location; al=actors_location; n=parent(self._npc); !--save old
					actor=self._npc; player=self._npc; location=last_room; actors_location=last_room; move self._npc to last_room;	!--set new
					retval=valueorrun(obj,door_to);			!--determine door
					actor=a; player=p; location=l; actors_location=al;
					if(n==nothing or 0)
						remove self._npc;
					else
						move self._npc to n;!--restore the old
					return retval;
				]
		,		_process[endroom r cr cl t;
					while(self._curPtr<self._endPtr){
						cr=__orPathFinder_room-->(self._curPtr);
						cl=self._findRoomDistance(cr);
						objectloop(t in compass){
							if(cr provides (t.door_dir)){
								r=cr.(t.door_dir);
								if(metaclass(r)==object) {
									if(r has door) r=self._resolveDoor(r,cr);
									if(r~=0){
										self._add(r,cl+1);
										if(r==endroom) return cl+1; !--success.  return needed length for path
										if(self._endPtr>PATHDEPTH) return -2; !--Out of space
									}
								}
							}
						}
						self._curPtr++;
					}
					return -1; !--completed search, with no possible path found
				]
		,		_unwindPath[pos d room r t; !--called once the search algorith has completed successfully, to find the shortest path and put it in  the property pointed to by self.path
					room=__orPathFinder_room-->(self._endPtr-1); !--locate the endroom
					pos=self._findRoomDistance(room); !--length of path

					if((self.#path/WORDSIZE)<pos) return -3; !--path too small
					while(pos>0){
						self.&path-->(pos-1)=room;
						objectloop(d in compass){
							if(room provides (d.door_dir)){
								r=room.(d.door_dir);
								if(r has door) r=self._resolveDoor(r,room);
								if(r~=0){
									t=self._findRoomDistance(r);
									if(t>-1 && t<pos) break;
								}
							}
						}
						room=r;
						pos--;
					}
					return self.&path-->0;
				]
		,		_directionOfAdjacentRoom[startRoom adjacentRoom d r;
					objectloop(d in compass){
						if(startRoom provides d.door_dir){
							r=startRoom.(d.door_dir);
							if(r has door) r=self._resolveDoor(r,startRoom);
							if(r==adjacentRoom) return d;
						}
					}
					return -1;
				]
		,		_findRoomDistance[room
						t;
					for(t=0:t<self._endPtr:t++){
						if(__orPathFinder_room-->t==room)
							return __orPathFinder_distance->t;
					}
					return -1;
				]
		with	ext_initialise[;_orMap.pathFinder=self;] !install this into the orMap object, where it belongs...
		,		path 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
		,		determinePath[npc_cntx endroom
						t startroom;
					if(parent(npc_cntx)==0){
						startroom=npc_cntx; !--room was passed in instead of a character
						self._npc=self;
					}
					else{
						startroom=scopeceiling(npc_cntx);
						self._npc=npc_cntx;
					}
					if(parent(endroom)~=0)endroom=scopeceiling(endroom); !--an object was specified instead of a room... determine
					if(startroom==endroom) return 0;
					for(t=0:t<self._endPtr:t++){ 	!-- first lets clear everything out
						__orPathFinder_room-->t=0;
						__orPathFinder_distance->t=0;
					}
					self._endPtr=self._curPtr=0;
					self._add(startroom,0);
					t=self._process(endroom);
					if(t<0) return t; !--- Errors? -1: not reachable; -2: out of workspace
					return self._unwindPath(); !---all was good, lets fill in the path property and return (either an error or the first room in the list...)
				]
		,		convertPathToDirections[startingLoc
						i room prevRoom;
					!for(i=util.orMap.path.getLength(path)-1:i>=0:i--){
					for(i=util.orArray.getLength(self,path)-1:i>=0:i--){

						room=self.&path-->i;
						if(i>0)
							prevRoom=self.&path-->(i-1);
						else
							prevRoom=startingLoc;

						self.&path-->i=self._directionOfAdjacentRoom(prevRoom, room);
					}
				]
	;
#endif; !--After verblib
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
