!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.04.17 orPrintPatterns_English
! Inline printing using $notation.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License:
!--------------------------------------------------------------------------------------
! Revision History
! 2024.04.17	Initial Creation
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orPrintPatterns_English_STAGE  0 ;
!--------------------------------------------------------------------------------------
! INCLUDED DEPENDENCIES
#include "orPrint";
#include "_orPronoun_English";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orPrintPatterns_English_STAGE	; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orPrintPatterns_English_STAGE  	< LIBRARY_STAGE);
   #undef      orPrintPatterns_English_STAGE  	;
   Constant    orPrintPatterns_English_STAGE  	LIBRARY_STAGE;
   #ifdef      orPrintPatterns_English_STAGE  	; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orPrintPatterns_English..."; #endif;

!======================================================================================
! AFTER PARSER
#iftrue (LIBRARY_STAGE == AFTER_PARSER);
	#ifndef orPronoun_English_STAGE;
		Default orPronoun_English_STAGE BEFORE_PARSER;
		#include "_orPronoun_English";
	#endif;
#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
	[ orPrintConjugate ptrn isPerfectTense !--TODO: handle isPerfectTense
			didPrintIrregular obj vrb lookupForm;
		if(ptrn.parametersString.isEmpty()) return;
		
		print " ";
		if(printIrregularVerbConjugation(ptrn)) return;
		
		obj=ptrn.contextObject;
		vrb=ptrn.getParamEphemeral(0);
		if(isPerfectTense){
			lookupForm=ptrn.getParamEphemeral(3);
			if(lookupForm==0) lookupForm=ptrn.getParamEphemeral(2);
			if(lookupForm~=-1 && lookupForm.isEmpty()==false) return lookupForm.print();
			return vrb.append("ed").print();
		}
		!--past tense
		else if(player provides narrative_tense && player.narrative_tense == PAST_TENSE){
			lookupForm=ptrn.getParamEphemeral(2);
			if(lookupForm~=-1 && lookupForm.isEmpty()==false) return lookupForm.print();
			return vrb.append("ed").print();
		}
		else{ !--present tense
			!--plural
			if ((self.contextObject==player && player provides narrative_voice && player.narrative_voice == 3 && player hasnt pluralname) || !we are narrating the player's actions in 3rd person singular, or
					(self.contextObject~=player && self.contextObject hasnt pluralname)){ !we are conjugating a verb with a singular non-player object
				
				lookupForm=ptrn.getParamEphemeral(1);
				if(lookupForm~=-1 && lookupForm.isEmpty()==false) return lookupForm;

				vrb.lock();
				if(vrb.right(1).equals("s")) {
					vrb.free();
					return vrb.append("es").print();
				}
				else{
					vrb.free();
					return vrb.append("s").print();
				}
			}
			!--singular
			else{
				lookupForm=ptrn.getParamEphemeral(0);
				if(lookupForm~=-1 && lookupForm.isEmpty()==false) return lookupForm.print();

				vrb.print(); 
				return;

			}
		}
	 ];

	 orPrintPattern _oprEnglish
		with
			printNounVerb[pronounRoutine;
				self.printNoun(pronounRoutine);
				return self.printVerb();
			]
		,	printNoun[pronounRoutine;
				pronounRoutine(self.contextObject);
				if(self.objSpecifiedFirst==false) print " ",(name) self.contextObject;
				rtrue;
			]
		,	printVerb[pronounRoutine 
					vrb conj;
				if(pronounRoutine) pronounRoutine(self.contextObject);
				self.conjugate();

				! if((vrb=self.getParamEphemeral(0))==0) rtrue;
				! if(self.contextObject==0) rtrue;
				
				! vrb.lock();
				! print " "; getConjugationEphemeral(self.contextObject, vrb).print();
				! vrb.free();
				 
				! rtrue;
				
			]
		,	runRule[ pat obj; pat=self.patternName; obj=self.contextObject;

				if(obj<=0) rfalse; !everything below this requires a noun to be specified, even if it isn't printed...
				!pronoun only. No noun, no verb.
				if(pat.equalsOneOf("mine", "mineYoursIts")) {Mine(obj); rtrue;}
				if(pat.equalsOneOf("Mine", "MineYoursIts")) {CMine(obj); rtrue;}
				if(pat.equalsOneOf("me","meYouIt")) {Me(obj); rtrue;}
				if(pat.equalsOneOf("Me", "MeYouIt")) {CMe(obj); rtrue;}
				if(pat.equalsOneOf("self", "myselfYourselfItself")) {Myself(obj); rtrue;}
				if(pat.equalsOneOf("Self", "MyselfYourselfItself")) {CMyself(obj); rtrue;}

				!possessive pronouns only.  No noun, no verb. 
				if(pat.equalsOneOf("my","myYourIts")) { My(obj); rtrue; }
				if(pat.equalsOneOf("My","MyYourIts")) { CMy(obj); rtrue; }

				! if(pat.equalsOneOf("my","myYourIts")) return self.doNoun(My);
				! if(pat.equalsOneOf("My","MyYourIts")) return self.doNoun(CMy);

				!pronoun + verb, dont print the noun.
				if(pat.equalsOneOf("i","iYouIt","nom","subj")) {
					print (I)obj;
					self.conjugate();
					rtrue;
				}
				if(pat.equalsOneOf("I","IYouIt","Nom", "Subj")) {
					print (CI)obj;
					self.conjugate();
					rtrue;
				}

				!qualifying pronouns.  Pronoun + Noun + verb.
				if(pat.equals("that")) return self.printNounVerb(ThatOrThose);
				if(pat.equals("That")) return self.printNounVerb(CThatOrThose);
				if(pat.equals("this")) return self.printNounVerb(ThisOrThese);
				if(pat.equals("This")) return self.printNounVerb(CThisOrThese);


				! token = util.orStr.new();

				! token.set(self.getToken(0));
				! contextObject=self.tokenToObject(token); !try to resolve the first token as an object

				! if(contextObject){
				! 	suppressObj=true; !by convention, if we successfully resolved an object from token 1, then we dont print it.  In such form, the object is only used for conjugation. (e.g. "$actor:the" prints "the";  "$the:actor" prints as "the troll".)
				! 	token.set(self.getToken(1));
				! }
				! else{
				! 	token.set(self.getToken(1));
				! 	contextObject=self.tokenToObject(token); !try to resolve the second token as an object
				! 	token.set(self.getToken(0));
				! }
				! ;

				! if(contextObject==0) {
				! 	token.free();
				! 	rfalse; !at this point, we expect to have handled all non-object print rules; so somethings wrong, just silently fail.
				! }


				!token.free();
				!rtrue;

			];
	[ ThisOrThese obj;
		if (obj == player) {
		if (player provides narrative_voice) {
			if (player.narrative_voice == 1) { print "me"; return; }
			if (player.narrative_voice == 3) { CDefart(player); return; }
		}
		print "you";
		return;
		}
		if (obj has pluralname)       { print "these"; return; }
		if (obj has female)           { print "her"; return; }
		if (obj has male or animate)
			if (obj hasnt neuter)     { print "him"; return; }
		print "this";
	];
	[ CThisOrThese obj;
		if (obj == player) {
		if (player provides narrative_voice) {
			if (player.narrative_voice == 1) { print "I"; return; }
			if (player.narrative_voice == 3) { CDefart(player); return; }
		}
		print "You";
		return;
		}
		if (obj has pluralname)		{ print "These"; return; }
		if (obj has female)			{ print "She"; return; }
		if (obj has male or animate) {
			if (obj hasnt neuter)		{ print "He"; return; }
		}
		print "This";
	];
#endif; !--After VERBLIB
!======================================================================================
! AFTER GRAMMAR
#iftrue (LIBRARY_STAGE == AFTER_GRAMMAR);
	#ifndef printIrregularVerbConjugation;
	! Unless the author opts-in, we don't try to conjugate irregular verbs
	[ printIrregularVerbConjugation; return false;];
	#endif; 
#endif;
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE

