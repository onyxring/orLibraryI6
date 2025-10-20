!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.03.23 ORAdjective [Z, Glulx]
! Add the adjective property for objects to help the parser more clearly understand.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! NORMAL BEHAVIOR:
! Consider the following items. In some cases, the words glass and marble serve
! as adjectives, in others they are nouns:
!
!		object -> glass "drinking glass" with name 'glass' 'drinking';
!		object -> looking_glass "looking glass" with name 'glass' 'looking';
! 		object -> marble "glass marble" with name 'glass' 'marble';
! 		object -> glass_table "glass table" with name 'glass' 'table';
!		object -> marble_table "marble table" with name 'marble' 'table';
!
! The player  cannot simply refer to the "marble", or "glass" objects without the
! parser asking for clarification:
!
! > x glass
! 	Which do you mean, the drinking glass the looking glass or the glass marble?
!
! > x marble
! 	Which do you mean, the glass marble or the marble table?
!
! REVISED BEHAVIOR:
! Using this module, and clearly distinguishing adjectives from nouns...
!
!		object -> glass "drinking glass" with name 'glass', adjective 'drinking';
!		object -> looking_glass "looking glass" with name 'glass', adjective 'looking';
! 		object -> marble "glass marble" with name 'marble', adjective 'glass';
! 		object -> glass_table "glass table" with name 'table', adjective 'glass';
!		object -> marble_table "marble table" with name 'table', adjective 'marble';
!
! provides the parser with enough information to decide correctly, when enough
! information is provided...
!
! > x glass
!   You see nothing special about the drinking glass.
!
! > x marble
!   You see nothing special about the glass marble.
!
! ...while still asking for clarification when the player input is too vague:
!
! > x table
! 	Which do you mean, the glass table or the marble table?
!
! By default, the parser will still match objects, even if only adjectives are provided:
!
! > x drinking
!   You see nothing special about the drinking glass.
!
! But the parser can be made to require at least one noun by changing the
! the orAdjective object's "mode" property, usually in Initialise():
!
!	orAdjective.mode=REQUIRE_NOUN; !--is PREFER_NOUN by default
!
!--------------------------------------------------------------------------------------
! Revision History
! 2002.05.08	Initial Creation
! ...			History from orLibrary v1 snipped away.
! 2024.03.23	Refactored for the orLibrary version 2.
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orAdjective_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orHookStandardLibrary";
#include "orUtilParser";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orAdjective_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orAdjective_STAGE  < LIBRARY_STAGE);
   #undef      orAdjective_STAGE  ;
   Constant    orAdjective_STAGE  LIBRARY_STAGE;
   #ifdef      orAdjective_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orAdjective..."; #endif;
!======================================================================================
! before parser
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);
	property individual adjective;
	property individual requireNoun;
#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
	Constant	REQUIRE_NOUN 0;
	Constant	PREFER_NOUN 1;
	attribute	noun_ref;

	object orAdjective LibraryExtensions
		private _atLeastOneObjectReferencedByNoun 0
		with mode PREFER_NOUN
		, ext_displayStatus[; !--this is the last entrypoint before we get input, so we use this as a convenience
            self._atLeastOneObjectReferencedByNoun=0;
        ]
		, ext_parsenoun [ obj
				a n f savWn objRequiresNoun; 
			savWn=wn;
			give obj ~noun_ref;
			while(true){
				f=AdjOrNoun(obj);
				if(f==0) break; !--current word does NOT reference the given object
				if(f==1 or 2) { !--current word is a noun, and not an adjective
					n++;			!--increment the noun count
					give obj noun_ref; !--this object was definitely  referenced by a noun word
					self._atLeastOneObjectReferencedByNoun=true;
				}
				if(f==2) parser_action=##PLURALFOUND;
				if(f==3) a++; !--increment the adjective count
			}
			if(a+n ==0) {
				wn=savWn;
				return -1; !--we didnt find anything that matched. lets defer to another parsenoun routine...
			}
			
			objRequiresNoun=false;
			if(obj provides requireNoun) 
				objRequiresNoun = ValueOrRun(obj,requireNoun); !--is there an object level override?
			else{
				if(ValueOrRun(orAdjective,mode)==REQUIRE_NOUN) objRequiresNoun=true;
			}

			if(n==0){ !--there are no nouns specified
				if(objRequiresNoun) {
					return 0; 
				}
			}
		
			return a+n; !--we return the total words.  We've identified objects reference by nouns with the noun_ref attribute; adjudicate can select these from objects referenced only by adjectives
		]
	,	ext_adjudicate[
				!todo: this will eliminate objects only referenced by adjectives, if we've also referenced another object using noun words; however, we haven't solved to prefer an object referenced by 2 nouns over an object referenced by 1 noun and 1 adjective
				!todo: backdrops should be even less preferred than adjective-only references
				o i retval nRef aRef;
				!--count up objects referenced only by adjectives vs those referenced only by nouns
				for(i=0: i<number_matched:i++) {
					o = match_list-->i;

					if(o has noun_ref) {
						nRef++;
						retval=o; !we save this off, just in case we end up with only one
					}
					else{
						aRef++;
					}
				}

				if(nRef==0 || aRef==0) return false; !--we are dealing exclusively in either adjectives or noun referenced objects, so nothing for this code to adjudicate
				if(nRef==1) {
					no_infer_message=true;
					return retval; !--we are dealing with only one noun referenced object, so return that
				}
				!--we can't decide here.  We know that we have at least two noun referenced objects and at least one object referenced only adjectives, so lets eliminate the adjective-only references for further adjudication attempts
				for(i=number_matched-1: i>=0:i--) {
					o = match_list-->i;
					if(o hasnt noun_ref) util.orParser.removeMatchPos(i);
				}
				rfalse;
		]
	,	ext_chooseobjects[ obj c;
			if(c<2) return 0; !--let the parser decide
			if(obj has noun_ref) return 2; !--a noun word was actually used, give decent priority
			if(obj hasnt noun_ref) { !--objects referred to only by adjectives are not rated as highly
				if(self._atLeastOneObjectReferencedByNoun==true) util.orParser.excludeObjFromDisambiguation(obj); !--if this object is only referenced by an adjective, and at least one other object was referenced by a noun, then just kill this option entirely, so it wont appear in disambiguation lists later
				if(number_matched==1) return 1;  !-- specifically, if it was the only object matched, give it an average score
				if(number_matched>1) return -1; !--but if other objects were matched too, then give it a less than average score in case one of the others was referred to by a noun
			}
		]
	,	ext_identical[o1 o2 p;
		if(o1 provides adjective && o2 provides adjective){
			!-- here we determine if the objects define adjectives, and they  are not two identical sets.  If so, then we should be able
			!-- to uniquely distinguish the two objects, even if their nouns are the same so return false: they are not identical.
			for (p=0 : p<o1.#adjective/WORDSIZE : p++) { ! check if every adjective
				if (WordInProperty(o1.&adjective-->p,o2,adjective)==false) rfalse;
			}
			for (p=0 : p<o2.#adjective/WORDSIZE : p++) {
				if (WordInProperty(o2.&adjective-->p,o1,adjective)==false) rfalse;
			}
		}
		rtrue; !--seem identical from an adjectives perspective; let the default behavior try and sort this out
	];
	!--used to determine if the CurrentWord is an adjective or a noun (singular or plural).
	!--returns 1:Noun; 2:PluralNoun; 3: Adj; 0:Neither
	[AdjOrNoun obj wrd;
		wrd=NextWord();

		if(wrd~=0 && obj provides adjective && WordInProperty(wrd,obj,adjective)==true) return 3; !--adjective
		if(WordInProperty(wrd,obj,name)==true) {
			if(wrd~=0 && ((wrd->#dict_par1) & 4)~=0)
				return 2; !--plural noun
			else
				return 1; !--singular noun
		}
	#ifdef orRecogName_STAGE;
		if(MatchAgainstShortName(obj,wn-1)==true) return 1; !--singular noun
	#endif;
		return 0; !--None of the above
	];
#endif; !--AFTER_VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE