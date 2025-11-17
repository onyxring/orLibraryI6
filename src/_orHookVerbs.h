!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.07.10 orHookVerbs
! Added hooks into several standard verbs, so they may be extended by multiple
! extensions without needing to be replaced.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! This extension, classified as a "hook", provides tooling for other extensions to
! use and provides no specific features by itself.
!--------------------------------------------------------------------------------------
! Revision History
! 2024.07.10	Initial Creation
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orHookVerbs_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orInfExt";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orHookVerbs_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orHookVerbs_STAGE  < LIBRARY_STAGE);
   #undef      orHookVerbs_STAGE  ;
   Constant    orHookVerbs_STAGE  LIBRARY_STAGE;
   #ifdef      orHookVerbs_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orHookVerbs..."; #endif;
!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);
   replace ExamineSub;
   replace LookSub;
   replace GoSub;
   replace TransferSub;

   property individual ext_examineSub;
   property individual ext_preDescription;
   property individual ext_postDescription;
   property individual ext_lookSub;
   property individual ext_goSub;
   property individual ext_goSubNoDir;

   Global orSuppressNewlineAfterDescription false;
#endif; !--Before Parser
!======================================================================================
! AFTER PARSER
#iftrue (LIBRARY_STAGE == AFTER_PARSER);
    orInfExt with ext_messages[;
        Look: switch(lm_n) {
            !--This is the same default message; however the standard library doesn't honor PrintOrRun's parameter to suppress the linefeed.  We need it to for this revised version of examine, so here's the revised message...
            7: CSubjectVerb(actor,true,false,"see",0,"sees", "saw"); print " nothing unexpected in that direction.";
                rtrue;
        }
    ];
