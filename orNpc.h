!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.09.02 orNpc
! A base class and controlling daemon for NPCs.
!--------------------------------------------------------------------------------------
! Created by Jin Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
!
!--------------------------------------------------------------------------------------
! Revision History
! 2002.02.08	Initial Creation
! ...			History from orLibrary v1 snipped away.
! 2024.09.02	Refactored for the orLibrary version 2.
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orNpc_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "orUtilArray";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orNpc_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orNpc_STAGE  < LIBRARY_STAGE);
   #undef      orNpc_STAGE  ;
   Constant    orNpc_STAGE  LIBRARY_STAGE;
   #ifdef      orNpc_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orNpc...";#endif;
!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);
!	replace MoveFloatingObjects;
!	property additive	heartbeat;
property additive npcInit;
#endif; !--Before Parser
!======================================================================================
! AFTER PARSER
#iftrue (LIBRARY_STAGE == AFTER_PARSER);
system_file;
	!---Can the player see what the current actor is doing? (current actor may be overriden with a passed in object)
	!- - - - - - - - - - - - - - - - - - - - - - - -
	!---	Checking the scope of the noun and second objects is no longer done. When it was,
	!---	NPCs that continuously tried to refer to an object that was not in scope for them, but
	!---	WAS in scope for the player, generated library messages which were inappropriate.
	[canPlayerWitness one scp;
		scp=ScopeCeiling(player);
		if(one==0) one=actor;
		return scp==ScopeCeiling(one);
!--		if(scp==ScopeCeiling(actor) || (noun && scp==ScopeCeiling(noun)) || (second && scp==ScopeCeiling(second)) ) return true;
!--		return false;
	];
#endif; !--After Parser
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
! [ MoveFloatingObjects i k l m address flag;
! 		objectloop (i)
! 		{
! 			if(i==parent(actor)) continue; !--lets not try to place an object inside of itself or remove the actors current location from the game world (This can only happen if an actor enters a floating object)
! 			address=i.&found_in;
! 			if (address~=0 && i hasnt absent)
! 			{
! 				if (ZRegion(address-->0)==2)
! 				{   if (i.found_in() ~= 0) move i to parent(actor); else remove i;
! 				}
! 				else
! 				{   k=i.#found_in;
! 					for (l=0: l<k/WORDSIZE: l++)
! 					{   m=address-->l;
! 						if (m==parent(actor) || m in parent(actor))
! 						{
! 							if (i notin parent(actor)) move i to parent(actor);
! 							flag = true;
! 						}
! 					}
! 					if (flag == false) { if (parent(i)) remove i; }
! 				}
! 			}
! 		}
! 	];

! 	array		npctemparray-->0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0; !---this is a generic array for use by various npc behaviors
! 	array		npctemparraybaselevel-->0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0; !---this is a generic array by base level object only
	class orNpcSkill with doesApplyToCharacter[npc; npc=npc; rfalse;],
			defaultPriority 5,
			canPerform true,
			perform[npc; npc=npc;]
	;

	Array tmpArray	table 10;
	class orNpc
		has		animate !transparent !TODO: why transparent?
		with	npcId 0
		,		isEnabled		true
		,		priority		0
		!,		tmpArray		0 0 0 0 0 0 0 0 0
		,		skills			0 0 0 0 0 0 0 0 0
		,		doNothing		0 !---message to print when npc takes no action
		,		continuedAction	0
		,		possessed false
		, 		interactingWith 0
		!,		heartbeat_post	0
		!,		dialoguePool 	0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 !TODO move this to the dialogue skill
		,		heartbeat[actn;
					! npctemparray-->0=npctemparray-->0; !--suppress warning
					actn=self.selectAction();

					if(actn~=0)
					 	actn.perform(self);
					else
						if(self.doNothing~=0) printorrun(self,doNothing);
				]
		,		getSkillPriority[skl; return skl.defaultPriority;]
		,		selectAction[t p act pri;
					util.orArray.clear(tmpArray);

					if(self.continuedAction~=0){
					 	if(self.getSkillPriority(self.continuedAction)<0)
					 		self.continuedAction=0;

					}

					for(t=0:t<util.orArray.getLength(self,skills):t++){
						act=util.orArray.get(self,skills,t);
						if(act==0) continue; !we could break to speed things up; however, with this approach, we just allow for actions to be zeroed out, but let following actions to still work.
						if(util.orRef.resolveValue(act,canPerform,self)~=true) continue;
						p=self.getSkillPriority(act);

						if(p>pri){
							pri=p;
							util.orArray.clear(tmpArray);
							util.orArray.insert(tmpArray, act);
						}
						if(p==pri) util.orArray.insert(tmpArray, act);
					}

					if(util.orArray.getLength(tmpArray)>0){
						return util.orArray.get(tmpArray,random(util.orArray.getLength(tmpArray))-1);
					}
					return 0; !--no action
		 		]
			,	learnSkill[skl;
					util.orArray.append(self,skills,skl);
				]
	;
	!---ORNPCControl  object
	object orNpcControl LibraryExtensions
		with areEnabled true !are NPCs active
		,	areActing false
		,	getById[id
					npc;
				objectloop(npc ofclass orNpc) if(npc.npcId==id) return npc;
				return 0;
			]
		,	daemon [npc; ! t lowest_priority highest_priority outcount savedparent savedparentloc;
				if(self.areEnabled==false) return;

				self.areActing=true;
				objectloop(npc ofclass orNpc && valueorrun(npc,isEnabled)==true) {
					if(npc==player && valueorrun(npc,possessed)==false) continue;
					npc.heartbeat();
				}
				self.areActing=false;

				!--in the weird case that the player is inside of a floating object...
				!savedparent=parent(player);
				!savedparentloc=parent(parent(player));


				! self.&npclist-->0=0;
				! objectloop(npc ofclass ORNPC && npc has animate && metaclass(parent(npc))==object) {
				! 	self.&npclist-->0=(self.&npclist-->0)+1;
				! 	self.&npclist-->(self.&npclist-->0)=npc;
				! 	if(npc.priority>highest_priority)highest_priority=npc.priority;
				! 	if(npc.priority<lowest_priority)lowest_priority=npc.priority;
				! }
				! for(outcount=lowest_priority:outcount<=highest_priority:outcount++){
				! 	for(t=0:t<self.&npclist-->0:t++){
				! 		npc=self.&npclist-->(t+1);
				! 		if(npc==player && valueorrun(npc,ownwill)==false) continue;
				! 		if(npc.priority~=outcount) continue;
				! 		ClearPronoun();
				! 		actor=npc;
				! 		MoveFloatingObjects();
				! 		npc.heartbeat();
				! 		if(npc.heartbeat_post~=0) npc.heartbeat_post();
				! 		!if(CanPlayerWitness(npc)==true) PronounNotice(npc);
				! 	}
				! }
				! ClearPronoun();
				! actor=player;		!restore the actor
				! MoveFloatingObjects();
				! self.areActing=false;

				! !--handle the odd case that the player is within a floating object...
				! if(parent(player)==savedparent && parent(parent(player))~=savedparentloc){ !-- if the player wasn't moved by an NPC...
				! 	move parent(player) to savedparentloc; !--then lets make sure that players location wasn't move around by non-typical use of MoveFloatingObjects
				! }

			]
		,	ext_initialise[npc skl count;
				count=0;
				!on startup, scan through all npcs and record the skills they can perform.  Also let them self initialize
				objectloop(npc ofclass orNPC){
					npc.npcId=++count;
					objectloop(skl ofclass orNpcSkill){
						if(skl.doesApplyToCharacter(npc)) {
							npc.learnSkill(skl);
						}
					}
					if(npc provides npcInit) npc.npcInit();
				}
				StartDaemon(self);
			]
		,
	;
#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
