!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.04.02 ORPronoun_English
! This is a collection of pronoun print rules to handle just about every imaginable
! scenerio.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! 2025 Note: orPronoun is included primarily for compatibility purposes.  Since this
! was first rolled out, the Standard Library has implemented its own version of 
! pronoun helpers, with differen names and signatures. I've keep this extension for now
! because it is a dependency of other orLib extensions.  In the future, I may rework those
! references to eliminate this one entirely.
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
! 2004 original description:
!
! Things to remember about this implementation.
! Functions, such as civerb() and iverb() return a value after they have
! printed.  When printing these the (nop) printrule should be used.
!
! PrintRules:
!
!	I, Me, My, Mine, Myself, CI, CMe, CMy, CMine, CMyself,
!	TheI, TheMe, TheMy, TheMine, TheMyself, CTheI, CTheMe, CTheMy, CTheMine, CTheMyself,
!	IAm, CIAm
!
! Functions
!
!	IVerb, CIVerb
!
!--------------------------------------------------------------------------------------
! Revision History
! 2001.05.05	Initial Creation
! [snipped history from original ORLibrary]
! 2024.04.02	Updated the extension stucture to use features of the the 6/12 library
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orPronoun_English_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDED DEPENDENCIES

!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orPronoun_English_STAGE	; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orPronoun_English_STAGE  < LIBRARY_STAGE);
   #undef      orPronoun_English_STAGE  ;
   Constant    orPronoun_English_STAGE  LIBRARY_STAGE;
   #ifdef      orPronoun_English_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orPronoun_English..."; #endif;
