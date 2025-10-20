
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.04.29 orKeyboardHook [Z, GLULX]
! An update to the standard library's Keyboard and KeyboardPrimitive routines.  No
! functionality has been altered but calls to LibraryExtensions handlers have been added.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License:
!--------------------------------------------------------------------------------------
! This extension, classified as a "hook", provides tooling for other extensions to
! use and provides no specific features by itself.
!
! The following Library Extension handlers have been added:
!
!		ext_keyboardPrimitive
!		ext_beforePrompt
!		ext_keyboardGetInput
!		ext_keyboardPrepInput
!		ext_keyboardHandleComment
!		ext_keyboardHandleUndo
!		ext_keyboardHandleOops
!--------------------------------------------------------------------------------------
! Revision History
! 2002.04.05	Initial Creation
! ...			History from orLibrary v1 snipped away.
! 2024.04.29	Refactored for the orLibrary version 2.
!======================================================================================
! INCLUDE DEPENDENCIES

!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orHookKeyboard_STAGE  0; #ifnot;
#ifndef        orHookKeyboard_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orHookKeyboard_STAGE  < LIBRARY_STAGE);
   #undef      orHookKeyboard_STAGE  ;
   Constant    orHookKeyboard_STAGE  LIBRARY_STAGE;
   #ifdef      orHookKeyboard_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orHookKeyboard..."; #endif;
!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);
replace keyboard;
replace KeyboardPrimitive _oldKeyboardPrimitive;

property individual ext_keyboardPrimitive;
property individual ext_keyboardGetInput;
property individual ext_beforePrompt;
property individual ext_keyboardPrepInput;
property individual ext_keyboardHandleComment;
property individual ext_keyboardHandleUndo;
property individual ext_keyboardHandleOops;
#endif; !--Before Parser
!======================================================================================
! AFTER PARSER
#iftrue (LIBRARY_STAGE == AFTER_PARSER);

	[ KeyboardPrimitive  a_buffer a_table retval;
		retval = LibraryExtensions.RunWhile(ext_keyboardPrimitive, false, a_buffer, a_table);
        if(retval==false) _oldKeyboardPrimitive(a_buffer, a_table);
		return retval;
	];

	[ Keyboard  a_buffer a_table  nw i w w2 x1 x2 retval;
		DisplayStatus();

	.FreshInput;
		retval = LibraryExtensions.RunWhile(ext_keyboardGetInput, false, a_buffer, a_table);
        if(retval == -1) jump FreshInput;
		if(retval == false){
			for (i=0 : i<OOPS_WORKSPACE_LEN : i++) oops_workspace->i = a_buffer->i;

			#Ifdef TARGET_ZCODE;
			a_buffer->0 = INPUT_BUFFER_LEN - WORDSIZE;
			a_table->0  = MAX_BUFFER_WORDS; ! Allow to split input into this many words
			#Endif; ! TARGET_


			retval=LibraryExtensions.RunWhile(ext_beforePrompt, false);
			if(retval==false){!  Print the prompt, and read in the words and dictionary addresses
				L__M(##Prompt);
			}
			if (AfterPrompt() == 0) LibraryExtensions.RunAll(ext_afterprompt);

			#IfV5;
			DrawStatusLine();
			#Endif; ! V5
			if(KeyboardPrimitive(a_buffer, a_table)==-1) jump FreshInput;
			nw = NumberWords(a_table);
		}

		retval = LibraryExtensions.RunWhile(ext_keyboardPrepInput, false, a_buffer, a_table); !--return true to interrupt standard library's default behavior
        if(retval == -1) jump FreshInput;
		if(retval == false){
			if (nw == 0){
				L__M(##Miscellany,10);
				jump FreshInput;
			}
		}

		retval = LibraryExtensions.RunWhile(ext_keyboardHandleComment, false, a_buffer, a_table); !--return true to interrupt standard library's default behavior
        if(retval == -1) jump FreshInput;
		if(retval == false){
			if (a_buffer->WORDSIZE == COMMENT_CHARACTER) {
				#Ifdef TARGET_ZCODE;
				if ((HDR_GAMEFLAGS-->0) & $0001 || xcommsdir)
												L__M(##Miscellany, 54);
				else                               L__M(##Miscellany, 55);
				#Ifnot; ! TARGET_GLULX
				if (gg_scriptstr || gg_commandstr) L__M(##Miscellany, 54);
				else                               L__M(##Miscellany, 55);
				#Endif; ! TARGET_

				jump FreshInput;
			}
		}
		retval = LibraryExtensions.RunWhile(ext_keyboardHandleUndo, false, a_buffer, a_table); !--return true to interrupt standard library's default behavior
        if(retval == -1) jump FreshInput;
		if(retval == false){
			#IFV5;	!  Undo handling
				w=a_table-->1;
				if ((w == UNDO1__WD or UNDO2__WD or UNDO3__WD) && (nw==1)){
					i=PerformUndo();
					if(i==0) jump FreshInput;
				}
			#Ifdef TARGET_ZCODE;
				@save_undo i;
			#Ifnot; ! TARGET_GLULX
				@saveundo i;
				if (i == -1) {
					GGRecoverObjects();
					i = 2;
				}
				else i = (~~i);
			#endif; ! TARGET_

				just_undone=0;
				undo_flag=2;
				if (i==-1) undo_flag=0;
				if (i==0) undo_flag=1;
				if (i==2)
				{
					RestoreColours();
					#Ifdef TARGET_ZCODE;
					style bold;
					#Ifnot; ! TARGET_GLULX
					glk_set_style(style_Subheader);
					#Endif; ! TARGET_
					print (name) location, "^";
					#Ifdef TARGET_ZCODE;
					style roman;
					#Ifnot; ! TARGET_GLULX
					glk_set_style(style_Normal);
					#Endif; ! TARGET_
					L__M(##Miscellany, 13);
					just_undone = 1;
					jump FreshInput;
				}
			#ENDIF; !V5
			return nw;
		}
.DoOops;
		retval = LibraryExtensions.RunWhile(ext_keyboardHandleOops, false, a_buffer, a_table); !--return true to interrupt standard library's default behavior
        if(retval == -1) jump FreshInput;
		if(retval == false){
			if (oops_from == 0) {
				L__M(##Miscellany, 14);
				jump FreshInput;
			}
			if (nw == 1) {
				L__M(##Miscellany, 15);
				jump FreshInput;
			}
			if (nw > 2) {
				L__M(##Miscellany, 16);
				jump FreshInput;
			}

			! So now we know: there was a previous mistake, and the player has
			! attempted to correct a single word of it.

			for (i=0 : i<INPUT_BUFFER_LEN : i++) buffer2->i = a_buffer->i;
			#Ifdef TARGET_ZCODE;
			x1 = a_table->9;  ! Start of word following "oops"
			x2 = a_table->8;  ! Length of word following "oops"
			#Ifnot; ! TARGET_GLULX
			x1 = a_table-->6; ! Start of word following "oops"
			x2 = a_table-->5; ! Length of word following "oops"
			#Endif; ! TARGET_

			! Repair the buffer to the text that was in it before the "oops"
			! was typed:

			for (i=0 : i < OOPS_WORKSPACE_LEN : i++) a_buffer->i = oops_workspace->i;
			Tokenise__(a_buffer, a_table);

			! Work out the position in the buffer of the word to be corrected:

			#Ifdef TARGET_ZCODE;
			w = a_table->(4*oops_from + 1); ! Start of word to go
			w2 = a_table->(4*oops_from);    ! Length of word to go
			#Ifnot; ! TARGET_GLULX
			w = a_table-->(3*oops_from);      ! Start of word to go
			w2 = a_table-->(3*oops_from - 1); ! Length of word to go
			#Endif; ! TARGET_

		#Ifdef OOPS_CHECK;
			print "[~";
			for (i=0 : i<w2 : i++) for (i=0 : i<w2 : i++) print (char)a_buffer->(i+w);
			print "~ --> ~";
		#Endif;

			! Write spaces over the word to be corrected:

			for (i=0 : i<w2 : i++) a_buffer->(i+w) = ' ';

			if (w2 < x2) {
				! If the replacement is longer than the original, move up...
				for (i=INPUT_BUFFER_LEN-1 : i>=w+x2 : i--)
					a_buffer->i = a_buffer->(i-x2+w2);

				! ...increasing buffer size accordingly.
				SetKeyBufLength(GetKeyBufLength(a_buffer) + (x2-w2), a_buffer);
			}

			! Write the correction in:

			for (i=0 : i<x2 : i++) {
				a_buffer->(i+w) = buffer2->(i+x1);
		#Ifdef OOPS_CHECK;
				print (char) buffer2->(i+x1);
		#Endif;
			}

		#Ifdef OOPS_CHECK;
				print "~]^^";
		#Endif;

			Tokenise__(a_buffer, a_table);
			nw=NumberWords(a_table);

			return nw;
		}
	];


#endif; !--After Parser
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
