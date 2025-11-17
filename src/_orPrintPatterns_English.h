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
	 orPrintPattern _oprEnglish
		with doNounVerb[pronounRoutine;
				self.doNoun(pronounRoutine);
				return self.doVerb();
			]
		,	doNoun[pronounRoutine;
				pronounRoutine(self.contextObject);
				if(self.objSpecifiedFirst==false) print " ",(name) self.contextObject;
				rtrue;
			]
		,	doVerb[pronounRoutine;
				if(pronounRoutine) pronounRoutine(self.contextObject);
				!contextObject
				rtrue;
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

				!possessive pronouns.  Pronoun + noun, no verb.
				if(pat.equalsOneOf("my","myYourIts")) return self.doNoun(My);
				if(pat.equalsOneOf("My","MyYourIts")) return self.doNoun(CMy);

				!pronoun + verb, dont print the noun.
				if(pat.equalsOneOf("i","iYouIt","nom","subj")) return self.doVerb(I);
				if(pat.equalsOneOf("I","IYouIt","Nom", "Subj")) return self.doVerb(CI);

				!qualifying pronouns.  Pronoun + Noun + verb.
				if(pat.equals("that")) return self.doNounVerb(ThatOrThose);
				if(pat.equals("That")) return self.doNounVerb(CThatOrThose);
				if(pat.equals("this")) return self.doNounVerb(ThisOrThese);
				if(pat.equals("This")) return self.doNounVerb(CThisOrThese);


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
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE

