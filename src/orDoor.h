!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.04.11 orDoor
! orDoor simplifies the creation of typical doors down to a single line of code.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! The orDoor class represents a simple door, but utilizes generic code for the door_to,
! door_dir properties so derived doors need not define these properties.
! Additionally, this module probes the world map for directions which lead to
! orDoor objects and attempts to fill in the found_in property.
!
! INSTEAD of this...
!
!		Object RedDoor "red door"
!			 with	name "door" "red"
!			 ,     door_dir
!					[;
!						if (location == LivingRoom)
!							return n_to;
!						else
!							return s_to;
!					]
!	        ,     door_to
!					[;
!						if (location == LivingRoom)
!							return DiningRoom;
!						else
!							return LivingRoom;
!					]
!	        ,     found_in LivingRoom DiningRoom
!			has openable door static scenery;
!
! ...you can simply write this:
!
!		orDoor RedDoor "red door" with	name 'door' 'red';
!
!--------------------------------------------------------------------------------------
! Revision History
! 2001.04.20	Initial Creation
! ...			History from orLibrary v1 snipped away.
! 2024.04.11	Refactored for the orLibrary version 2.
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orDoor_STAGE 0;
!======================================================================================
! INCLUDE DEPENDENCIES
#include "orString";
#include "_orInfExt";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orDoor_STAGE	; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orDoor_STAGE < LIBRARY_STAGE);
   #undef      orDoor_STAGE ;
   Constant    orDoor_STAGE LIBRARY_STAGE;
   #ifdef      orDoor_STAGE ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orDoor..."; #endif;
!======================================================================================
! AFTER PARSER
#iftrue (LIBRARY_STAGE == AFTER_PARSER);
	system_file;

	class orDoor
		has static door openable scenery
		with	door_to
				[;
					if((self.&found_in-->0)==parent(actor)) return self.&found_in-->1;
					return self.&found_in-->0;
				]
		,		door_dir
				[;
					if(parent(actor) provides n_to && parent(actor).n_to==self) return n_to;
					if(parent(actor) provides s_to && parent(actor).s_to==self) return s_to;
					if(parent(actor) provides e_to && parent(actor).e_to==self) return e_to;
					if(parent(actor) provides w_to && parent(actor).w_to==self) return w_to;
					if(parent(actor) provides ne_to && parent(actor).ne_to==self) return ne_to;
					if(parent(actor) provides se_to && parent(actor).se_to==self) return se_to;
					if(parent(actor) provides nw_to && parent(actor).nw_to==self) return nw_to;
					if(parent(actor) provides sw_to && parent(actor).sw_to==self) return sw_to;
					if(parent(actor) provides u_to && parent(actor).u_to==self) return u_to;
					if(parent(actor) provides d_to && parent(actor).d_to==self) return d_to;
					return self;
				]
		,		found_in 0 0
	;
endif; !--AFTER PARSER
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
	orInfExt with	ext_initialise[room dir dest str;
		str=util.orStr.new(); !--create a string object, to suppress output
		objectloop(room provides	n_to or s_to or e_to or w_to
						or	nw_to or sw_to or ne_to or se_to
						or	u_to or d_to){
			objectloop(dir in compass){
				if(room provides (dir.door_dir)){
					if(metaclass(room.(dir.door_dir))~=object) continue; !--don't do anything with a string...
					str.capture(); !--suppress any messages that the direction routines may spit out...
					dest=valueorrun(room,(dir.door_dir));
					str.release();

					if(dest ofclass orDoor && WordInProperty(room,dest,found_in)==false){
						if((dest.&found_in-->0)==0)
							(dest.&found_in-->0)=room;
						else{
							if((dest.&found_in-->1)==0)
								(dest.&found_in-->1)=room;
							else{
	#ifdef DEBUG;
								print "Warning. Inconsistency found with map.  Door ~",(object)dest,"~ has too many found_in entries detected.";
	#endif;
							}
						}
					}
				}
			}
		}
		str.free(); !when we are done with our string, let's free up its buffer for other purposes
#ifdef DEBUG;
		objectloop(dest ofclass orDoor && dest provides found_in && dest.#found_in>1){
			if(dest.&found_in-->0==0 || dest.&found_in-->1==0) {
				print "^ERROR found.  Door ",(object)dest," does not connect two locations.  An unmodified version of the library routine MoveFloatingObjects() will generate errors.^";
			}
		}
	#endif;
	];
#endif; !--AFTER VERBLIB
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
