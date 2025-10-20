!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.03.30 ORDistinctMeSelf
! Makes the subtle distinction between 'self' and 'me', having them
! equate to 'actor' and 'player' repectively.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! This extension is most useful when ordering NPC's around.  The default functionality
! of the standard library is to equate all "me" words with the player.  This results
! in the following two orders being the same:
!
!		"Bob, pour coffee on me"
!		"Bob, pour coffee on self"
!
! Both forms would normally result in an order for Bob to pour coffee on the player.
! This module will make the appropriate distinction where the second form will instruct
! "Bob" to pour coffee on himself.  Additionally, it adds the word "yourself" as a "me"
! word that equals the actor.
!--------------------------------------------------------------------------------------
! Revision History
! 2003.01.03	Initial Creation
! ...			History from orLibrary v1 snipped away.
! 2024.03.30	Refactored for the orLibrary version 2.
!TODO: tie this into a Hook
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orDistinctMeSelf_STAGE  0;
!======================================================================================
! INCLUDE DEPENDENCIES
#include "_orHookParseToken";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orDistinctMeSelf_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orDistinctMeSelf_STAGE  < LIBRARY_STAGE);
   #undef      orDistinctMeSelf_STAGE  ;
   Constant    orDistinctMeSelf_STAGE  LIBRARY_STAGE;
   #ifdef      orDistinctMeSelf_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orDistinctMeSelf..."; #endif;
!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);
	replace PT_HandleMeWords__;
	replace TryGivenObject;
	Constant ME4__WD 'yourself';
#endif; !--Before parser
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);

	[PT_HandleMeWords__ o;
		if (o==ME1__WD or ME2__WD or ME3__WD or ME4__WD){
			pronoun__word = pronoun_word = o;
			if (o==ME1__WD or ME2__WD) pronoun__obj = pronoun_obj = player;
			else pronoun__obj =pronoun_obj = actor;
		}
	];

	[ TryGivenObject obj threshold k w j z;
 	#Ifdef DEBUG;
    if (parser_trace >= 5) print "    Trying ", (the) obj, " (", obj, ") at word ", wn, "^";
    #Endif; ! DEBUG

    dict_flags_of_noun = 0;

    !  If input has run out then always match, with only quality 0 (this saves
    !  time).

    if (wn > num_words) {
        if (indef_mode ~= 0)
            dict_flags_of_noun = DICT_X654;  ! Reject "plural" bit
        MakeMatch(obj,0);
        #Ifdef DEBUG;
        if (parser_trace >= 5) print "    Matched (0)^";
        #Endif; ! DEBUG
        return 1;
    }

    !  Ask the object to parse itself if necessary, sitting up and taking notice
    !  if it says the plural was used:

    if (obj.parse_name~=0) {
        parser_action = NULL; j=wn;
        k = RunRoutines(obj,parse_name);
        if (k > 0) {
            wn=j+k;

          .MMbyPN;

            if (parser_action == ##PluralFound)
                dict_flags_of_noun = dict_flags_of_noun | DICT_PLUR;

            if (dict_flags_of_noun & DICT_PLUR) {
                if (~~allow_plurals) k = 0;
                else {
                    if (indef_mode == 0) {
                        indef_mode = 1; indef_type = 0; indef_wanted = 0;
                    }
                    indef_type = indef_type | PLURAL_BIT;
                    if (indef_wanted == 0) indef_wanted = 100;
                }
            }

            #Ifdef DEBUG;
            if (parser_trace >= 5) print "    Matched (", k, ")^";
            #Endif; ! DEBUG
            MakeMatch(obj,k);
            return k;
        }
        if (k == 0) jump NoWordsMatch;
        wn = j;
    }

    ! The default algorithm is simply to count up how many words pass the
    ! Refers test:

    parser_action = NULL;

    w = NounWord();
!-----------------modified code to support actor/player differences (Begin)--------------
	wn=wn-1;
	z=NextWord();
	if (w==1 || z==ME4__WD){ !--if a me word
			if(player==actor && obj==player) { k=1; jump MMbyPN; } !--if player and actor are the same then all "me" words will work
			if(z==ME1__WD or ME2__WD){
				if(obj==player) { k=1; jump MMbyPN; } !--me and myself reference player
			}
			else{
				if(obj==actor) { k=1; jump MMbyPN; } !--self and yourself reference actor
			}
	}
!-----------------modified code to support actor/player differences (END)  --------------

	if (w >= 2 && w < 128 && (LanguagePronouns-->w == obj)) { k = 1; jump MMbyPN; }

    j = --wn;
    threshold = ParseNoun(obj);
    if (threshold == -1) {
        LibraryExtensions.ext_number_1 = wn;    ! Set the "between calls" functionality to
        LibraryExtensions.BetweenCalls = LibraryExtensions.RestoreWN;
        threshold = LibraryExtensions.RunWhile(ext_parsenoun, -1, obj);
        LibraryExtensions.BetweenCalls = 0;     ! Turn off the "between calls" functionality
    }
    #Ifdef DEBUG;
    if (threshold >= 0 && parser_trace >= 5) print "    ParseNoun returned ", threshold, "^";
    #Endif; ! DEBUG
    ! Don't arbitrarily increase wn when ParseNoun() returns -1
    if (threshold > 0) {
	k = threshold;
	wn = j + k;
	jump MMbyPN;
    }
    ! Check wn instead of wn - 1
    if (threshold == 0 || Refers(obj,wn) == 0) {
	.NoWordsMatch;
	if (indef_mode ~= 0) {
		! Restore wn to pre-ParseNoun() state
		k = 0; parser_action = NULL; wn = j;
		jump MMbyPN;
	}
	rfalse;
    }

    if (threshold < 0) {
	! Set threshold to reflect any words consumed by ParseNoun()
	threshold = wn - j;
	w = NextWord();	! Ensure w contains actual first word of noun phrase
			! if ParseNoun() moved wn.
	dict_flags_of_noun = (w->#dict_par1) & (DICT_X654+DICT_PLUR);!$$01110100;
        while (Refers(obj, wn-1)) {
            threshold++;
            if (w)
               dict_flags_of_noun = dict_flags_of_noun | ((w->#dict_par1) & (DICT_X654+DICT_PLUR));
            w = NextWord();
        }
    }

    k = threshold;
    jump MMbyPN;
	];

#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE