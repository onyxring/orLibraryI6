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
#ifndef        orCenter_STAGE;
Constant        orCenter_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
	#include "orUtilUi";
	#include "orUtilBuf";
	!TODO: use orBuf for this code.
!--------------------------------------------------------------------------------------
#endif;
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
#ifndef center_bgl; !--a Beguile game with the center extension (via bglBindings/orCenter) doesn't need it
	system_file;

	Array cursPos buffer 10;
	Global _orcWidth; Global _orcMaxWidth; Global _orcHighlight; Global _orcLeftAlign;
	Global _orcFrame; Global _orcSubIndent; Global _orcPadding;

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
		return orCenter(text, maxwidth, NO_HIGHLIGHT, true, 0, subIndent);
	];

	!	text 		- the text to be centered
	!	maxwidth	- the maximum width of a line; if not defined, then the screen width is used
	!	highlight	- how to print the text in reverse:
	!					NO_HIGHLIGHT (default) don't print any of the text in reverse 
	!					HIGHLIGHT_TEXT_ONLY print the text in reverse, but not the line of spaces leading up to the text
	!					HIGHLIGHT_ALL print the text text, and also the spaces preceding it
	!	leftAlign	- true to center a block of left-aligned text of maxwidth length
	!	prefixPostFixCallback - a callback routine called three times with a single paramter:
	!						0 - return the length in characters of the prefix and postfix combined (for centering calculations)
	!						1 - print the prefix
	!						2 - print the postfix
	!	subIndent	- indents the text by adding additional spaces to the left; primariliy used by the orInset routine
	[orCenter text maxwidth highlight leftAlign prefixPostFixCallback subIndent 
			padding width o c start lineEnd nextStart end len;

		orTempBuffer=util.orBuf.convertToSizedBuffer(orTempBuffer,8000);
		
		if(prefixPostFixCallback~=0) padding = prefixPostFixCallback(0)/2;
		font off; !fixed width
		width=util.orUI.getCurrentWidth();
		if(maxwidth==0 || maxwidth>width-2) maxwidth=width-2;	!keep the left-most and right-most column blank

			util.orBuf.capture(orTempBuffer);

			switch(metaclass(text)){
				string:
					print (string)text;		!print text in array
				routine:
					if(text()) maxwidth=width-2;
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
		_orcWidth=width; _orcMaxWidth=maxwidth; _orcHighlight=highlight; _orcLeftAlign=leftAlign;
		_orcFrame=prefixPostFixCallback; _orcSubIndent=subIndent; _orcPadding=padding;
		start=WORDSIZE;
		end=len+WORDSIZE;
		lineEnd=-1;
		for(o=start:o<=end:o++){
			if(o==end) c=10; else c=orTempBuffer->o;
			if(c==10 or 13){
				_orCenterLine(start, o);
				start=o+1;
				lineEnd=-1;
				continue;
			}
			if(o-start>=maxwidth){ !--character o doesn't fit on this line
				if(c==' '){
					_orCenterLine(start, o);
					start=o+1;
					lineEnd=-1;
					continue;
				}
				if(lineEnd>start){
					_orCenterLine(start, lineEnd);
					start=nextStart;
				}
				else{ !--a word longer than the line: split it
					_orCenterLine(start, o);
					start=o;
				}
				lineEnd=-1;
			}
			if(c==' '){ lineEnd=o; nextStart=o+1; }
			if(c=='?' or '/' or '-' or ':'){ lineEnd=o+1; nextStart=o+1; }
		}
		if(highlight>NO_HIGHLIGHT) style roman; !-- if we highlighted the text, turn it off
		font on; !restore variable-width font
	];

	[_orCenterLine start end
			offset i;
		if(_orcLeftAlign==false)
			offset=(_orcWidth-(end-start))/2;
		else
			offset=(_orcWidth-_orcMaxWidth)/2;
		if(start~=WORDSIZE) offset=offset+_orcSubIndent;

		if(_orcHighlight==HIGHLIGHT_ALL) style reverse;
		font off; !GLULX interpreters turn variable-width fonts back on when a style command is issued

		spaces offset-_orcPadding;

		if(_orcHighlight> NO_HIGHLIGHT) style reverse;
		font off; !GLULX interpreters turn variable-width fonts back on when a style command is issued

		if(_orcFrame~=0) _orcFrame(1);
		for(i=start:i<end:i++) print (char) orTempBuffer->i;
		if(_orcFrame~=0) _orcFrame(2);

		if(_orcHighlight~=HIGHLIGHT_ALL) style roman;
		font off; !GLULX interpreters turn variable-width fonts back on when a style command is issued

		new_line;
	];
#endif; !--center_bgl
#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
