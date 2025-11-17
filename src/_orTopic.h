!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.28 orTopic
! This class provides a concept of knowledge topics which are shared across multiple
! extensions.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
!
!--------------------------------------------------------------------------------------
! Revision History
! 2001.10.01	Initial Creation
! ...			History from orLibrary v1 snipped away.
! 2024.09.04	Refactored for the orLibrary version 2.
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orTopic_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "orUtilArray";
#include "orUtilParser";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orTopic_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orTopic_STAGE  < LIBRARY_STAGE);
   #undef      orTopic_STAGE  ;
   Constant    orTopic_STAGE  LIBRARY_STAGE;
   #ifdef      orTopic_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orTopic..."; #endif;
!======================================================================================
! AFTER PARSER
#iftrue (LIBRARY_STAGE == AFTER_PARSER);
	system_file;

	attribute responseOnly;

	property individual knownBy;
	property individual context;

	class orTopic
#ifdef orRecogName_STAGE;
		has noRecogName
#endif;
		private _getCharacterId[p;
#ifdef orNpc_STAGE;
				if(p ofclass orNPC) return p.npcId;
#endif;
				if(p==player) return 0;
				return -1;

			]
		with dissemTrack 0
		,	quip 0
		,	isKnownBy[npc v;
				if((self provides knownBy)==true){
					if(util.orArray.getSize(self,knownBy)==1){
						v=util.orArray.get(self,knownBy,0);
						if(v==true) rtrue;
						if(metaclass(v)==routine) return v(npc);
					}
					if(wordinproperty(npc, self, knownBy)) rtrue; !-explicity defined as known by the npc
				}
				else if(parent(self)~=0 && parent(self) provides isKnownBy) return parent(self).isKnownBy(npc); !allow topics to inherit their "known by" from parents.
				rfalse;
			]
		,	isInContextFor[npc v;
				if(util.orArray.getSize(self,context)==1){
					v=util.orArray.get(self,context,0);
					if(v==true) rtrue; !explicitly defining as 'in context' for everyone
					if(metaclass(v)==routine) return v(npc);
				}
				if(wordinproperty(npc, self, context)) rtrue; !-explicity defined as in context for the npc

				if((self provides context)==false){
					if(parent(self)==0 || (parent(self) provides isInContextFor)==false ) rtrue; ! no context to inherit, so assume in context
					return parent(self).isInContextFor(npc); !allow topics to inherit their "context" from parents.
				}

				rfalse; !topic DOES provide context, but it wasn't previously resolved; therefore, NOT in context.
			]
		,	isAppropriateFor[c1 c2;
				if(self.isTellable() && self.isKnownBy(c1) && self.isInContextFor(c2) && (self.hasBeenToldTo(c2, c1) == false)) rtrue; !--tell

				if(self.isAskable() && !--an askable topic
					self.isKnownBy(c2) && !--known by the second character
					self.isInContextFor(c1) &&  !--in context for the first character
					(self.hasBeenToldTo(c1, c2) == false)!--has NOT already been told to the first character by the second character already
					) rtrue;
				rfalse;
			]
		,	isAskable[;
				if(util.orArray.getSize(self,quip)>1) rtrue;
				rfalse;
			]
		,	isTellable[;
				if(self.quip==0) rfalse; !--if quip is not defined, then it is not tellable.
				if(self hasnt responseOnly) rtrue;

				rfalse;
			]
		,	chooseobject[code;
				if(code<2) return -1;
				if(self.isInContextFor(advance_warning)) return 3; !-- by default, advance_warning will not be resolved; however, some verbs (e.g., sayTopic) need it.
				return util.orParser.excludeObjFromDisambiguation(self);
			]
		,	recordTell[p1 p2
					t;

				p1=self._getCharacterId(p1);
				p2=self._getCharacterId(p2);

				t=self.getTracePos();
				if(t==-1) return;

				self.&dissemTrack->t=p1;
				self.&dissemTrack->(t+1)=p2; !TODO.  Will this cause a probablem, trying to put a double byte number into a single byte?  Or will it just trim the MSB and add what's left?
			]
		,	getTracePos[
					t;

				for(t=0:t<self.#dissemTrack:t=t+2){
					if(self.&dissemTrack->t==0 && self.&dissemTrack->(t+1)==0) return t;
				}
				#ifdef DEBUG;
					print "WARNING! dissemTrack array is full in topic ",(name)self," (currently sized at ",self.#dissemTrack,")";
				#endif;
				return -1;
			]
		,	hasBeenTold[;
				if(self.&dissemTrack-->0==0) rfalse;
				rtrue;
			]
		,	hasBeenToldTo[pToldTo pToldBy
					t;

				if(pToldBy~=0) return self.hasBeenToldBy(pToldBy,pToldTo); !--redirect this, since the "both condition" is implemented in the variant routine
				pToldTo=self._getCharacterId(pToldTo);
				if(pToldTo==-1) "ERROR: hasBeenToldTo() takes the player objects or an orNpc instance as a parameter.";
				for(t=0:t<self.#dissemTrack:t=t+2){
					if(self.&dissemTrack->t==0 && self.&dissemTrack->(t+1)==0) break;
					if(self.&dissemTrack->(t+1)==pToldTo) rtrue; !
					if(self.&dissemTrack->t==0 && self.&dissemTrack->(t+1)==0) break;
				}
				rfalse;
			]

		,	hasBeenToldBy[pToldBy pToldTo
					t testBoth;
				pToldBy=self._getCharacterId(pToldBy);

				if(pToldTo~=0) {
					testBoth=true;
					pToldTo=self._getCharacterId(pToldTo);
				}

				if(pToldBy==-1) "ERROR: hasBeenToldBy() takes the player objects or an orNpc instance as a parameter (FIRST parameter failed this condition).";
				if(pToldTo==-1) "ERROR: hasBeenToldBy() takes the player objects or an orNpc instance as a parameter (SECOND parameter failed this condition).";

				for(t=0:t<self.#dissemTrack:t=t+2){
					if(self.&dissemTrack->t==0 && self.&dissemTrack->(t+1)==0) break;
					if(self.&dissemTrack->t==pToldBy) {
						if(testBoth==false) rtrue;
						if(self.&dissemTrack->(t+1)==pToldTo) rtrue;
					}
				}
				rfalse;
			]
	;

#endif; !--After Parser
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
