!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.06.19 orCenter
! Centers text (with wordwrap)
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!--------------------------------------------------------------------------------------
! License: Public Domain.
!--------------------------------------------------------------------------------------
! orCenter() centers text on the screen, applying wordwrap as appropriate.  By default,
! The interpreter's current screen width is used for calculations; however, this can 
! be overriden by specifying the maxwidth parameter.  The following example centers 
! the specified text on the screen, breaking it up into lines which are no greater 
! than 20 characters in length:
!
!	orCenter("Fore score and seven years ago our forefathers brought forth on this continent, a new nation, conceived in Liberty, and dedicated to the proposition that all men are created equal.",20);
!
! orCenter also considers line breaks. For example:
!
!	orCenter("I wandered lonely as a cloud,^That floats on high o'er vales
!		   and hills,^When all at once I saw a crowd,^A host of golden
!		   daffodils,^Beside the lake, beneath the trees,^Fluttering and
!		   dancing in the breeze^^--William Wordsworth");
!
! Will center eight lines of text (including the blank line).
!
! The highlight parameter indicates the centered text should appear in reverse font,
! and how.  A value of HIGHLIGHT_TEXT_ONLY indicates the text should be highlighted,  
! a value of HIGHLIGHT_ALL indicates the text and its preceeding spaces should appear
! in reverse font.
!
! The leftAlign parameter, if true, produces a centered block of left-justified text.
!
! orInset() is a wrapper to the orCenter function which indents left-justified text, 
! offset with a specified margin on either side of the screen.
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
	#include "orUtilUi";
	#include "orUtilBuf";
	!TODO: use the orUI tools and perhaps orBuf for this code.
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
#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
