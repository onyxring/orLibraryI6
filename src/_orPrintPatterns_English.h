!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.04.17 orPrintPatterns_English
! Inline printing using $notation.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! Revision History
! 2024.04.17	Initial Creation
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
#ifndef        orPrintPatterns_English_STAGE;
default        orPrintPatterns_English_STAGE  0 ;
system_file;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES

!--------------------------------------------------------------------------------------
#endif;
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
		,	printVerbParamFromTense[verbword;
				if(getNarrativeTense() == PAST_TENSE)	
					verbword=self.getParamEphemeral(1); 
				else
					verbword=self.getParamEphemeral(0); 
				
				verbword.print();
				rtrue;				
			]
		,	runRule[ pat obj; pat=self.patternName; obj=self.contextObject;
				if(pat.equals("tense",true)) {
					self.printVerbParamFromTense();
					rtrue;
				}
				
				if(obj<=0) rfalse; !everything below this requires a noun to be specified, even if it isn't printed... 
				!-- Pronouns only. No conjugation -------------------------------------
				if(pat.equalsOneOf("mine", "mineYoursItsTheirs",true)) return self.chooseCap(obj,Mine, CMine);
				if(pat.equalsOneOf("me","meYouItThem",true)) return self.chooseCap(obj,Me, CMe);
				if(pat.equalsOneOf("self", "myselfYourselfItselfThemselves",true)) return self.chooseCap(obj,Myself, CMyself);
				if(pat.equalsOneOf("theSelf", "theMyselfYourselfItselfThemselves",true)) return self.chooseCap(obj,theSelf, cTheSelf);
				if(pat.equalsOneOf("my","myYourItsTheir",true)) return self.chooseCap(obj,my, cMy);

				!-- Pronoun + "verbs of being" (Object suppression syntax should also work to suppress the pronoun)
				if(pat.equalsOneOf("iAm", "iAmYouAreItIsTheyAre",true)) {
					if(self.objSpecifiedFirst)
						return self.chooseCap(obj,am, cAm);
					else
						return self.chooseCap(obj,iAm, cIAm);
				}
				
				!-- Word + "verbs of being" (Object suppression syntax should also work to suppress the pronoun)
				if(pat.equalsOneOf("theIs", "theIsTheyAre",true)) {
					if(self.objSpecifiedFirst==false) !--if suppress object syntax
						return self.chooseCap(obj,theIAm, cTheIAm);
					else
						return self.chooseCap(obj,am, cAm);
				}
				
				if(pat.equalsOneOf("theIsnt", "theIsTheyArent",true)) {
					if(self.objSpecifiedFirst==false) !--if suppress object syntax
						return self.chooseCap(obj,theIsnt, cTheIsnt);
					else
						return self.chooseCap(obj,am, cAm);
				}
				
				!-- Pronoun + "verbs of being" contractions (Object suppression does NOT make sense)
				if(pat.equalsOneOf("im", "imYoureItsTheyre",true)) return self.chooseCap(obj,im, cIm);
				if(pat.equalsOneOf("imNot", "imNotYouArentItIsntTheyArent",true)) return self.chooseCap(obj,imNot, cImNot);
				
				!-- Pronouns + verb (if specified) -------------------------------------
				if(pat.equalsOneOf("i","iYouItThey",true)) {
					self.chooseCap(obj,i, cI);
					if(self.objSpecifiedFirst==false) self.conjugate();
					rtrue;
				}
				
				!-- Qualifying pronoun + noun + verb (if specified). Object suppression syntax should also work with these.
				if(pat.equals("that",true)) {
					self.chooseCap(obj,ThatOrThose, cThatOrThose);
					if(self.objSpecifiedFirst==false) {
						print " ",(name)obj;
						self.conjugate();
					}

					rtrue;
				}
				if(pat.equals("this",true)) {
					self.chooseCap(obj,ThisOrThese, cThisOrThese);
					if(self.objSpecifiedFirst==false) {
						print " ",(name)obj;
						self.conjugate();
					}

					rtrue;
				}
				
				! !--$actor:i(am)...
				! if(pat.equalsOneOf("am", "amAreIs")) {
				! 	print (am)obj;
				! 	rtrue;
				! }
				! if(pat.equalsOneOf("Am", "AmAreIs")) {
				! 	print (CAm)obj;
				! 	rtrue;
				! }
				
				
				
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
	! [ __ThatOrThose obj;
	! 	if (obj has pluralname)	
	! 		print "those"; 
	! 	else
	! 		print "that";
	! ];
	! [ __CThatOrThose obj;
	! 	if (obj has pluralname)	
	! 		print "Those"; 
	! 	else
	! 		print "That";
	! ];
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