#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);

   [ ExamineSub i;
        orSuppressNewlineAfterDescription =false;
        if(LibraryExtensions.RunWhile(ext_examineSub,false)~=false) return;

        if (location == thedark) return L__M(##Examine, 1, noun);
        i = noun.description;
        if (i == 0) {
        if (noun has container)
                if (noun has open or transparent) <<Search noun, actor>>;
                else return L__M(##Search, 5, noun);
        if (noun has switchable) { L__M(##Examine, 3, noun); rfalse; }
        return L__M(##Examine, 2, noun);
        }

        LibraryExtensions.RunAll(ext_preDescription);
        i = PrintOrRun(noun, description, true); !true: suppress the new_line feed
        LibraryExtensions.RunAll(ext_postDescription);
        if(orSuppressNewlineAfterDescription == false) new_line; !do the newline feed that normally goes after the description, here.
        if (i == 0 && noun has switchable) L__M(##Examine, 3, noun);
        AfterRoutines();

   ];
   [ LookSub allow_abbrev  visibility_levels i j k nl_flag;
        if(LibraryExtensions.RunWhile(ext_lookSub,false)~=false) return;
        if (parent(player) == 0) return RunTimeError(10);

  .MovedByInitial;

    if (location == thedark) { visibility_ceiling = thedark; NoteArrival(); }
    else {
        visibility_levels = FindVisibilityLevels();
        if (visibility_ceiling == location) {
            NoteArrival();
            if (visibility_ceiling ~= location) jump MovedByInitial;
        }
    }
    ! Printing the top line: e.g.
    ! Octagonal Room (on the table) (as Frodo)
    new_line;
    #Ifdef TARGET_ZCODE;
    style bold;
    #Ifnot; ! TARGET_GLULX;
    glk_set_style(style_Subheader);
    #Endif; ! TARGET_
    if (visibility_levels == 0) print (name) thedark;
    else {
        if (visibility_ceiling ~= location) print (The) visibility_ceiling;
        else print (name) visibility_ceiling;
    }
    #Ifdef TARGET_ZCODE;
    style roman;
    #Ifnot; ! TARGET_GLULX;
    glk_set_style(style_Normal);
    #Endif; ! TARGET_

    for (j=1,i=parent(player) : j<visibility_levels : j++,i=parent(i))
        if (i has supporter) L__M(##Look, 1, i);
        else                 L__M(##Look, 2, i);

    if (print_player_flag == 1) L__M(##Look, 3, player);
    new_line;

    ! The room description (if visible)
    LibraryExtensions.RunAll(ext_preDescription);
    if (lookmode < 3 && visibility_ceiling == location) {
        if ((allow_abbrev ~= 1) || (lookmode == 2) || (location hasnt visited)) {
            if (location.&describe) RunRoutines(location, describe);
            else {
                if (location.description == 0) RunTimeError(11, location, description);
                else PrintOrRun(location, description, true);
            }
        }
    }
    LibraryExtensions.RunAll(ext_postDescription);
    new_line;
    if (visibility_ceiling == location) nl_flag = 1;
    if (visibility_levels == 0) Locale(thedark);
    else {
        for (i=player,j=visibility_levels : j>0 : j--,i=parent(i)) give i workflag;

        for (j=visibility_levels : j>0 : j--) {
            for (i=player,k=0 : k<j : k++) i=parent(i);
            if (i.inside_description) {
                if (nl_flag) new_line; else nl_flag = 1;
                PrintOrRun(i,inside_description);
            }
            if (Locale(i)) nl_flag=1;
        }
    }
    if(LookRoutine() == false) LibraryExtensions.RunAll(ext_lookroutine);

    ScoreArrival();
    action = ##Look;
    AfterRoutines();
];

[ GoSub i j k movewith thedir next_loc;
    if(LibraryExtensions.RunWhile(ext_goSub,false)~=false) return;
    ! first, check if any PushDir object is touchable
    if (second && second notin Compass && ObjectIsUntouchable(second)) return;

    movewith = 0;
    i = parent(actor);
    if ((location ~= thedark && i ~= location) || (location == thedark && i ~= real_location)) {
        j = location;
        if (location == thedark) location = real_location;
        k = RunRoutines(i, before); if (k ~= 3) location = j;
        if (k == 1) {
           movewith = i; i = parent(i);
        }
        else {
            if (k) rtrue;
            if (ImplicitExit(i)) return L__M(##Go, 1, i);
            i = parent(actor);
        }
    }

    if (noun.door_dir ~= nothing) {
	thedir = noun.door_dir;
	if (metaclass(thedir) == Routine)
	    thedir = RunRoutines(noun, door_dir);
	next_loc = i.thedir;
    } else
	next_loc = noun;

    k = metaclass(next_loc);

    if (k == String) {
	print (string) next_loc;
	new_line; rfalse;
    }

    if (k == Routine) {
        next_loc = RunRoutines(i, thedir);
        if (next_loc == 1) rtrue;
    }

    if (k == nothing || next_loc == 0) {
        if(LibraryExtensions.RunWhile(ext_goSubNoDir,false,thedir)~=false) return;

        if (i.cant_go ~= 0 or CANTGO__TX) PrintOrRun(i, cant_go);
        else                              L__M(##Go, 2);
        rfalse;

    }

    if (next_loc has door) {
        if (next_loc has concealed) return L__M(##Go, 2);
        if (next_loc hasnt open && ImplicitOpen(next_loc)) {
            if (noun == u_obj) return L__M(##Go, 3, next_loc);
            if (noun == d_obj) return L__M(##Go, 4, next_loc);
            return L__M(##Go, 5, next_loc);
        }
        k = RunRoutines(next_loc, door_to);
        if (k == 0) return L__M(##Go, 6, next_loc);
        if (k == 1) rtrue;
        next_loc = k;
    }

    action = ##Going;
    if (RunRoutines(next_loc, before)) { action = ##Go; return; }
    action = ##Go;

    if (movewith == 0) move actor to next_loc; else move movewith to next_loc;
    if (actor ~= player) return L__M(##Go, 7);

    k = location; location = next_loc;
    MoveFloatingObjects();
    if (OffersLight(location))
        lightflag = true;
    else {
        lightflag = false;
        if (k == thedark) {
            if(DarkToDark() == false) ! From real_location To location
                LibraryExtensions.RunAll(ext_darktodark);
            if (deadflag) rtrue;
        }
        location = thedark;
    }
    NoteDeparture(); real_location = next_loc;
    action = ##Going;
    if (RunRoutines(prev_location, after)) { action = ##Go; return; }
    action = ##Go;
    if (AfterRoutines() || keep_silent) return;
    LookSub(1);
];

[ TransferSub ks;
    ks = keep_silent;

    if (noun notin actor) {
        keep_silent = true;
        AttemptToTakeObject(noun);
    }

    if (noun in actor) {
    	if (second has supporter) <PutOn noun second, actor>;
	    !--jimf: I think the following, from the standard library, has a bug which generates a bogus insert after the appropriate drop...
        !if (second == d_obj) <Drop noun, actor>;
	    !<Insert noun second, actor>;

        !--jimf: In my tests, the following fixes it...
        else if (second == d_obj) <Drop noun, actor>;
	    else <Insert noun second, actor>;
    }

    keep_silent = ks;

    if (noun notin actor) rfalse;
    rtrue;

];

#endif; !--After VERBLIB
!======================================================================================
! AFTER Grammar
#iftrue (LIBRARY_STAGE == AFTER_GRAMMAR);

#endif; !--After Grammar
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
