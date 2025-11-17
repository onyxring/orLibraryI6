!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.06.30 orInformLibraryHook
! Adds hooks to the InformLibrary.play() routine:
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License:
!--------------------------------------------------------------------------------------
! This extension, classified as a "hook", provides tooling for other extensions to
! use and provides no specific features by itself.
!
!  ext_playLoopRewriteParsedCommands - used to change the action variable and swap entries in the inputobjs array
!  ext_playLoopAssignObjectVariables - assign the noun and second variables
!  ext_playLoopPerformSingleObjectAction - perform a simple command, when the noun and second variables are correctly resolved
!  ext_playLoopPerformMultiObjectAction - perform a command against multiple objects
!--------------------------------------------------------------------------------------
! Revision History
! 2002.02.07	Initial Creation
! ...			   History from orLibrary v1 snipped away.
! 2024.06.30	Refactored for the orLibrary version 2.
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orHookInformLibrary_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES

!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orHookInformLibrary_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orHookInformLibrary_STAGE  < LIBRARY_STAGE);
   #undef      orHookInformLibrary_STAGE  ;
   Constant    orHookInformLibrary_STAGE  LIBRARY_STAGE;
   #ifdef      orHookInformLibrary_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orHookInformLibrary..."; #endif;
!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);
   replace main;
   property individual ext_playLoopRewriteParsedCommands;
   property individual ext_playLoopAssignObjectVariables;
   property individual ext_playLoopPerformSingleObjectAction;
   property individual ext_playLoopPerformMultiObjectAction;
