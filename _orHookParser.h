!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.10.16 orHookParser
! Adds additional hooks for use by the standard library's LibraryExtensions object.
! No default behavior has been change beyond allowing extensions to be defined.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! This extension, classified as a "hook", provides tooling for other extensions to
! use and provides no specific features by itself.
!--------------------------------------------------------------------------------------
! Revision History
! 2024.10.16	Initial Creation
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orHookParser_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES

!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orHookParser_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orHookParser_STAGE  < LIBRARY_STAGE);
   #undef      orHookParser_STAGE  ;
   Constant    orHookParser_STAGE  LIBRARY_STAGE;
   #ifdef      orHookParser_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orHookParser..."; #endif;
!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);
replace Adjudicate; ! _oldAdjudicate;
replace Parser__parse;
property individual ext_suppressParsingImplicitTake;
property individual ext_forceLookAhead;
property individual ext_adjudicate;
#endif;
!======================================================================================
! AFTER PARSER
#iftrue (LIBRARY_STAGE == AFTER_PARSER);
    !--TODO: add additonal hooks.  Also, there's a bug fix to the standard library which should be made optional until/unless it is included in the standard library.
    [ Adjudicate context i j k good_flag good_ones last n flag offset sovert;

		i  = LibraryExtensions.RunWhile(ext_adjudicate, false, context);
        if(i~=false) return i;

    #Ifdef DEBUG;
    if (parser_trace >= 4) {
        print "   [Adjudicating match list of size ", number_matched, " in context ", context, "]^";
        print "   ";
        if (indef_mode) {
            print "indefinite type: ";
            if (indef_type & OTHER_BIT)  print "other ";
            if (indef_type & MY_BIT)     print "my ";
            if (indef_type & THAT_BIT)   print "that ";
            if (indef_type & PLURAL_BIT) print "plural ";
            if (indef_type & LIT_BIT)    print "lit ";
            if (indef_type & UNLIT_BIT)  print "unlit ";
            if (indef_owner ~= 0) print "owner:", (name) indef_owner;
            new_line;
            print "   number wanted: ";
            if (indef_wanted == 100) print "all"; else print indef_wanted;
            new_line;
            print "   most likely GNAs of names: ", indef_cases, "^";
        }
        else print "definite object^";
    }
    #Endif; ! DEBUG

    j = number_matched-1; good_ones = 0; last = match_list-->0;
    for (i=0 : i<=j : i++) {
        n = match_list-->i;
        match_scores-->i = 0;

        good_flag = false;

        switch (context) {
          HELD_TOKEN, MULTIHELD_TOKEN:
            if (parent(n) == actor) good_flag = true;
          MULTIEXCEPT_TOKEN:
            if (advance_warning == -1) {
                good_flag = true;
            }
            else {
                if (n ~= advance_warning) good_flag = true;
            }
          MULTIINSIDE_TOKEN:
            if (advance_warning == -1) {
                if (parent(n) ~= actor) good_flag = true;
            }
            else {
                if (n in advance_warning) good_flag = true;
            }
          CREATURE_TOKEN:
            if (CreatureTest(n) == 1) good_flag = true;
          default:
            good_flag = true;
        }

        if (good_flag) {
            match_scores-->i = SCORE__IFGOOD;
            good_ones++; last = n;
        }
    }
    if (good_ones == 1) return last;

    ! If there is ambiguity about what was typed, but it definitely wasn't
    ! animate as required, then return anything; higher up in the parser
    ! a suitable error will be given.  (This prevents a question being asked.)

    if (context == CREATURE_TOKEN && good_ones == 0) return match_list-->0;

    if (indef_mode == 0) indef_type=0;

    ScoreMatchL(context);
    if (number_matched == 0) return -1;

    if (indef_mode == 1 && indef_type & PLURAL_BIT ~= 0) {
        if (context ~= MULTI_TOKEN or MULTIHELD_TOKEN or MULTIEXCEPT_TOKEN
                     or MULTIINSIDE_TOKEN) {
            etype = MULTI_PE;
            return -1;
        }
        i = 0; offset = multiple_object-->0; sovert = -1;
        for (j=BestGuess() : j~=-1 && i<indef_wanted && i+offset<63 : j=BestGuess()) {
            flag = 1;

            !--jimf: I believe the standard library code below has a bug, which surfaces sporadically.  In short, it takes a best guess
            !       calculation and uses it as a watermark to eliminate items in the list; however, it does this before eliminating
            !       some obvious candidates.  This means the watermark can be calculated artificially high from, say, its consideration of scenery
            !       thus eliminating objects which are possible candidates but scored significantly lower than object which should have been eliminated


            !if (j has concealed or worn) flag = 0;
            ! if (sovert == -1) sovert = bestguess_score/SCORE__DIVISOR;
            ! else {
            !     if (indef_wanted == 100 && bestguess_score/SCORE__DIVISOR < sovert)
            !         flag = 0;
            ! }
            ! if (context == MULTIHELD_TOKEN or MULTIEXCEPT_TOKEN && parent(j) ~= actor)
            !     flag = 0;
            ! #Ifdef TRADITIONAL_TAKE_ALL;
            ! if (action_to_be == ##Take or ##Remove && parent(j) == actor)
            !     flag = 0;
            ! #Ifnot;
            ! if (action_to_be == ##Take or ##Remove &&
            !    (j has animate or scenery or static || parent(j) == actor))
            !     flag = 0;
            ! #Endif; ! TRADITIONAL_TAKE_ALL
            ! #Ifdef NO_TAKE_ALL;
            ! if (take_all_rule == 2 && match_length == 0) flag = 0;
            ! #Endif; ! NO_TAKE_ALL


            !--first eliminate some obviously wrong things...
            if (j has concealed or worn) flag = 0;
            if (context == MULTIHELD_TOKEN or MULTIEXCEPT_TOKEN && parent(j) ~= actor) flag = 0;

            #Ifdef TRADITIONAL_TAKE_ALL;
                if (action_to_be == ##Take or ##Remove && parent(j) == actor) flag = 0;
            #Ifnot;
                if (action_to_be == ##Take or ##Remove && (j has animate or scenery or static || parent(j) == actor)) flag = 0;
            #Endif; ! TRADITIONAL_TAKE_ALL

            #Ifdef NO_TAKE_ALL;
                if (take_all_rule == 2 && match_length == 0) flag = 0;
            #Endif; ! NO_TAKE_ALL

            if(flag){ !--don't establish the sovert watermark if are are not considering this object
                if(sovert == -1)
                    sovert = bestguess_score/SCORE__DIVISOR;
                else {
                    if (indef_wanted == 100 && bestguess_score/SCORE__DIVISOR < sovert)
                        flag = 0;
                }
            }
            !--end of jimf's change
            n = ChooseObjects(j, flag);
            if (n == 0) n = LibraryExtensions.RunWhile(ext_chooseobjects, 0, j, flag);
            switch (n) {
              2: flag = 0;  ! forcing rejection
              1: flag = 1;  ! forcing acceptance
             !0:            ! going with parser's decision
            }
            if (flag == 1) {
                i++; multiple_object-->(i+offset) = j;
                #Ifdef DEBUG;
                if (parser_trace >= 4) print "   Accepting it^";
                #Endif; ! DEBUG
            }
            else {
                i = i;
                #Ifdef DEBUG;
                if (parser_trace >= 4) print "   Rejecting it^";
                #Endif; ! DEBUG
            }
        }
        if (i < indef_wanted && indef_wanted < 100) {
            etype = TOOFEW_PE; multi_wanted = indef_wanted;
            multi_had=i;
            return -1;
        }
        multiple_object-->0 = i+offset;
        multi_context = context;
        #Ifdef DEBUG;
        if (parser_trace >= 4)
            print "   Made multiple object of size ", i, "]^";
        #Endif; ! DEBUG
        return 1;
    }

    for (i=0 : i<number_matched : i++) match_classes-->i = 0;

    n = 1;
    for (i=0 : i<number_matched : i++)
        if (match_classes-->i == 0) {
            match_classes-->i = n++; flag = 0;
            for (j=i+1 : j<number_matched : j++)
                if (match_classes-->j == 0 && Identical(match_list-->i, match_list-->j) == 1) {
                    flag=1;
                    match_classes-->j = match_classes-->i;
                }
            if (flag == 1) match_classes-->i = 1-n;
        }
     n--; number_of_classes = n;

    #Ifdef DEBUG;
    if (parser_trace >= 4) {
        print "   Grouped into ", n, " possibilities by name:^";
        for (i=0 : i<number_matched : i++)
            if (match_classes-->i > 0)
                print "   ", (The) match_list-->i, " (", match_list-->i, ")  ---  group ",
                  match_classes-->i, "^";
    }
    #Endif; ! DEBUG
    if (n == 1) dont_infer = true;

    if (indef_mode == 0) {
        !  Is there now a single highest-scoring object?
        i = SingleBestGuess();
        if (i >= 0) {

            #Ifdef DEBUG;
            if (parser_trace >= 4) print "   Single best-scoring object returned.]^";
            #Endif; ! DEBUG
            return i;
        }
    }

    if (indef_mode == 0) {
        if (n > 1) {
            k = -1;
            for (i=0 : i<number_matched : i++) {
                if (match_scores-->i > k) {
                    k = match_scores-->i;
                    j = match_classes-->i; j = j*j;
                    flag = 0;
                }
                else
                    if (match_scores-->i == k) {
                        if ((match_classes-->i) * (match_classes-->i) ~= j)
                            flag = 1;
                    }
            }

        if (flag) {
	    if(match_length == 1) {
		k = 0;
		for (i=1 : i<=number_of_classes : i++) {
		    while ((match_classes-->k) ~= i or -i) k++;
		    j = match_list-->k;

		    if(MatchWord(j, match_from)) {
			no_infer_message = true;
			return j;
		    }
		}
	    }
            #Ifdef DEBUG;
            if (parser_trace >= 4) print "   Unable to choose best group, so ask player.]^";
            #Endif; ! DEBUG
            return 0;
        }
        #Ifdef DEBUG;
        if (parser_trace >= 4) print "   Best choices are all from the same group.^";
        #Endif; ! DEBUG
        }
    }

    !  When the player is really vague, or there's a single collection of
    !  indistinguishable objects to choose from, choose the one the player
    !  most recently acquired, or if the player has none of them, then
    !  the one most recently put where it is.

    return BestGuess();

    ]; ! Adjudicate
! ----------------------------------------------------------------------------
!   To simplify the picture a little, a rough map of the main routine:
!
!   (A) Get the input, do "oops" and "again"
!   (B) Is it a direction, and so an implicit "go"?  If so go to (K)
!   (C) Is anyone being addressed?
!   (D) Get the verb: try all the syntax lines for that verb
!   (E) Break down a syntax line into analysed tokens
!   (F) Look ahead for advance warning for multiexcept/multiinside
!   (G) Parse each token in turn (calling ParseToken to do most of the work)
!   (H) Cheaply parse otherwise unrecognised conversation and return
!   (I) Print best possible error message
!   (J) Retry the whole lot
!   (K) Last thing: check for "then" and further instructions(s), return.
!
!   The strategic points (A) to (K) are marked in the commentary.
!
!   Note that there are three different places where a return can happen.
! ----------------------------------------------------------------------------

[ Parser__parse  results   syntax line num_lines line_address i j k
                           token l m line_etype vw forceLookahead;

    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    !
    ! A: Get the input, do "oops" and "again"
    !
    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    ! Firstly, in "not held" mode, we still have a command left over from last
    ! time (eg, the user typed "eat biscuit", which was parsed as "take biscuit"
    ! last time, with "eat biscuit" tucked away until now).  So we return that.

    if (notheld_mode == 1) {
        for (i=0 : i<8 : i++) results-->i = kept_results-->i;
        notheld_mode = 0;
        rtrue;
    }

    if (held_back_mode ~= 0) {
        held_back_mode = 0;
        Tokenise__(buffer, parse);
        jump ReParse;
    }

  .ReType;
    Keyboard(buffer, parse);

#Ifdef INFIX;
    ! An Infix verb is a special kind of meta verb.  We mark them here.
    if (GetNthChar(buffer, 0) == ';')
	infix_verb = true;
    else
	infix_verb = false;
#Endif;

  .ReParse;

    parser_inflection = name;
    parser_inflection_func = false;

    ! Initially assume the command is aimed at the player, and the verb
    ! is the first word

    num_words = NumberWords();
    wn = 1;

    #Ifdef LanguageToInformese;
    LanguageToInformese();
    #IfV5;
    ! Re-tokenise:
    Tokenise__(buffer,parse);
    #Endif; ! V5
    #Endif; ! LanguageToInformese

    if (BeforeParsing() == false) {
        LibraryExtensions.ext_number_1 = wn;    ! Set "between calls" functionality to restore wn each pass
        LibraryExtensions.BetweenCalls = LibraryExtensions.RestoreWN;
        LibraryExtensions.RunWhile(ext_beforeparsing, false);
        LibraryExtensions.BetweenCalls = 0;     ! Turn off "between calls" functionality
     }
    num_words = NumberWords();

    k=0;
    #Ifdef DEBUG;
    if (parser_trace >= 2) {
        print "[ ";
        for (i=0 : i<num_words : i++) {

            j = WordValue(i+1);
            k = WordAddress(i+1);
            l = WordLength(i+1);
            print "~"; for (m=0 : m<l : m++) print (char) k->m; print "~ ";

            if (j == 0) print "?";
            else {
                #Ifdef TARGET_ZCODE;
                if (UnsignedCompare(j, HDR_DICTIONARY-->0) >= 0 &&
                    UnsignedCompare(j, HDR_HIGHMEMORY-->0) < 0)
                     print (address) j;
                else print j;
                #Ifnot; ! TARGET_GLULX
                if (j->0 == $60) print (address) j;
                else print j;
                #Endif; ! TARGET_
            }
            if (i ~= num_words-1) print " / ";
        }
        print " ]^";
    }
    #Endif; ! DEBUG
    verb_wordnum = 1;
    actor = player;
    actors_location = ScopeCeiling(player);
    usual_grammar_after = 0;

  .AlmostReParse;

    scope_token = 0;
    action_to_be = NULL;

    ! Begin from what we currently think is the verb word

  .BeginCommand;

    wn = verb_wordnum;
    verb_word = NextWordStopped();

    ! If there's no input here, we must have something like "person,".

    if (verb_word == -1) {
        best_etype = STUCK_PE;
        jump GiveError;
    }

    ! Now try for "again" or "g", which are special cases: don't allow "again" if nothing
    ! has previously been typed; simply copy the previous text across

    if (verb_word == AGAIN2__WD or AGAIN3__WD) verb_word = AGAIN1__WD;
    if (verb_word == AGAIN1__WD) {
        if (actor ~= player) {
            L__M(##Miscellany, 20);
            jump ReType;
        }
        if (GetKeyBufLength(buffer3) == 0) {
            L__M(##Miscellany, 21);
            jump ReType;
        }

        if (WordAddress(verb_wordnum) == buffer + WORDSIZE) { ! not held back
            ! splice rest of buffer onto end of buffer3
            i = GetKeyBufLength(buffer3);
            while (buffer3 -> (i + WORDSIZE - 1) == ' ' or '.')
                i--;
            j = i - WordLength(verb_wordnum);  ! amount to move buffer up by
            if (j > 0) {
                for (m=INPUT_BUFFER_LEN-1 : m>=WORDSIZE+j : m--)
                    buffer->m = buffer->(m-j);
                SetKeyBufLength(GetKeyBufLength()+j);
                }
            for (m=WORDSIZE : m<WORDSIZE+i : m++) buffer->m = buffer3->m;
            if (j < 0) for (:m<WORDSIZE+i-j : m++) buffer->m = ' ';
         }
        else
            for (i=0 : i<INPUT_BUFFER_LEN : i++) buffer->i = buffer3->i;
        jump ReParse;
    }

    ! Save the present input in case of an "again" next time

    if (verb_word ~= AGAIN1__WD)
        for (i=0 : i<INPUT_BUFFER_LEN : i++) buffer3->i = buffer->i;

    if (usual_grammar_after == 0) {
        j = verb_wordnum;
        #Ifdef TARGET_ZCODE;
		parser_one = 0;
        #Endif;
        i = RunRoutines(actor, grammar);
        #Ifdef DEBUG;
        if (parser_trace >= 2 && actor.grammar ~= 0 or NULL)
            print " [Grammar property returned ", i, "]^";
        #Endif; ! DEBUG

        #Ifdef TARGET_ZCODE;
        if ((i ~= 0 or 1) &&
	    (parser_one ~= 0 ||
            (UnsignedCompare(i, dict_start) < 0 ||
             UnsignedCompare(i, dict_end) >= 0 ||
             (i - dict_start) % dict_entry_size ~= 0))) {
            usual_grammar_after = j;
            i=-i;
        }

        #Ifnot; ! TARGET_GLULX
        if (i < 0) { usual_grammar_after = j; i=-i; }
        #Endif;

        if (i == 1) {
            results-->0 = action;
            results-->1 = 0;		! Number of parameters
            results-->2 = noun;
            results-->3 = second;
            if (noun) results-->1 = 1;
            if (second) results-->1 = 2;
            rtrue;
        }
        if (i ~= 0) { verb_word = i; wn--; verb_wordnum--; }
        else { wn = verb_wordnum; verb_word = NextWord(); }
    }
    else usual_grammar_after = 0;

    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    !
    ! B: Is it a direction, and so an implicit "go"?  If so go to (K)
    !
    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    #Ifdef LanguageIsVerb;
    if (verb_word == 0) {
        i = wn; verb_word = LanguageIsVerb(buffer, parse, verb_wordnum);
        wn = i;
    }
    #Endif; ! LanguageIsVerb

    ! If the first word is not listed as a verb, it must be a direction
    ! or the name of someone to talk to

    if (verb_word == 0 || ((verb_word->#dict_par1) & DICT_VERB) == 0) {

        ! So is the first word an object contained in the special object "Compass"
        ! (i.e., a direction)?  This needs use of NounDomain, a routine which
        ! does the object matching, returning the object number, or 0 if none found,
        ! or REPARSE_CODE if it has restructured the parse table so the whole parse
        ! must be begun again...

        wn = verb_wordnum; indef_mode = false; token_filter = 0;
        l = NounDomain(Compass, 0, NOUN_TOKEN);
        if (l == REPARSE_CODE) jump ReParse;

        ! If it is a direction, send back the results:
        ! action=GoSub, no of arguments=1, argument 1=the direction.

        if (l ~= 0) {
            results-->0 = ##Go;
            action_to_be = ##Go;
            results-->1 = 1;
            results-->2 = l;
            jump LookForMore;
        }

    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    !
    ! C: Is anyone being addressed?
    !
    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        ! Only check for a comma (a "someone, do something" command) if we are
        ! not already in the middle of one.  (This simplification stops us from
        ! worrying about "robot, wizard, you are an idiot", telling the robot to
        ! tell the wizard that she is an idiot.)

        if (actor == player) {
            for (j=2 : j<=num_words : j++) {
                i=NextWord();
                if (i == comma_word) jump Conversation;
            }
        }
        vw = verb_word;
        verb_word = UnknownVerb(vw);
        if (verb_word == false) verb_word = LibraryExtensions.RunWhile(ext_unknownverb, false, vw);
        if (verb_word) jump VerbAccepted;
        best_etype = VERB_PE;
        jump GiveError;

        ! NextWord nudges the word number wn on by one each time, so we've now
        ! advanced past a comma.  (A comma is a word all on its own in the table.)

      .Conversation;

        j = wn - 1;
        if (j == 1) {
            L__M(##Miscellany, 22);
            jump ReType;
        }

        ! Use NounDomain (in the context of "animate creature") to see if the
        ! words make sense as the name of someone held or nearby

        wn = 1; lookahead = HELD_TOKEN;
        scope_reason = TALKING_REASON;
        l = NounDomain(player,actors_location,CREATURE_TOKEN);
        scope_reason = PARSING_REASON;
        if (l == REPARSE_CODE) jump ReParse;
        if (l == 0) {
            L__M(##Miscellany, 23);
            jump ReType;
        }

      .Conversation2;

        ! The object addressed must at least be "talkable" if not actually "animate"
        ! (the distinction allows, for instance, a microphone to be spoken to,
        ! without the parser thinking that the microphone is human).

        if (l hasnt animate && l hasnt talkable) {
            L__M(##Miscellany, 24, l);
            jump ReType;
        }

        ! Check that there aren't any mystery words between the end of the person's
        ! name and the comma (eg, throw out "dwarf sdfgsdgs, go north").

        if (wn ~= j) {
            L__M(##Miscellany, 25);
            jump ReType;
        }

        ! The player has now successfully named someone.  Adjust "him", "her", "it":

        PronounNotice(l);

        ! Set the global variable "actor", adjust the number of the first word,
        ! and begin parsing again from there.

        verb_wordnum = j + 1;

        ! Stop things like "me, again":

        if (l == player) {
            wn = verb_wordnum;
            if (NextWordStopped() == AGAIN1__WD or AGAIN2__WD or AGAIN3__WD) {
                L__M(##Miscellany, 20);
                jump ReType;
            }
        }

        actor = l;
        actors_location = ScopeCeiling(l);
        #Ifdef DEBUG;
        if (parser_trace >= 1)
            print "[Actor is ", (the) actor, " in ", (name) actors_location, "]^";
        #Endif; ! DEBUG
        jump BeginCommand;

    } ! end of first-word-not-a-verb

    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    !
    ! D: Get the verb: try all the syntax lines for that verb
    !
    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  .VerbAccepted;

    ! We now definitely have a verb, not a direction, whether we got here by the
    ! "take ..." or "person, take ..." method.  Get the meta flag for this verb:

    meta = (verb_word->#dict_par1) & DICT_META;

    ! You can't order other people to "full score" for you, and so on...

    if (meta && actor ~= player) {
        best_etype = VERB_PE;
        meta = false;
        jump GiveError;
    }

    ! Now let i be the corresponding verb number, stored in the dictionary entry
    ! (in a peculiar 255-n fashion for traditional Infocom reasons)...

    i = DictionaryWordToVerbNum(verb_word);

    ! ...then look up the i-th entry in the verb table, whose address is at word
    ! 7 in the Z-machine (in the header), so as to get the address of the syntax
    ! table for the given verb...

    #Ifdef TARGET_ZCODE;
    syntax = (HDR_STATICMEMORY-->0)-->i;
    #Ifnot; ! TARGET_GLULX
    syntax = (#grammar_table)-->(i+1);
    #Endif; ! TARGET_

    ! ...and then see how many lines (ie, different patterns corresponding to the
    ! same verb) are stored in the parse table...

    num_lines = (syntax->0) - 1;

    ! ...and now go through them all, one by one.
    ! To prevent pronoun_word 0 being misunderstood,

    pronoun_word = NULL; pronoun_obj = NULL;

    #Ifdef DEBUG;
    if (parser_trace >= 1) print "[Parsing for the verb '", (address) verb_word, "' (", num_lines+1, " lines)]^";
    #Endif; ! DEBUG

    best_etype = STUCK_PE; nextbest_etype = STUCK_PE;
    multiflag = false; saved_oops = 0;

    ! "best_etype" is the current failure-to-match error - it is by default
    ! the least informative one, "don't understand that sentence".
    ! "nextbest_etype" remembers the best alternative to having to ask a
    ! scope token for an error message (i.e., the best not counting ASKSCOPE_PE).
    ! multiflag is used here to prevent inappropriate MULTI_PE errors
    ! in addition to its unrelated duties passing information to action routines

    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    !
    ! E: Break down a syntax line into analysed tokens
    !
    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    line_address = syntax + 1;

    for (line=0 : line<=num_lines : line++) {

        for (i=0 : i<32 : i++) {
            line_token-->i = ENDIT_TOKEN;
            line_ttype-->i = ELEMENTARY_TT;
            line_tdata-->i = ENDIT_TOKEN;
        }

        ! Unpack the syntax line from Inform format into three arrays; ensure that
        ! the sequence of tokens ends in an ENDIT_TOKEN.

        line_address = UnpackGrammarLine(line_address);

        #Ifdef DEBUG;
        if (parser_trace >= 1) {
            if (parser_trace >= 2) new_line;
            print "[line ", line; DebugGrammarLine();
            print "]^";
        }
        #Endif; ! DEBUG

        ! We aren't in "not holding" or inferring modes, and haven't entered
        ! any parameters on the line yet, or any special numbers; the multiple
        ! object is still empty.

        token_filter = 0;
        not_holding = 0;
        inferfrom = 0;
        parameters = 0;
        nsns = 0; special_word = 0; special_number = 0;
        multiple_object-->0 = 0;
        multi_context = 0;
        etype = STUCK_PE; line_etype = 100;

        ! Put the word marker back to just after the verb

        wn = verb_wordnum+1;

    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    !
    ! F: Look ahead for advance warning for multiexcept/multiinside
    !
    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        ! There are two special cases where parsing a token now has to be
        ! affected by the result of parsing another token later, and these
        ! two cases (multiexcept and multiinside tokens) are helped by a quick
        ! look ahead, to work out the future token now.  We can only carry this
        ! out in the simple (but by far the most common) case:
        !
        !     multiexcept <one or more prepositions> noun
        !
        ! and similarly for multiinside.

        advance_warning = NULL; indef_mode = false;
        for (i=0,m=false,pcount=0 : line_token-->pcount ~= ENDIT_TOKEN : pcount++) {
            scope_token = 0;

            if (line_ttype-->pcount ~= PREPOSITION_TT) i++;

            forceLookahead = LibraryExtensions.RunWhile(ext_forceLookAhead,false);

            if (line_ttype-->pcount == ELEMENTARY_TT || forceLookahead == true) {
                if (line_tdata-->pcount == MULTI_TOKEN) m = true;
                if (forceLookahead == true || (line_tdata-->pcount == MULTIEXCEPT_TOKEN or MULTIINSIDE_TOKEN  && i == 1)) {

                    ! First non-preposition is "multiexcept" or
                    ! "multiinside", so look ahead.
                    #Ifdef DEBUG;
                    if (parser_trace >= 2) print " [Trying look-ahead]^";
                    #Endif; ! DEBUG

                    ! We need this to be followed by 1 or more prepositions.

                    pcount++;
                    if (line_ttype-->pcount == PREPOSITION_TT) {
                        ! skip ahead to a preposition word in the input
                        do {
                            l = NextWord();
                        } until ((wn > num_words) ||
                                 (l && (l->#dict_par1) & DICT_PREP ~= 0));

                        if (wn > num_words) {
                            #Ifdef DEBUG;
                            if (parser_trace >= 2)
                                print " [Look-ahead aborted: prepositions missing]^";
                            #Endif;
                            jump EmptyLine;
                        }

                        do {
                            if (PrepositionChain(l, pcount) ~= -1) {
                                ! advance past the chain
                                if ((line_token-->pcount)->0 & $20 ~= 0) {
                                    pcount++;
                                    while ((line_token-->pcount ~= ENDIT_TOKEN) &&
                                           ((line_token-->pcount)->0 & $10 ~= 0))
                                        pcount++;
                                } else {
                                    pcount++;
                                }
                            } else {
                                ! try to find another preposition word
                                do {
                                    l = NextWord();
                                } until ((wn >= num_words) ||
                                         (l && (l->#dict_par1) & 8 ~= 0));

                                if (l && (l->#dict_par1) & 8) continue;

                                ! lookahead failed
                                #Ifdef DEBUG;
                                if (parser_trace >= 2)
                                    print " [Look-ahead aborted: prepositions don't match]^";
                                #Endif;
                                jump LineFailed;
                            }
                            l = NextWord();
                        } until (line_ttype-->pcount ~= PREPOSITION_TT);

                        .EmptyLine;
                        ! put back the non-preposition we just read
                        wn--;

                        if (forceLookahead == true || ((line_ttype-->pcount == ELEMENTARY_TT)  && (line_tdata-->pcount == NOUN_TOKEN))) {

                            l = Descriptors();  ! skip past THE etc
                            if (l~=0) etype=l;  ! don't allow multiple objects
                            l = NounDomain(actors_location, actor, NOUN_TOKEN);

                            #Ifdef DEBUG;
                            if (parser_trace >= 2) {
                                print " [Advanced to ~noun~ token: ";
                                if (l == REPARSE_CODE) print "re-parse request]^";
                                if (l == 1) print "but multiple found]^";
                                if (l == 0) print "error ", etype, "]^";
                                if (l >= 2) print (the) l, "]^";
                            }
                            #Endif; ! DEBUG
                            if (l == REPARSE_CODE) jump ReParse;

                            if (l >= 2) advance_warning = l;

                        }
                    }
                    break;
                }
            }
        }

        ! Slightly different line-parsing rules will apply to "take multi", to
        ! prevent "take all" behaving correctly but misleadingly when there's
        ! nothing to take.

        take_all_rule = 0;
        if (m && params_wanted == 1 && action_to_be == ##Take)
            take_all_rule = 1;

        ! And now start again, properly, forearmed or not as the case may be.
        ! As a precaution, we clear all the variables again (they may have been
        ! disturbed by the call to NounDomain, which may have called outside
        ! code, which may have done anything!).

        not_holding = 0;
        inferfrom = 0;
        inferword = 0;
        parameters = 0;
        nsns = 0; special_word = 0; special_number = 0;
        multiple_object-->0 = 0;
        etype = STUCK_PE; line_etype = 100;
        wn = verb_wordnum+1;

    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    !
    ! G: Parse each token in turn (calling ParseToken to do most of the work)
    !
    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        ! "Pattern" gradually accumulates what has been recognised so far,
        ! so that it may be reprinted by the parser later on

        for (pcount=1 : : pcount++) {
            pattern-->pcount = PATTERN_NULL; scope_token = 0;

            token = line_token-->(pcount-1);
            lookahead = line_token-->pcount;

            #Ifdef DEBUG;
            if (parser_trace >= 2)
                print " [line ", line, " token ", pcount, " word ", wn, " : ", (DebugToken) token,
                  "]^";
            #Endif; ! DEBUG

            if (token ~= ENDIT_TOKEN) {
                scope_reason = PARSING_REASON;
                parser_inflection = name;
                parser_inflection_func = false;
                AnalyseToken(token);

                if (action_to_be == ##AskTo && found_ttype == ELEMENTARY_TT &&
                    found_tdata == TOPIC_TOKEN && line_etype == 100) {
                    if (actor ~= player) {
                        best_etype = VERB_PE; jump GiveError;
                    }
                    l = inputobjs-->2;
                    wn--;
                    j = wn;
                    jump Conversation2;
                }

                l = ParseToken__(found_ttype, found_tdata, pcount-1, token);
                while (l<-200) l = ParseToken__(ELEMENTARY_TT, l + 256);
                scope_reason = PARSING_REASON;

                if (l == GPR_PREPOSITION) {
                    if (found_ttype~=PREPOSITION_TT && (found_ttype~=ELEMENTARY_TT ||
                        found_tdata~=TOPIC_TOKEN)) params_wanted--;
                    l = true;
                }
                else
                    if (l < 0) l = false;
                    else
                        if (l ~= GPR_REPARSE) {
                            if (l == GPR_NUMBER) {
                                if (nsns == 0) special_number1 = parsed_number;
                                else special_number2 = parsed_number;
                                nsns++; l = 1;
                            }
                            if (l == GPR_MULTIPLE) l = 0;
                            results-->(parameters+2) = l;
                            parameters++;
                            pattern-->pcount = l;
                            l = true;
                        }

                #Ifdef DEBUG;
                if (parser_trace >= 3) {
                    print "  [token resulted in ";
                    if (l == REPARSE_CODE) print "re-parse request]^";
                    if (l == 0) print "failure with error type ", etype, "]^";
                    if (l == 1) print "success]^";
                }
                #Endif; ! DEBUG

                if (l == REPARSE_CODE) jump ReParse;
                if (l == false) {
                    if (etype < line_etype) line_etype = etype;
                    if (etype == STUCK_PE || wn >= num_words) break;
                }
            }
            else {

                ! If the player has entered enough already but there's still
                ! text to wade through: store the pattern away so as to be able to produce
                ! a decent error message if this turns out to be the best we ever manage,
                ! and in the mean time give up on this line

                ! However, if the superfluous text begins with a comma or "then" then
                ! take that to be the start of another instruction

                if (line_etype < 100) break;
                if (wn <= num_words) {
                    l = NextWord();
                    if (l == THEN1__WD or THEN2__WD or THEN3__WD or comma_word or AND1__WD) {
                        held_back_mode = 1; hb_wn = wn-1;
                    }
                    else {
                        for (m=0 : m<32 : m++) pattern2-->m = pattern-->m;
                        pcount2 = pcount;
                        etype = UPTO_PE;
                        break;
                    }
                }

                ! Now, we may need to revise the multiple object because of the single one
                ! we now know (but didn't when the list was drawn up).

                if (parameters >= 1 && results-->2 == 0) {
                    l = ReviseMulti(results-->3);
                    if (l ~= 0) { etype = l; results-->0 = action_to_be; break; }
                }
                if (parameters >= 2 && results-->3 == 0) {
                    l = ReviseMulti(results-->2);
                    if (l ~= 0) { etype = l; break; }
                }

                ! To trap the case of "take all" inferring only "yourself" when absolutely
                ! nothing else is in the vicinity...

                if (take_all_rule == 2 && results-->2 == actor) {
                    best_etype = NOTHING_PE;
                    jump GiveError;
                }

                #Ifdef DEBUG;
                if (parser_trace >= 1) print "[Line successfully parsed]^";
                #Endif; ! DEBUG

                ! The line has successfully matched the text.  Declare the input error-free...

                oops_from = 0;

                ! ...explain any inferences made (using the pattern)...
		! This is where the parser replies (the sword) if things aren't clear.
                if (inferfrom ~= 0 && no_infer_message == false) {
                    print "("; PrintCommand(inferfrom); print ")^";
                }
		no_infer_message = false;

                ! ...copy the action number, and the number of parameters...

                results-->0 = action_to_be;
                results-->1 = parameters;

                ! ...reverse first and second parameters if need be...

                if (action_reversed && parameters == 2) {
                    i = results-->2; results-->2 = results-->3;
                    results-->3 = i;
                    if (nsns == 2) {
                        i = special_number1; special_number1 = special_number2;
                        special_number2 = i;
                    }
                }

                ! ...and to reset "it"-style objects to the first of these parameters, if
                ! there is one (and it really is an object)...

                if (parameters > 0 && results-->2 >= 2)
                    PronounNotice(results-->2);

                ! ...and worry about the case where an object was allowed as a parameter
                ! even though the player wasn't holding it and should have been: in this
                ! event, keep the results for next time round, go into "not holding" mode,
                ! and for now tell the player what's happening and return a "take" request
                ! instead...

                  if (not_holding ~= 0) i=LibraryExtensions.RunWhile(ext_suppressParsingImplicitTake, false, action_to_be , results-->2, results-->3);

                  if(i==-1)  jump ReType;
                  if(i==false){ !-perform default "implicit take" functionality (true skips over this)
                     if (not_holding ~= 0 && actor == player) {
                        action = ##Take;
                        i = RunRoutines(not_holding, before_implicit);
                        ! i = 0: Take the object, tell the player (default)
                        ! i = 1: Take the object, don't tell the player
                        ! i = 2: don't Take the object, continue
                        ! i = 3: don't Take the object, don't continue

                        if (i > 2 || no_implicit_actions) { best_etype = NOTHELD_PE; jump GiveError; }
                        ! perform the implicit Take
                        if (i < 2) {
                            if (i ~= 1)     ! and tell the player
                                L__M(##Miscellany, 26, not_holding);

                            notheld_mode = 1;
                            for (i=0 : i<8 : i++) kept_results-->i = results-->i;
                            results-->0 = ##Take;
                            results-->1 = 1;
                            results-->2 = not_holding;
                        }
                    }
                  }

                ! (Notice that implicit takes are only generated for the player, and not
                ! for other actors.  This avoids entirely logical, but misleading, text
                ! being printed.)

                ! ...and return from the parser altogether, having successfully matched
                ! a line.

                if (held_back_mode == 1) {
                    wn=hb_wn;
                    jump LookForMore;
                }
                rtrue;

            } ! end of if(token ~= ENDIT_TOKEN) else
        } ! end of for(pcount++)

        .LineFailed;
        ! The line has failed to match.
        ! We continue the outer "for" loop, trying the next line in the grammar.

        if (line_etype < 100) etype = line_etype;
        if (etype > best_etype) best_etype = etype;
        if (etype ~= ASKSCOPE_PE && etype > nextbest_etype) nextbest_etype = etype;

        ! ...unless the line was something like "take all" which failed because
        ! nothing matched the "all", in which case we stop and give an error now.

        if (take_all_rule == 2 && etype==NOTHING_PE) break;

    } ! end of for(line++)

    ! The grammar is exhausted: every line has failed to match.

    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    !
    ! H: Cheaply parse otherwise unrecognised conversation and return
    !
    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

  .GiveError;

    etype = best_etype;

    ! Errors are handled differently depending on who was talking.
    ! If the command was addressed to somebody else (eg, "dwarf, sfgh") then
    ! it is taken as conversation which the parser has no business in disallowing.

    if (actor ~= player) {
        if (usual_grammar_after ~= 0) {
            verb_wordnum = usual_grammar_after;
            jump AlmostReParse;
        }
        wn = verb_wordnum;
        special_word = NextWord();
        if (special_word == comma_word) {
            special_word = NextWord();
            verb_wordnum++;
        }
        special_number = TryNumber(verb_wordnum);
        results-->0 = ##NotUnderstood;
        results-->1 = 2;
        results-->2 = 1; special_number1 = special_word;
        results-->3 = actor;
        consult_from = verb_wordnum; consult_words = num_words-consult_from+1;
        rtrue;
    }

    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    !
    ! I: Print best possible error message
    !
    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    ! If the player was the actor (eg, in "take dfghh") the error must be
    ! printed, and fresh input called for.  In four cases the oops word
    ! must be jiggled (where oops_from is set to something).

    if (ParserError(etype)) jump ReType;
    if (LibraryExtensions.RunWhile(ext_parsererror, false, etype, results)) jump ReType;

    pronoun_word = pronoun__word; pronoun_obj = pronoun__obj;

    if (etype == STUCK_PE) {    L__M(##Miscellany, 27); oops_from = 1; }
    if (etype == UPTO_PE) {     L__M(##Miscellany, 28);
        for (m=0 : m<32 : m++) pattern-->m = pattern2-->m;
        pcount = pcount2; PrintCommand(0); L__M(##Miscellany, 56);
        oops_from = wn-1;
    }
    if (etype == NUMBER_PE)     L__M(##Miscellany, 29);
    if (etype == CANTSEE_PE) {  L__M(##Miscellany, 30); oops_from=saved_oops;}
    if (etype == TOOLIT_PE)     L__M(##Miscellany, 31);
    if (etype == NOTHELD_PE) {  L__M(##Miscellany, 32, not_holding); oops_from=saved_oops; }
    if (etype == MULTI_PE)      L__M(##Miscellany, 33);
    if (etype == MMULTI_PE)     L__M(##Miscellany, 34);
    if (etype == VAGUE_PE)      L__M(##Miscellany, 35, pronoun_word);
    if (etype == EXCEPT_PE)     L__M(##Miscellany, 36);
    if (etype == ANIMA_PE)      L__M(##Miscellany, 37);
    if (etype == VERB_PE)       L__M(##Miscellany, 38);
    if (etype == SCENERY_PE)    L__M(##Miscellany, 39);
    if (etype == ITGONE_PE) {
        if (pronoun_obj == NULL)
                                L__M(##Miscellany, 35, pronoun_word);
        else                    L__M(##Miscellany, 40, pronoun_word, pronoun_obj);
    }
    if (etype == JUNKAFTER_PE)  L__M(##Miscellany, 41);
    if (etype == TOOFEW_PE)     L__M(##Miscellany, 42, multi_had);
    if (etype == NOTHING_PE) {
        if (results-->0 == ##Remove && results-->3 ofclass Object) {
            noun = results-->3; ! ensure valid for messages
            if (noun has animate) L__M(##Miscellany, 44, verb_word);
            else if (noun hasnt container or supporter) L__M(##Insert, 2, noun);
            else if (noun has container && noun hasnt open) L__M(##Take, 9, noun);
            else if (children(noun)==0) L__M(##Search, 6, noun);
            else results-->0 = 0;
        }
        if (results-->0 ~= ##Remove) {
            if (multi_wanted == 100)    L__M(##Miscellany, 43);
            else {
                #Ifdef NO_TAKE_ALL;
                if (take_all_rule == 2) L__M(##Miscellany, 59);
                else                    L__M(##Miscellany, 44, verb_word);
                #Ifnot;
                L__M(##Miscellany, 44, verb_word);
                #Endif; ! NO_TAKE_ALL
            }
        }
    }
    if (etype == ASKSCOPE_PE) {
        scope_stage = 3;
        if (scope_error() == -1) {
            best_etype = nextbest_etype;
            jump GiveError;
        }
    }

    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    !
    ! J: Retry the whole lot
    !
    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    ! And go (almost) right back to square one...

    jump ReType;

    ! ...being careful not to go all the way back, to avoid infinite repetition
    ! of a deferred command causing an error.


    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    !
    ! K: Last thing: check for "then" and further instructions(s), return.
    !
    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    ! At this point, the return value is all prepared, and we are only looking
    ! to see if there is a "then" followed by subsequent instruction(s).

  .LookForMore;

    if (wn > num_words) rtrue;

    i = NextWord();
    if (i == THEN1__WD or THEN2__WD or THEN3__WD or comma_word or AND1__WD) {
        if (wn > num_words) {
           held_back_mode = false;
           return;
        }
        i = WordAddress(verb_wordnum);
        j = WordAddress(wn);
        for (: i<j : i++) i->0 = ' ';
        i = NextWord();
        if (i == AGAIN1__WD or AGAIN2__WD or AGAIN3__WD) {
            ! Delete the words "then again" from the again buffer,
            ! in which we have just realised that it must occur:
            ! prevents an infinite loop on "i. again"

            i = WordAddress(wn-2)-buffer;
            if (wn > num_words) j = INPUT_BUFFER_LEN-1;
            else j = WordAddress(wn)-buffer;
            for (: i<j : i++) buffer3->i = ' ';
        }
        Tokenise__(buffer,parse);
        held_back_mode = true;
        return;
    }
    best_etype = UPTO_PE;
    jump GiveError;

]; ! end of Parser__parse

#endif; !--Before Parser
!======================================================================================
! AFTER PARSER
#iftrue (LIBRARY_STAGE == AFTER_PARSER);

#endif; !--After Parser
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);

#endif; !--After VERBLIB
!======================================================================================
! AFTER Grammar
#iftrue (LIBRARY_STAGE == AFTER_GRAMMAR);

#endif; !--After Grammar
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