!======================================================================================
! BEFORE PARSER
! #iftrue (LIBRARY_STAGE == BEFORE_PARSER);
! 	system_file;
! 	Constant SINGULAR_OBJ 0;
! 	Constant PLURAL_OBJ -1;
! 	!replace PSN__;
! #endif; !-- Before Parser
!======================================================================================
! AFTER PARSER
#iftrue (LIBRARY_STAGE == AFTER_PARSER);

	system_file;
	Constant SINGULAR_OBJ 0;
	Constant PLURAL_OBJ -1;
	!replace PSN__;

	Global MPronounObj;
	Global FPronounObj;
	Global NPronounObj;

	!----------------------------------------------------------------------
	! TODO: change these to support the library's version of these.
	!----------------------------------------------------------------------
	Default FIRST_PERSON 1;
	Default SECOND_PERSON 2;
	Default THIRD_PERSON 3;
	Default PAST_TENSE 1;
	!Default PRESENT_TENSE 2;
	Default PRESENT_TENSE 0; !Standard Library introduced this constant with a different value...
	Default FUTURE_TENSE 3;

	[getNarrativeTense; 
		if(player provides narrative_tense) 
			return player.narrative_tense;
		return PRESENT_TENSE;
	];
	[getNarrativePerson; 
		if(player provides narrative_voice) 
			return player.narrative_voice;
		return SECOND_PERSON;
	];

	! [ PSN__ o;
	! 	if (o==0) { print (string) NOTHING__TX; rtrue; }
	! 	switch(metaclass(o))
	! 	{   Routine: print "<routine ", o, ">"; rtrue;
	! 		String:  print "<string ~", (string) o, "~>"; rtrue;
	! 		nothing: print "<illegal object number ", o, ">"; rtrue;
	! 	}
	! 	if (o==player && NarrativePerson~=THIRD_PERSON) { print (string) YOURSELF__TX; rtrue; }
	! 	#ifdef LanguagePrintShortName;
	! 	if (LanguagePrintShortName(o)) rtrue;
	! 	#endif;
	! 	if (indef_mode && o.&short_name_indef~=0
	! 		&& PrintOrRun(o, short_name_indef, 1)~=0) rtrue;
	! 	if (o.&short_name~=0 && PrintOrRun(o,short_name,1)~=0) rtrue;
	! 	print (object)o;
	! ];
	system_file;
	  !------------------------------------------------------------------------
    ! ppf (past, present, future tense options)
    [ppf past present future;
        switch(getNarrativeTense())
        {
            PAST_TENSE: print (string) past;
            PRESENT_TENSE: print (string) present;
            FUTURE_TENSE:print (string) future;
        }
    ];
    !------------------------------------------------------------------------
    ! fst (First,Second,Third Person options)
    [fst first second third;
        switch(getNarrativePerson())
        {
            FIRST_PERSON:    print (string) first;
            SECOND_PERSON:    print (string) second;
            THIRD_PERSON:    print (string) third;
        }
    ];
	!----------------------------------------------------------------------
	! Special formating print routines go here
	!----------------------------------------------------------------------
	!----------------------------------------------------------------------
	! All common code in the pronoun print routines is contined here.
	! These are broken up into two routines because a single routine would
	! have too parameters for the Z-Machine.
	!----------------------------------------------------------------------
	[CommonPronounCode12 obj i we you allofyou;
		if(obj==player && getNarrativePerson()~=THIRD_PERSON) 
		{
			if(getNarrativePerson()==FIRST_PERSON) {
				if(obj hasnt pluralname) print(string)i;
				else print (string)we;
			}
			else { !--must be SECOND_PERSON then
				if(obj hasnt pluralname) print (string)you;
				else print (string)allofyou;
			}
			return true;
		}
		return false; !otherwise return false so that the other handler can be called
	];
	[CommonPronounCode3 obj they it she he;
		if(obj~=player || (obj==player && getNarrativePerson()==THIRD_PERSON))
		{
			if(obj has pluralname)
				print (string)they;
			else{ ! is singular
				if (obj has neuter || obj hasnt animate)
					print (string)it;
				else{
					if (obj has female)  print (string)she;
					else print (string)he;
				}
			}
			return true;
		}
		return false;
	];
	!----------------------------------------------------------
	! lowercase versions of the five sets of pronouns.
	!----------------------------------------------------------
	[I obj; if(CommonPronounCode12(obj, "I", "we","you","you")==false) CommonPronounCode3(obj,"they","it","she","he");];
	[Me obj; if(CommonPronounCode12(obj, "me", "us","you","you")==false) CommonPronounCode3(obj, "them","it","her","him");];
	[My obj; if(CommonPronounCode12(obj, "my","our","your","your")==false) CommonPronounCode3(obj,"their","its","her","his");];
	[Mine obj; if(CommonPronounCode12(obj, "mine", "ours","yours","yours")==false) CommonPronounCode3(obj,"theirs","its","hers","his");];
	[Myself obj; if(CommonPronounCode12(obj, "myself", "ourselves","yourself","yourselves")==false) CommonPronounCode3(obj,"themselves","itself","herself","himself");];
	!----------------------------------------------------------
	! UPPERCASE versions of the five sets of pronouns
	!----------------------------------------------------------
	[CI obj; if(CommonPronounCode12(obj, "I", "We","You","You")==false) CommonPronounCode3(obj,"They","It","She","He");];
	[CMe obj; if(CommonPronounCode12(obj, "Me", "Us","You","You")==false) CommonPronounCode3(obj, "Them","It","Her","Him");];
	[CMy obj; if(CommonPronounCode12(obj, "My","Our","Your","Your")==false) CommonPronounCode3(obj,"Their","Its","Her","His");];
	[CMine obj; if(CommonPronounCode12(obj, "Mine", "Ours","Yours","Yours")==false) CommonPronounCode3(obj,"Theirs","Its","Hers","His");];
	[CMyself obj; if(CommonPronounCode12(obj, "Myself", "Ourselves","Yourself","Yourselves")==false) CommonPronounCode3(obj,"Themselves","Itself","Herself","Himself");];
	!----------------------------------------------------------
	! All common code in the "The or pronoun" style routines
	!----------------------------------------------------------
	[CommonTheORPronounCode obj cap checkobj;
		if(obj hasnt animate || obj has neuter) !first locate the appropriate variable to test
			checkobj=NPronounObj;
		else
		{
			if(obj has female)
				checkobj=FPronounObj;
			else
				checkobj=MPronounObj;
		}
		if(obj==checkobj || (obj==player && getNarrativePerson()~=THIRD_PERSON))  return false; !--if they are the same, then return false, nothing output
		if(cap)
			print (The)obj;
		else
			print (the)obj;
		if(checkobj==NULL && (obj~=player || getNarrativePerson()==THIRD_PERSON)) !save the object for a later test
		{
			if(obj hasnt animate || obj has neuter)
				NPronounObj=obj;
			else
			{
				if(obj has female) FPronounObj=obj;
				else MPronounObj=obj;
			}
		}
		return true; !printed the name
	];
	!----------------------------------------------------------
	! Reset the pronoun affinity
	!----------------------------------------------------------
	[ClearPronoun;
		MPronounObj=FPronounObj=NPronounObj=null;
	];
	!----------------------------------------------------------
	! lowercase versions of the five sets of pronouns
	!----------------------------------------------------------
	[TheI obj; if(CommonTheORPronounCode(obj,false)==false) print (I)obj;];
	[TheMe obj; if(CommonTheORPronounCode(obj,false)==false) print (Me)obj;];
	[TheMy obj; if(CommonTheORPronounCode(obj,false)==false) print (My)obj; else print "'s";];
	[TheMine obj; if(CommonTheORPronounCode(obj,false)==false) print (Mine)obj; else print "'s";];
	[TheMyself obj; if(CommonTheORPronounCode(obj,false)==false) print (Myself)obj;];
	
	!----------------------------------------------------------
	! UPPERCASE versions of the five sets of pronouns
	!----------------------------------------------------------
	[CTheI obj; if(CommonTheORPronounCode(obj,true)==false) print (CI)obj;];
	[CTheMe obj; if(CommonTheORPronounCode(obj,true)==false) print (CMe)obj;];
	[CTheMy obj; if(CommonTheORPronounCode(obj,true)==false) print (CMy)obj; else print "'s";];
	[CTheMine obj; if(CommonTheORPronounCode(obj,true)==false) print (CMine)obj; else print "'s";];
	[CTheMyself obj; if(CommonTheORPronounCode(obj,true)==false) print (CMyself)obj;];
	!----------------------------------------------------------
	! A couple of routines to address the change in verb form
	!	when dealing with singular nouns in the third-person,
	!	present tense.
	!----------------------------------------------------------
	[Vrb obj pastverb vrb altverb futverb; !if no object, then obj can equal SINGULAR_OBJ or PLURAL_OBJ
		!-- if(3rdPerson/PresentTense/Singular) then "runs" else "run"
		if(getNarrativeTense()==PRESENT_TENSE && (obj~=player || getNarrativePerson()==THIRD_PERSON) && (obj==SINGULAR_OBJ || (metaclass(obj)==object && obj hasnt pluralname))){
			if(altverb) print (string)altverb;
			else print (string)vrb,"s";
		}
		else {
			switch(getNarrativeTense()){
				PAST_TENSE: print (string) pastverb;
				PRESENT_TENSE: print(string)vrb;
				FUTURE_TENSE: if(futverb)print_ret (string)futverb;
							else print "will ",(string)vrb;
			}
		}
	];
	[CommonVerbAdjustment cap obj pastverb verb altverb futverb forcename;
		if(forcename){
			if(cap) print (The) obj," ";
			else  print (the) obj," ";
		}
		else{
			if(cap) print (CTheI) obj," ";
			else  print (TheI) obj," ";
		}
		Vrb(obj,pastverb,verb,altverb,futverb);
	];
	[IVerb obj pastverb verb altverb futverb; CommonVerbAdjustment(false,obj,pastverb, verb,altverb, futverb);];
	[CIVerb obj pastverb verb altverb futverb; CommonVerbAdjustment(true,obj,pastverb, verb,altverb, futverb);];
	[TheVerb obj pastverb verb altverb futverb; CommonVerbAdjustment(false,obj,pastverb, verb,altverb, futverb,true);];
	[CTheVerb obj pastverb verb altverb futverb; CommonVerbAdjustment(true,obj,pastverb, verb,altverb, futverb,true);];
	!----------------------------------------------------------
	! Print the appropriate "verb of being"
	!----------------------------------------------------------
	! pass false for generic singular object or true for generic plural object, otherwise pass in the object itself
	[Am obj; !was/am/will/were/are/is
		
		if(obj==player && getNarrativePerson()~=THIRD_PERSON) !first or second person
		{
			switch(getNarrativePerson())
			{
			FIRST_PERSON: print (nop) ppf("was","am","will");
			SECOND_PERSON: print (nop) ppf("were","are","will");
			}
			return;
		}
		if(obj~=null && (obj == true || obj has pluralname))
			print (nop) ppf("were","are","will");
		else
			print (nop) ppf("was","is","will");
	];
	[TheIAm obj; print (TheI)obj," ",(Am)obj;];
	[CTheIAm obj; print (CTheI)obj," ",(Am)obj;];
	[IAm obj; print (I)obj," ",(Am)obj;];
	[CIAm obj; print (CI)obj," ",(Am)obj;];
	
	[Im obj; if(CommonPronounCode12(obj, "I'm", "we're","you're","you're")==false) CommonPronounCode3(obj,"they're","it's","she's","he's");];
	[CIm obj; if(CommonPronounCode12(obj, "I'm", "We're","You're","You're")==false) CommonPronounCode3(obj,"They're","It's","She's","He's");];
	[ImNot obj; if(CommonPronounCode12(obj, "I'm not", "we aren't","you aren't","you aren't")==false) CommonPronounCode3(obj,"they aren't","it isn't","she isn't","he isn't");];
	[CImNot obj; if(CommonPronounCode12(obj, "I'm not", "We aren't","You aren't","You aren't")==false) CommonPronounCode3(obj,"They aren't","It isn't","She isn't","He isn't");];
	
	[Have obj; !had/have/will have
		!--for present tense always use "have" except when speaking in singular third person
		if(obj hasnt pluralname && (obj~=player || (obj==player && getNarrativePerson()==THIRD_PERSON))) {
			print (nop) ppf("had","has","will have had");
			return;
		}
		print (nop) ppf("had","have","will have had");
	];
	[IHave obj; print (TheI)obj," ",(Have)obj;];
	[CIHave obj; print (CTheI)obj," ",(Have)obj;];
	[ICan obj; IVerb(obj,"could","can","can");];
	[CICan obj; CIVerb(obj,"could","can","can");];
	[ICant obj; IVerb(obj,"could not","cannot","cannot");];
	[CICant obj; CIVerb(obj,"could not","cannot","cannot");];
#endif; !--AFTER_VERBLIB
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE