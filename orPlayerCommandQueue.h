!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.08.10 orPlayerCommandQueue
! Gives the player a queue of actions that will be executed sequentially without
! pause (unless interupted, or one of the next action becomes impossible).
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! A hook into the player's input routines, enabling actions for multiple turns
! to be specified in advance.  An example for a use might be a path of GO commands
! populated using ORPathMaker, so that the player could type "Go to the Library" and
! the actions needed to get there could be generated and placed in the actions queue.
! Another, simpler, example might be a command like "Wait 10 turns" :
!
!	extend "wait" first
!		* number 'turns' -> WaitNum
!		* number -> WaitNum
!	;
!	[WaitNumSub t;
!		if(noun>ACTIONQUEUE_BUFFER) {
!			noun=ACTIONQUEUE_BUFFER; print "[Maximum number of queued actions is ",ACTIONQUEUE_BUFFER,".]^^";
!		}
!		for(t=0:t<noun-1:t++) playerCommands.push(##Wait); !--queue up all but the first one
!		<<WAIT>>; !--go ahead and do the first one now
!	];
!
!--------------------------------------------------------------------------------------
! Revision History
! 2004.02.28	Initial Creation (orplayerActions)
! ...			   History from orLibrary v1 snipped away.
! 2024.08.10	Refactored for the orLibrary version 2
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orPlayerCommandQueue_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orActionQueue";
#include "_orInfExt";
#include "orUtilBuf";
#include "_orHookKeyboard";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orPlayerCommandQueue_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orPlayerCommandQueue_STAGE  < LIBRARY_STAGE);
   #undef      orPlayerCommandQueue_STAGE  ;
   Constant    orPlayerCommandQueue_STAGE  LIBRARY_STAGE;
   #ifdef      orPlayerCommandQueue_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orPlayerCommandQueue...";#endif;

!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);
	Constant noInferMessage 1;

#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
	[redirectCommand cmd;
        playerCommands.preemptCommand(cmd, false, true);
        meta=true;
        orSuppressNewlineAfterDescription = true;
        rtrue;
    ];

	orInfExt with ext_messages[;
				Miscellany:switch(lm_n){
							99: print "[Queued actions interrupted by an event.]^";
							98: print "[Queued actions are no longer possible.]^";
						   }
			]
		,	ext_keyboardPrimitive[ a_buffer a_table;
				if(doQueuedAction(a_buffer)){
					Tokenise__(a_buffer,a_table); !TODO does this do anything?
					return true;
				}
				rfalse;
			]
		,	ext_beforePrompt[;
				orActionFlags=0;
				!if(playerCommands.getLength()>0 && playerCommands.get(0).keepSilent==true) rtrue;
				if(playerCommands.getLength()>0) rtrue; !--the queued action processor will print its own prompt
				rfalse;
			]
		;

	orActionQueue playerCommands;

	[doQueuedAction inputBuffer
			a iqa t;

		if(playerCommands.getLength()==0) rfalse; !--no queued actions

		if(just_undone~=0){ !--there are queued actions, but they were restored from the last undo, so lets not do them...
			playerCommands.clear();
			rfalse;
		}
		a=playerCommands.dequeue();

		iqa=ValueOrRun(playerCommands,interrupt);

		!-- actions have been interrupted...
		if(iqa){ ! || a.CanAttempt()==false){
			if(iqa==true) L__M(##Miscellany,99);
			else L__M(##Miscellany,98);
			playerCommands.clear();
			orAction.destroy(a);
			if(metaclass(playerCommands.interrupt)~=Routine) playerCommands.interrupt=false;
			return true;
		}

		if(a.getCustomFlag(noInferMessage)) no_infer_message = true;

		for(t=WORDSIZE:t<INPUT_BUFFER_LEN - WORDSIZE:t++) inputBuffer->t=0;

		!also print it to the passed in input buffer, which is where the text would go if they actually DID type it

		util.orBuf.capture(inputBuffer);
			 a.print();
		util.orBuf.release(); !--since we print to it as though it were a normal array, buffer starts with a WORD-sized length (the MSB will be zero unless we printed too much).

		!for(t=WORDSIZE:t<INPUT_BUFFER_LEN - WORDSIZE:t++) if(inputBuffer->t==13) inputBuffer->t=32; !remove returns, since these are not possible via input but may show up from the print statment
		for(t=WORDSIZE:t<INPUT_BUFFER_LEN:t++) if(inputBuffer->t==13) inputBuffer->t=32; !remove returns, since these are not possible via input but may show up from the print statment

		util.orBuf.toLower(inputBuffer); !all the text in the input buffer must be lower case or the parser won't recognize it

		if(a.keepSilent==false){
			print (string) orQueuedActionPrompt;

			style bold;
			util.orBuf.print(inputBuffer); !--print the text of the command so it resembles something typed.
			style roman;
			new_line;
		}

		#ifdef TARGET_ZCODE;
		inputBuffer->0=INPUT_BUFFER_LEN - WORDSIZE; !for the Z-Machine only, this is a bit of manual patching, making the first byte equal to buffer size, just as though the standard library did all of this. (first byte size of buffer; second byte length of string, which is already populated because it cannot be longer than 255)
		#endif;

		orActionFlags=a.getCustomFlags();
		orAction.destroy(a);

		!for(t=WORDSIZE:t<23:t++) print inputBuffer->t,",";
		!new_line;

		!InformLibrary.end_turn_sequence(); !--do all the normal stuff that follows an action
		!DrawStatusLine();
		!DisplayStatus();
		!if(deadflag) GameEpilogue();
		return true;
	];
default  orQueuedActionPrompt "^>>";
#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
