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
		with
			printNounVerb[pronounRoutine;
				self.printNoun(pronounRoutine);
				self.printVerb();
				rtrue;
			]
		,	printNoun[pronounRoutine;
				pronounRoutine(self.contextObject);
				if(self.objSpecifiedFirst==false) print " ",(name) self.contextObject;
				rtrue;
			]
		,	printVerb[pronounRoutine;
				if(pronounRoutine) pronounRoutine(self.contextObject);
				self.conjugate();				
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
				if(pat.equalsOneOf("theSelf", "theMyselfYourselfItself")) {theSelf(obj); rtrue;}
				if(pat.equalsOneOf("TheSelf", "TheMyselfYourselfItself")) {cTheSelf(obj); rtrue;}
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
				if(pat.equalsOneOf("iAm", "iAmYouAreItIs")) {
					print (iAm)obj;
					rtrue;
				}
				if(pat.equalsOneOf("IAm", "IAmYouAreItIs")) {
					print (CIAm)obj;
					rtrue;
				}
				if(pat.equalsOneOf("im", "imYoureIts")) {
					print (Im)obj;
					rtrue;
				}
				if(pat.equalsOneOf("Im", "ImYoureIts")) {
					print (CIm)obj;
					rtrue;
				}
				if(pat.equalsOneOf("imNot", "imNotYouArentItIsnt")) {
					print (ImNot)obj;
					rtrue;
				}
				if(pat.equalsOneOf("ImNot", "ImNotYouArentItIsnt")) {
					print (CImNot)obj;
					rtrue;
				}
				!qualifying pronouns.  Pronoun + Noun + verb.
				if(pat.equals("that")) return self.printNounVerb(__ThatOrThose);
				if(pat.equals("That")) return self.printNounVerb(__CThatOrThose);
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
	[ __ThatOrThose obj;
		if (obj has pluralname)	
			print "those"; 
		else
			print "that";
	];
	[ __CThatOrThose obj;
		if (obj has pluralname)	
			print "Those"; 
		else
			print "That";
	];
	[ ThisOrThese obj;
		if (obj has pluralname)	
			print "these"; 
		else
			print "this";
	];
	[ CThisOrThese obj;
		if (obj has pluralname)	
			print "These"; 
		else
			print "This";
	];
	[theSelf obj;  if(obj==actor) print (Myself)obj; else print (the)obj; ];
	[cTheSelf obj; if(obj==actor) print (CMyself)obj; else print (The)obj;]; 
	
#endif; !--After VERBLIB
!======================================================================================
! AFTER GRAMMAR
#iftrue (LIBRARY_STAGE == AFTER_GRAMMAR);

#endif;
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE

