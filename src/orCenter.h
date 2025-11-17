!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.06.19 orCenter
! Centers text (with wordwrap)
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!--------------------------------------------------------------------------------------
! License:
!--------------------------------------------------------------------------------------
! orCenter() is a function that works similar to  print except that text is centered
! on the screen.  Wordwrap also occurs as it does with print, but for centered text, it
! is generally more attractive to limit the width of a line.  This can be accomplished
! by specifying an additional value representing the maximum width of a line.  If the
! width is either not specified or is specified to be larger than the screen, then the
! screen width is used instead.
!
! It should also be noted that Center takes line breaks into account. For example:
!
!	orCenter("I wandered lonely as a cloud,^That floats on high o'er vales
!		   and hills,^When all at once I saw a crowd,^A host of golden
!		   daffodils,^Beside the lake, beneath the trees,^Fluttering and
!		   dancing in the breeze^^--William Wordsworth");
!
! Will acurately center eight lines of text (including the blank line).
!
! For less poetic centering, the second parameter, width, can be specified:
!
!	orCenter("Fore score and seven years ago our forefathers brought forth on this continent, a new nation, conceived in Liberty, and dedicated to the proposition that all men are created equal.",20);
!
! Will center the specified text on the screen, breaking it up in intervals that
! are no greater than 20 characters in length.
!
! The final parameter is highlight.  Pass this parameter as a one to get the centered
! text to appear in reverse font.  Pass a two to this parameter to get the text AND ITS
! PRECEEDING SPACES to appear in reverse font.
!
! orInset() is a wrapper to the orCenter function which centers a block of text, but not
! left aligns the text within the block itself.  It takes the text, plus the max width
! of the text block.
!--------------------------------------------------------------------------------------
! Revision History
! 2000.08.03	Initial Creation (before OrLib)
! 2001.04.01	First release in (in ORLibrary)
! ...			History from orLibrary v1 snipped away.
! 2024.06.19	Refactored for the orLibrary version 2; added orInset() function
! 2024.12.28	Added sub-indent functionality to inset additional lines more than the
!				first line.
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orCenter_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
!TODO: orStyleHint for GLULX
	#include "orUtilUi";
	#include "orUtilBuf";
!TODO: use the orUI tools and perhaps orBuf tool for this code.
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orCenter_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orCenter_STAGE  < LIBRARY_STAGE);
   #undef      orCenter_STAGE  ;
   Constant    orCenter_STAGE  LIBRARY_STAGE;
   #ifdef      orCenter_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orCenter..."; #endif;
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
	system_file;

	Array cursPos buffer 10;

	#ifndef orTempBuffer;
		array _orTmpBuffer8000 buffer 8000;
		Global orTempBuffer _orTmpBuffer8000;
	#endif; !orTempBuffer is used throughout the library
	Constant NO_HIGHLIGHT 0;
	Constant HIGHLIGHT_TEXT_ONLY 1;
	Constant HIGHLIGHT_ALL 2;


	[orInset text inset subIndent
			maxwidth;
		if(inset==0) inset=8;
		maxWidth=util.orUI.getScreenWidth()-(inset*2);
		return orCenter(text, maxwidth, NO_HIGHLIGHT, true, subIndent);
	];

	[orCenter text maxwidth highlight leftAlign subIndent
			width markerstart markerend o i offset sv len svt includeBreakingChar;

		orTempBuffer=util.orBuf.convertToSizedBuffer(orTempBuffer,8000);
		svt=0; sv=0; !--suppress compiler warnings...
		font off; !fixed width
		width=util.orUI.getScreenWidth();
		width=width-2;							!subtract a little from the width (keep the left-most and right-most column blank)
		if(maxwidth==0 || maxwidth>width) maxwidth=width;		!default maxwidth of line to the width of the display if it is not defined or too large to display

		util.orBuf.capture(orTempBuffer);

			switch(metaclass(text)){
				string:
					print (string)text;		!print text in array
				routine:
					if(text()) maxwidth=width;
				object:
#ifdef orString;

					if(text ofclass orString)
						text.print();
					else
						print (name)text;
#ifnot;
						print (name)text;
#endif;

			}

		util.orBuf.release();

		len=util.orBuf.getLength(orTempBuffer);

		for(markerend=markerstart=o=WORDSIZE:o<=(len+WORDSIZE):o++){
			if(orTempBuffer->o=='?' or '/' or '-' or ':' or ' ' or 10 or 13 ) markerend=o;					!adjust pointer to end-of-line in increments of whole words
			if(orTempBuffer->o==10 or 13 || o==(len+WORDSIZE)|| o-markerstart>maxwidth)			!dump the captured line if 1)found carriage return, 2)finished parsing input, or 3) reached max size of screen
			{
				if(orTempBuffer->markerend == '?' or '/' or '-' or ':') { !cases where we allow breaks and still want to print the character
					includeBreakingChar=true;
					markerend=markerend+1;
				}
				else includeBreakingChar=false;

				if(o==(len+WORDSIZE)) markerend=o;				!--if we have finished parsing the input, then the character pointed to by o is not necessarily a space or return.  Go ahead and include it in the output
				if(leftAlign==false)
					offset=(width/2)-((markerend-markerstart)/2);	!--GLULX note: These /2 are correct.  They do NOT represent WORDSIZE, but calculate the half way point
				else
					offset=(util.orUI.getScreenWidth()-maxwidth)/4; !TODO not sure about this formula, but if the /4 is set to /2 like above, it creates wordwrap issues

				if(highlight==HIGHLIGHT_ALL) style reverse;
				font off; !GLULX interpreters turn variable-width fonts back on when a style command is issued

				if(markerstart==WORDSIZE)
					spaces offset;
				else
					spaces offset+subIndent;

				if(highlight> NO_HIGHLIGHT) style reverse;
				font off; !GLULX interpreters turn variable-width fonts back on when a style command is issued

				for(i=markerstart:i<markerend:i++) print (char) orTempBuffer->i;	!print every character from startmark to just prior to endmark (so as not to print ending space or carriage return)

				if(highlight~=HIGHLIGHT_ALL) style roman;
				font off; !GLULX interpreters turn variable-width fonts back on when a style command is issued

				new_line;												!next line

				if(includeBreakingChar==false)
					markerend=markerstart=markerend+1;							!skip the return or the space for the next run (so that it doesn't get appended to the front of the next lfine)
				else
					markerend=markerstart=markerend;
			}
		}
		if(highlight>NO_HIGHLIGHT) style roman; !-- if we highlighted the text, turn it off
		font on; !restore variable-width font
	];
! #ifdef NTARGET_GLULX;
! 	default style_Center		style_User1;
! 	default style_CenterRvrs	style_User2;

! 	ORStyleHint	with user_num style_Center
! 		,	windows wintype_TextBuffer
! 		,	hints stylehint_Proportional stylehint_Justification
! 		,	values 0 stylehint_just_Centered
! 	;
! 	ORStyleHint	with user_num style_CenterRvrs
! 		,	windows wintype_TextBuffer
! 		,	hints stylehint_ReverseColor stylehint_Proportional stylehint_Justification
! 		,	values 1 0 stylehint_just_Centered
! 	;
! 	[set_centered; glk_set_style(style_Center);];
! 	[set_highlight_centered; glk_set_style(style_CenterRvrs);];
!#endif;!GLULX
#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
