!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.02.07 orUniqueMultiMessage	[Z, GLULX]
! Consolidates identical messages for actions that accept multiple objects.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! Several identical messages are often generated when a player performs an action that
! affects multiple objects.  Consider the following normal transcript:
!
!	>TAKE ALL MARBLES FROM BOWL
!	marble: Removed.
!	marble: Removed.
!	marble: Removed.
!	marble: Removed.
!	marble: Removed.
!	marble: Removed.
!
! The orUniqueMultiMessage module will analyze the responses made by each of the
! commands and attempt to consolidate them (provided they occur sequentially).
! By leveraging this entry, the above example would appear as:
!
!	>TAKE ALL MARBLES FROM BOWL
!	6 marbles: Removed.
!
! No additional code need be written to make this functionality occur.  Simply include
! this module and consolidated messages will occur.
!--------------------------------------------------------------------------------------
! Revision History
! 2002.02.08	Initial Creation
!! ...			History from orLibrary v1 snipped away.
! 2024.06.30	Refactored for the orLibrary version 2.
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
#ifndef        orUniqueMultiMessage_STAGE;
Constant        orUniqueMultiMessage_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "orUtilBuf";
#include "_orHookInformLibrary";
!--------------------------------------------------------------------------------------
#endif;
#ifnot;
#ifndef        orUniqueMultiMessage_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orUniqueMultiMessage_STAGE  < LIBRARY_STAGE);
   #undef      orUniqueMultiMessage_STAGE  ;
   Constant    orUniqueMultiMessage_STAGE  LIBRARY_STAGE;
   #ifdef      orUniqueMultiMessage_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orUniqueMultiMessage...";#endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);

	default orUniqueMultiMessageBufferSize 500;        !--longest response consolidated, in characters
	array _orUmmCurrent buffer orUniqueMultiMessageBufferSize;   !--what the current object's action printed
	array _orUmmPrevious buffer orUniqueMultiMessageBufferSize;  !--what the previous object's action printed
	object orUniqueMultiMessage LibraryExtensions
		with currentResult _orUmmCurrent
		,	 previousResult _orUmmPrevious
		,	ext_playLoopPerformMultiObjectAction[
			i j k l count retval previous_obj severe_error;

			j = multiple_object-->0;
			if (j == 0) return 0; !--default

			util.orBuf.setLength(self.currentResult, 0);
			util.orBuf.setLength(self.previousResult, 0);

			previous_obj=0;
			multiflag = true;

			if (toomany_flag) {
				toomany_flag = false;
				L__M(##Miscellany, 1);
			}
			i = location;
			for (k=1 : k<=j : k++) {

				if (deadflag) break;

				if (location ~= i) {
					severe_error=true;
					break;
				}
				l = multiple_object-->k;
				PronounNotice(l);

				!print (name) l, (string) COLON__TX, " ";
				if (inp1 == 0) {
					inp1 = l;
					util.orBuf.capture(self.currentResult, orUniqueMultiMessageBufferSize);
					retval=InformLibrary.actor_act(actor, action, l, second);
					util.orBuf.release();
					switch (retval) {
						ACTOR_ACT_ABORT_NOTUNDERSTOOD:
							return self.release(1); ! begin action;
						ACTOR_ACT_ABORT_ORDER:
							return self.release(2); !--turn end
					}
					inp1 = 0;
				}
				else {
					inp2 = l;
					util.orBuf.capture(self.currentResult, orUniqueMultiMessageBufferSize);
					retval=InformLibrary.actor_act(actor, action, noun, l);
					util.orBuf.release();
					if(retval==ACTOR_ACT_ABORT_NOTUNDERSTOOD) return self.release(1); ! begin action;
					inp2 = 0;
				}

				if(identical(previous_obj,l)==false || util.orBuf.equals(self.currentResult, self.previousResult)==false){
					if(previous_obj~=0) PrintMultiAction(previous_obj,self.previousResult,count);
					util.orBuf.setFromBuffer(self.previousResult, self.currentResult);
					previous_obj=l;
					count=1;
				}else{
					count++;
				}

			}
			PrintMultiAction(previous_obj,self.previousResult,count);
			if(severe_error==true) L__M(##Miscellany, 51);

			return self.release(2); ; !continue...
		],
		release[rv;
			return rv;
		]
	;
	[PrintMultiAction obj str count;
		if(count>1)print count," ";
		if(count>1 && obj provides plural)
			print (string)obj.plural;
		else
			print (name)obj;
		print " : ";
		util.orBuf.print(str);
	];
#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
