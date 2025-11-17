!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.12.28 orStatusLine
! An alternative implementation of DrawStatusLine which provides extended functionality
! to the game designer.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! Replaces the standard library's status line functionality, introducing three
! routines/values which can be overridden:
!
!	orStatusLeft - typically printing the name of the room.  This text is aligned left
!					in the status bar.
!	orStatusMiddle - text printed in the middle of the status bar.
!	orStatusRight - text aligned to the right of the status.
!
! By default, the extension behaves almost exactly like the default behavior, but the
! above routines/strings can be defined before Verblib to override the defaults.
!--------------------------------------------------------------------------------------
! Revision History
! 2025.01.31	Initial Creation
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orStatusLine_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "orString";
#include "orCenter";
#include "orUtilRef";
#include "orUtilUi";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orStatusLine_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orStatusLine_STAGE  < LIBRARY_STAGE);
   #undef      orStatusLine_STAGE  ;
   Constant    orStatusLine_STAGE  LIBRARY_STAGE;
   #ifdef      orStatusLine_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orStatusLine...";#endif;
!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);
	replace DrawStatusLine;

#endif; !--Before Parser
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_GRAMMAR);
#ifndef orStatusLeft;
[ orStatusLeft;
	if (location == thedark) {
        print (name) location;
    }
    else {
        FindVisibilityLevels();
        if (visibility_ceiling == location)
            print (name) location;
        else
            print (The) visibility_ceiling;
    }
];
#endif;
#ifndef orStatusRight;
	[ orStatusRight
			width;
		width=util.orUI.getScreenWidth();

		if (sys_statusline_flag) {
			if (width > 29) { !this mimics the standard library logic.  Strange that it prints NOTHING in time mode when less then 30 character screen width.
				print (string) TIME__TX;
				LanguageTimeOfDay(sline1, sline2);
			}
			return;
		}
	#ifdef NO_SCORE;
		print (string) MOVES__TX, sline2;
	#ifnot;
		if(width>66){ !big screen
			print (string) SCORE__TX, sline1," ",(string) MOVES__TX, sline2;
		}else{
			print sline1, "/", sline2;
		}
	#endif;
	];
#endif;
#ifndef orStatusMiddle;
	[ orStatusMiddle; ]; !the standard library doesn't do centered text by default, so we mimic this by also doing nothing.
#endif;
	[ DrawStatusLine
			strL  strM  strR ;

    #Ifdef TARGET_GLULX;
        if (gg_statuswin == 0) rfalse;
    #Endif;
		if (location == nothing || parent(player) == nothing) rfalse;


		!create our working strings
		strL=util.orStr.new();
		strM=util.orStr.new();
		strR=util.orStr.new();

		!capture our aligned text in each
		strL.captureOutput(orStatusLeft);
		strM.captureOutput(orStatusMiddle);
		strR.captureOutput(orStatusRight);

		!Now lets draw the status line...
		util.orUI.activateStatus();
		util.orUI.setStatusHeight(1);

		style reverse;
		font off;

		util.orUI.position(0,0);
		spaces(util.orUI.getScreenWidth());
		!util.orUI.eraseStatus();

		!drawing right to left, first we align the Right string, based on the width of the screen, minus the length of our string
		util.orUI.position(util.orUi.getScreenWidth()-strR.getLength(),0);
		strR.print();

		!reposition our cursor to the start and Center the "Middle" aligned text (this does NOT overwrite the right most text)
		util.orUI.position(0,0);
		orCenter(strM, 0, HIGHLIGHT_ALL);
		style reverse; !the above line turns off reverse, so lets turn it back on
		font off;

		!reposition our cursor to the start and Center the "Left" aligned text (this does NOT overwrite the middle or right text)
		util.orUI.position(0,0);
		strL.print();

		!leave the status bar, going back to the main window
		util.orUI.activateMain();

		!clean up our allocated memory
		strL.free();
		strM.free();
		strR.free();
		style roman;
		font on;
		return;

	];
#endif; !--AFTER_VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE