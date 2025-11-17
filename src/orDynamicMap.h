!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.05.02 orDynamicMap
! This module dynamically creates the ties between certain rooms based upon movements
! by the player, similar to an effect in the "mars" scene of Adam Cadre's "Photopia".
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License:
!--------------------------------------------------------------------------------------
! To use, create  an instance of the orDynamicMap class with the found_in property
! listing the rooms which  are to be arranged,  in the order they are to appear.  The
! first entry in the list should already be accessable by the player.
!
! As for the rooms that are to be created, there are they should be derrived from initially provide empty directions.
! The cant_go property on the orDynamicMap object, if defined, will be propogated down to
! the rearanged rooms which also provide cant_go, but that do not define it.
!
! To simplify the creation of the rearangable rooms, a simple class is provided
! that defines all directons and the cant_go property as zero.  Note the following example
! which allows the player to explore in virtually any direction from the water's edge:
!
!	orDynamicMap with cant_go "The trees are too dense to travel in that direction."
!		,	found_in wateredge sparseforest forestmidst forestclearing treasuretrove
!	;
!	orDynamicMapRoom wateredge "Edge of water/forest"
!		with s_to "Your boat is that way, but you can't go home until the treasure is found."
!		,	description "This is a small strip of land separating the southern inlet
!				of ocean from the vast forest in all other directions."
!	;
!	orDynamicMapRoom sparseforest "Sparse Forest"
!		with description "This is the start of a vast forest.  The trees here are thin
!				and sparse."
!	;
!	orDynamicMapRoom forestmidst "Midst of Forest"
!		with description "Smack dab in the middle of the forest. Trees are everywhere."
!	;
!	orDynamicMapRoom forestclearing "Forest Clearing"
!		with description "Suddenly clear of trees.  You feel yourself draw closer to the target."
!	;
!	orDynamicMapRoom treasuretrove "Pile of Treasure"
!		with description "At last! The long sought-after pile of treaure.  You are victorious."
!	;
!
!--------------------------------------------------------------------------------------
! Revision History
! 2002.01.01	Initial Creation
! ...			History from orLibrary v1 snipped away.
! 2024.05.02	Refactored for the orLibrary version 2.
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orDynamicMap_STAGE  0;
!======================================================================================
! INCLUDE DEPENDENCIES
#include "orUtilMap";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orDynamicMap_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orDynamicMap_STAGE  < LIBRARY_STAGE);
   #undef      orDynamicMap_STAGE  ;
   Constant    orDynamicMap_STAGE  LIBRARY_STAGE;
   #ifdef      orDynamicMap_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orDynamicMap..."; #endif;
!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);

#endif; !--Before Parser
!======================================================================================
! AFTER PARSER
#iftrue (LIBRARY_STAGE == AFTER_PARSER);
system_file;
	class	orDynamicMapRoom
		with	n_to , s_to, e_to, w_to
		,		ne_to, nw_to, se_to, sw_to
		,		u_to, d_to
		,		in_to, out_to
		,		cant_go
	;
	class orDynamicMap
		has		concealed
		private	_fi_ptr 0
		with	found_in 0 !TODO does this have to be found_in or can it be another property name?
		,		cant_go	0
		,		complete false
		,		react_before[curloc destroom;
					go: if(actor~=player) rfalse; !TODO: can we allow NPCs to create the map?
						curloc=scopeceiling(player);
						do{
							!if(self._fi_ptr>=(self.#found_in/WORDSIZE)) rfalse;
							if(self.complete==true) rfalse;
							destroom=self.&found_in-->(self._fi_ptr);
							self._fi_ptr++;
							if(self.cant_go~=0 && destroom provides cant_go && destroom.cant_go==0) destroom.cant_go=self.cant_go;
						}until(destroom~=curloc);
						if(curloc provides (noun.door_dir) && curloc.(noun.door_dir)==0){
							curloc.(noun.door_dir)=destroom;
							destroom.(util.orMap.reverseDirection(noun.door_dir))=curloc;
						}
						else self._fi_ptr--; !--wasnt able to go that way, undo the previous incremented counter for next attempt
						if(self._fi_ptr>=(self.#found_in/WORDSIZE)) self.complete=true;
				]
	;
#endif; !--After Parser
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);

#endif; !--After VERBLIB
!======================================================================================
! AFTER Grammar
#iftrue (LIBRARY_STAGE == AFTER_GRAMMAR);

#endif; !--After Grammar
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE

