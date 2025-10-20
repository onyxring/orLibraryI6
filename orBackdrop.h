!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.07.03 orBackdrop
!	This class eases implementation of a generic object which does not need to be
!	referenced in the game.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! Sample use:  TODO Need to comment how this works.  Its nuanced and needs to be commented.
! TODO: Also need to document the use of the "can't do that" property as an acutal before rule to redirect actions.
!
! Complementary extensions
! *	_orString: If included, the "description" text and the "can't do that" text may
!				be orString objects.
! *	orPrint  : If included, the "description" text and the "can't do that" text will
!				resolve print patterns if they exist.
!--------------------------------------------------------------------------------------
! Revision History
! 2001.12.01	Initial Creation (Not really, but this is the earliest date I can find
!				without searching my code archive)
! ...			History from orLibrary v1 snipped away.
! 2024.05.02	Refactored ORProp for the orLibrary version 2; also refactored to
!				include a syntax similar to the scenic.h extension by Richard Barnett,
!				Roger Firth, Stefano Gaburri, and Joe Mason.
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orBackdrop_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orInfExt";
#include "orUtilArray";
#include "orUtilRef";
#include "orBetterChoice";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orBackdrop_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orBackdrop_STAGE  < LIBRARY_STAGE);
   #undef      orBackdrop_STAGE  ;
   Constant    orBackdrop_STAGE  LIBRARY_STAGE;
   #ifdef      orBackdrop_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orBackdrop..."; #endif;
!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);
Constant bdDefault	-1;
Constant bdEnd 		0;
Constant bdDescr	1;
#endif; !BEFORE_PARSER
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);

	system_file;
	property individual backdrops;
	property individual backdrops1;
	property individual backdrops2;
	property individual backdrops3;

	!--an ever present object, which interjects itself if the player is referencing dictionary words listed in the location's backdrops property
	object orBackDropEngine has scenery proper talkable
		private _getEndPos[obj
					i;
				i=self._usedPos;
				if(i<0) return bdDefault;

				for(:i<util.orArray.getSize(obj,self._usedProperty):i++){
					if(util.orArray.get(obj,self._usedProperty, i)==bdEnd) return i;
				}
				return bdDefault;
			]
		,	_getDescriptionPos[obj
					i;
				i=self._getEndPos(obj)-2;  !description is 2 before the end marker
				if(i<1) return bdDefault; !--malformed, we expect at least one dict word to proceed the description
				if(util.orArray.get(obj,self._usedProperty, i)==bdDefault) return bdDefault;
				return i;
			]
		,	_getIgnoreMsgPos[obj i;
				i=self._getEndPos(obj)-1; !ignore message is 1 before end marker
				if(i<2) return bdDefault; !--malformed, we expect at least one dict word and a description to proceed the ignore msg
				if(util.orArray.get(obj,self._usedProperty, i)==bdDescr) i--;
				if(util.orArray.get(obj,self._usedProperty, i)==bdDefault) return bdDefault;
				return i;
			]

		,	_handleOtherActions[obj; return self._printResponse(obj,false); ]
		,   _printResponse[obj isDescription
					i e;
				if(isDescription) !is this backdrop being examined/described, or is the player trying to manipulate it (so we should try to print the "you can ignore this" message)
					i=self._getDescriptionPos(obj);
				else
					i=self._getIgnoreMsgPos(obj);

				if(i==bdDefault) {
					L__M(##Miscellany, 39);
					rtrue;
				}

				!---at this point i should point to the index of the message to print
				e=util.orArray.get(obj,self._usedProperty,i);
				switch(metaclass(e)){
		#ifdef orString;
					Object:
						if(e ofclass orString) e.format("$0").print();  !format here, will resolve orPrint print patterns, if orPrint is included...
		#endif;
					String:
		#ifdef orPrint_STAGE;
						orPrint(e);
		#ifnot;
						print (string) e;
		#endif;
					Routine:
						if(e()==false) L__M(##Miscellany, 39);
						rtrue; !action was redirected, so return true to suppress the automatic line feed

				}
			]
		with _usedPos 0			!--our parse_noun routine determines if the backdrop property is being referenced, and if so...
		,   _usedProperty 0		!--... which position and property are being referenced.
		,   _usedObject  0		!--... and on which object the backdrop is defined
		,	_sayDescription[obj; return self._printResponse(obj,true);  ]
		,	isBackdropReferenced[;
				if(self._usedObject==0) rfalse;
				if(second==0) rfalse;
				if(second == orBackdropEngine) rtrue;
				if(second ofclass orBackdrop) rtrue;
				rfalse;
			]
		,	getContainer[; if(self.isBackdropReferenced()==false) return 0; return self._usedObject; ]
		,	getKeyword[loc;
				if(self.isBackdropReferenced()==false) return 0;
				if(loc~=0 && loc~=self.getContainer()) return 0;
				return util.orArray.get(self._usedObject,self._usedProperty, self._usedPos);
			]
		,	short_name "your surroundings"
		,	found_in [;rtrue;] !-always present
		,   chooseobject[code;
				if(code<2) return 2; !--exclude backdrops from "all" check
				return 0; !--prefer other real objects
			]
		,   description[; self._sayDescription(location);]
		!,	descriptionVerbs ##Examine ##Search 0 0 0 0
		,	descriptionVerbs ##Examine 0 0 0 0 0
		,	registerDescriptionVerb[v; util.orArray.append(self, descriptionVerbs, v); ]
		,	before[obj;
				if(util.orArray.find(self, descriptionVerbs, action)~=-1) {
					rfalse; !--is this a verb which should respond with a description?
				}
				if(obj==0) obj=location;
				if(self._handleOtherActions(obj)==false) !if the handler returned true, then we are probably redirecting an action to skip the terminating new_line, since the other action will do that when it completes.
					new_line;
				rtrue;
			]
	;
	!--a class, from which to create actual objects which proffer the same backdrops property and behavior. This is useful if you
	!  want to create floating backdrops which span multiple rooms, or if you exceed the maximum size of the property array (32 for Z5)
	class orBackdrop has scenery
		with chooseobject[; return -1;],
		description[; return orBackDropEngine._sayDescription(self); ],
		before[; return orBackDropEngine.before(self);]
	;

	orInfExt !--contains hooks
		! searches all backdrop properties for the wrd.  It may be found for than once.
		! The position wrd, within the proeprty, is added resultList array in the following format:
		! 		The last two digits represent the position with the property.
		!		The hundredths digit represents the property it was found in:
		!			 1 - backdrops, 2 - backdrops1, 3 - backdrops2, 4 - backdrops3
		private	_findMatches[obj wrd resultList
						pos;

				util.orArray.clear(self,resultList);
				if(wrd==0) return 0; !no word to match, so return 0

				if(obj provides backdrops){
					for(pos=0:pos<util.orArray.getSize(obj, backdrops):pos++){
						if(util.orArray.get(obj,backdrops,pos)==wrd) util.orArray.append(self, resultList, self._posToEntryId(obj, backdrops, pos));
					}
				}
				if(obj provides backdrops1){
					for(pos=0:pos<util.orArray.getSize(obj, backdrops1):pos++){
						if(util.orArray.get(obj,backdrops1,pos)==wrd) util.orArray.append(self, resultList, self._posToEntryId(obj, backdrops1, pos));
					}
				}
				if(obj provides backdrops2){
					for(pos=0:pos<util.orArray.getSize(obj, backdrops2):pos++){
						if(util.orArray.get(obj,backdrops2,pos)==wrd) util.orArray.append(self, resultList, self._posToEntryId(obj, backdrops2, pos));
					}
				}
				if(obj provides backdrops3){
					for(pos=0:pos<util.orArray.getSize(obj, backdrops3):pos++){
						if(util.orArray.get(obj,backdrops3,pos)==wrd) util.orArray.append(self, resultList, self._posToEntryId(obj, backdrops3, pos));
					}
				}
				return util.orArray.getLength(self,resultList);
			]
		!given a position in a backdrop array, we back up, one entry at a time, until we find the first word in this backdrop object.
		,	_posToEntryId[obj prop pos
					retval;
				for(:pos>=0:pos--){ !we start at the given word and look backwards
					if(util.orArray.get(obj, prop, pos)==bdEnd){
						pos=pos+1;
						break;
					}
				}
				if(pos<0)pos=0;
				switch(prop){
					backdrops: retval=1;
					backdrops1: retval=2;
					backdrops2: retval=3;
					backdrops3: retval=4;
				}
				return (retval*100)+pos;
			]
		with _foundList 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
		,	_seedList  0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 !a list used to capture possible matches
	 	,	ext_beforePrompt[;
				orBackDropEngine._usedProperty=0;
				orBackDropEngine._usedPos=0;
				orBackDropEngine._usedObject=0;
			]
		,	ext_adjudicate[ !--when a reference to a backdrop also matches a real object
					i o retval backDropsConsidered nonBackDropsConsidered;
				!--count up backdrops and real objects
				for(i=0: i<number_matched:i++) {
					o = match_list-->i;
					if(o == orBackdropEngine || o ofclass orBackdrop)
						backDropsConsidered++;
					else{
						nonBackDropsConsidered++;
						retval=o; !we save this off, just in case we end up with only one
					}
				}
				if(backDropsConsidered==0 || nonBackDropsConsidered==0) return false; !--we are dealing exclusively in either backdrops or objects, so nothing for this code to adjudicate

				if(nonBackDropsConsidered==1) {
					no_infer_message=true;
					return retval; !--we are left with a single non-backdrop object, so let's choose that one
				}
				!--we can't decide. We know that we have at least two objects and at least one backdrop, so let's eliminate our backdrops from the list and let subsequent adjudication attempts decide
				for(i=number_matched-1: i>=0:i--) {
					o = match_list-->i;
					if(o == orBackdropEngine || o ofclass orBackdrop) util.orParser.removeMatchPos(i);
				}
				rfalse;
			]
		,	ext_parsenoun [ obj wrd
				i f retval saveWn; !--pull the prop handlers into scope if referencing something by prop words
            if(obj~=orBackDropEngine && (obj ofclass orBackdrop)==false) return -1; !--we aren't considering a backdrop, so decline to answer
			if(obj==orBackDropEngine) obj=parent(obj);
 			saveWn=wn;
			wrd=NextWord(); !get the first word we are matching

			!find all occurances of wrd, across all backDrop properties, placing the matches in _seedList, exiting if none
			if(self._findMatches(obj, wrd, _seedList)==0) return 0;
			!at this point, _seedList contains a list of all the entries across all the backdrop properties, which contain the first word.  Now we'll go through each additional word and reduce our list of possibilities
			wrd=NextWord();
			while(wrd~=0){
				if(self._findMatches(obj, wrd, _foundList)==0) break;  !we didn't find any occurances of the word, so lets stop iterating

				!now _foundList contains a list of all the entries across all backdrop properites, which contain the given word
				!lets compare _foundList and _seedList, removing anything from _seedList which is NOT also in _foundList.  The point here is to eliminate possibilities which aren't referenced by all words
				for(i=util.orArray.getLength(self,_seedList):i>0:i--){
					f=util.orArray.get(self,_seedList,i-1);
					if(util.orArray.find(self,_foundList,f)==-1) util.orArray.removeValue(self,_seedList,f);
				}
				!if we've ended up eliminating all entries in _seedList, then we know that words were used which are not refereneced, so this object's backdrops are not being referenced.
				if(util.orArray.getLength(self,_seedList)==0) return 0;
				wrd=NextWord();
			}

			!save the property and pos here
			retval=util.orArray.get(self,_seedList,0); !note: it is possible that more than one match was returned; just ignore these and return the first match since its as good as any of the others.

			switch(retval/100){
				1:	orBackDropEngine._usedProperty=backdrops;
				2:	orBackDropEngine._usedProperty=backdrops1;
				3:	orBackDropEngine._usedProperty=backdrops2;
				4:	orBackDropEngine._usedProperty=backdrops3;
			}
			orBackDropEngine._usedPos=retval % 100;
			orBackDropEngine._usedObject = obj;

			return wn-saveWn-1;
 		]
	;
#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