#endif; !--Before Parser
!======================================================================================
! AFTER PARSER
#iftrue (LIBRARY_STAGE == AFTER_PARSER);
	[ Main o; o=o; !suppress warning
		InformLibrary.play=__orPlayHooks;
      #ifdef allocateInformExtensions;
            allocateInformExtensions();
      #endif;
		InformLibrary.play();
	];

   [__orPlayHooks i j k l;
         #Ifdef TARGET_ZCODE;
            ZZInitialise();
         #Ifnot; ! TARGET_GLULX
            GGInitialise();
         #Endif; ! TARGET_

         GamePrologue();

         while (~~deadflag) {    ! everything happens in this loop
            #Ifdef EnglishNaturalLanguage;
               PronounOldEnglish();
               old_itobj = PronounValue('it');
               old_himobj = PronounValue('him');
               old_herobj = PronounValue('her');
            #Endif; ! EnglishNaturalLanguage

         .very__late__error;
               if (score ~= last_score) {
               if (notify_mode == 1) NotifyTheScore();
               last_score = score;
            }
         .late__error;
            inputobjs-->0 = 0; inputobjs-->1 = 0;
            inputobjs-->2 = 0; inputobjs-->3 = 0; meta = false;

            InformParser.parse_input(inputobjs);
            action = inputobjs-->0;

            if(LibraryExtensions.RunWhile(ext_playLoopRewriteParsedCommands, false) == false) {!--return true to interrupt standard library's default behavior
                  ! Reverse "give fred biscuit" into "give biscuit to fred"
                  if (action == ##GiveR or ##ShowR) {
                     i = inputobjs-->2; inputobjs-->2 = inputobjs-->3; inputobjs-->3 = i;
                     if (action == ##GiveR) action = ##Give; else action = ##Show;
                  }

                  ! Convert "P, tell me about X" to "ask P about X"
                  if (action == ##Tell && inputobjs-->2 == player && actor ~= player) {
                     inputobjs-->2 = actor; actor = player; action = ##Ask;
                  }

                  ! Convert "ask P for X" to "P, give X to me"
                  if (action == ##AskFor && inputobjs-->2 ~= player && actor == player) {
                     actor = inputobjs-->2; inputobjs-->2 = inputobjs-->3;
                     inputobjs-->3 = player; action = ##Give;
                  }

                  ! For old, obsolete code: special_word contains the topic word
                  ! in conversation
                  if (action == ##Ask or ##Tell or ##Answer)
                     special_word = special_number1;
            }

            multiflag = false; onotheld_mode = notheld_mode; notheld_mode = false;  ! For implicit taking and multiple object detection

         .begin__action;
            inp1 = 0; inp2 = 0; i = inputobjs-->1;
            if (i >= 1) inp1 = inputobjs-->2;
            if (i >= 2) inp2 = inputobjs-->3;

            if(LibraryExtensions.RunWhile(ext_playLoopAssignObjectVariables, false) == false) {!--return true to interrupt standard library's default behavior
               ! inp1 and inp2 hold: object numbers, or 0 for "multiple object",
               ! or 1 for "a number or dictionary address"

               if (inp1 == 1) noun = special_number1; else noun = inp1;
               if (inp2 == 1) {
                  if (inp1 == 1) second = special_number2;
                  else           second = special_number1;
               }
               else second = inp2;
            }

            switch(LibraryExtensions.RunWhile(ext_playLoopPerformSingleObjectAction,0)){
               1: jump begin__action;
               2: jump turn__end;
               3: jump late__error;
               0: !--default behavior
                  if ( (i == 0) || (i == 1 && inp1 ~= 0) || (i == 2 && inp1 ~= 0 && inp2 ~= 0)) {
                     if (actor ~= player) {
                        switch (self.actor_act(actor, action, noun, second)) {
                           ACTOR_ACT_ABORT_NOTUNDERSTOOD:
                              jump begin__action;
                           default:
                              jump turn__end;
                           }
                     }
                     self.begin_action(action, noun, second, 0);
                     jump turn__end;
                  }
            }

            switch(LibraryExtensions.RunWhile(ext_playLoopPerformMultiObjectAction,0)){
               1: jump begin__action;
               2: jump turn__end;
               3: jump late__error;
               0: !--default behavior
                  ! (a) check the multiple list isn't empty;
                  ! (b) warn the player if it has been cut short because too long;
                  ! (c) generate a sequence of actions from the list
                  !     (stopping in the event of death or movement away).
                  multiflag = true;
                  j = multiple_object-->0;
                  if (j == 0) {
                     L__M(##Miscellany, 2);
                     jump late__error;
                  }
                  if (toomany_flag) {
                     toomany_flag = false;
                     L__M(##Miscellany, 1);
                  }
                  i = location;
                  for (k=1 : k<=j : k++) {
                     if (deadflag) break;
                     if (location ~= i) {
                           L__M(##Miscellany, 51);
                           break;
                     }
                     l = multiple_object-->k;
                     PronounNotice(l);
                     print (name) l, (string) COLON__TX, " ";
                     if (inp1 == 0) {
                           inp1 = l;
                           switch (self.actor_act(actor, action, l, second)) {
                              ACTOR_ACT_ABORT_NOTUNDERSTOOD:
                                 jump begin__action;
                              ACTOR_ACT_ABORT_ORDER:
                                 jump turn__end;
                           }
                           inp1 = 0;
                     }
                     else {
                           inp2 = l;
                           if (self.actor_act(actor, action, noun, l) == ACTOR_ACT_ABORT_NOTUNDERSTOOD)
                              jump begin__action;
                           inp2 = 0;
                     }
                  }
            }
         .turn__end;
            ! No time passes if either (i) the verb was meta, or
            ! (ii) we've only had the implicit take before the "real"
            ! action to follow.
            if (notheld_mode == 1) { NoteObjectAcquisitions(); continue; }
            if (meta) continue;
            if (~~deadflag) self.end_turn_sequence();
            else if (START_MOVE ~= 1) turns++;

         }

         if (deadflag ~= 2 && AfterLife() == false)
            LibraryExtensions.RunAll(ext_afterlife);

         if (deadflag == 0) jump very__late__error;

         GameEpilogue();
   ];

#endif; !--After PARSER
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE

