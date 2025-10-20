!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.07.14 orUI
! A utility object for interacting with the Interpreter's User Interface.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! This extension, classified as a "utility", provides tooling for other extensions to
! use and provides no specific features by itself.
!
! It provides functions wrapped into an object.  Specifically:
!	orUI, which contains functions to interacting with the interpreter's UI.
!
!--------------------------------------------------------------------------------------
! Revision History
! 2024.07.14	Initial Creation
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orUtilUI_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUtil";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orUtilUI_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orUtilUI_STAGE  < LIBRARY_STAGE);
   #undef      orUtilUI_STAGE  ;
   Constant    orUtilUI_STAGE  LIBRARY_STAGE;
   #ifdef      orUtilUI_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orUtilUI...";#endif;
!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == AFTER_PARSER);
   constant ORUI_MAIN	99;
	constant ORUI_STATUS	98;

   object   _orUI
     with   getScreenWidth[w;  !--mode is not relevant for Z-Machine
         if(w==0) w=ORUI_STATUS;

         #ifdef TARGET_ZCODE;
            return ScreenWidth();
         #ifnot;
            switch(w){
               ORUI_MAIN:
                  glk_window_get_size(gg_mainwin, gg_arguments, 0);
               ORUI_STATUS:
                  glk_window_get_size(gg_statuswin, gg_arguments, 0);
               default:
                  glk_window_get_size(w, gg_arguments, 0);
            }
            return gg_arguments-->0;
         #endif;
     ]
		,	   getScreenHeight[; 
               #ifdef TARGET_ZCODE; 
                  return ScreenHeight(); 
               #ifnot;
                  return ScreenHeight() + __getStatusHeight(); 
               #endif;
            ]
      ,	   getStatusHeight[; return __getStatusHeight();]
      ,	   setStatusHeight[h;
               StatusLineHeight(h);
               #ifdef TARGET_ZCODE; @split_window h; #endif;
            ]
      ,	   activateMain[; self.activateWindow(ORUI_MAIN); ]
      ,	   activateStatus[; self.activateWindow(ORUI_STATUS); ]
      ,	   activateWindow[w;!--set the current window
               #ifdef TARGET_GLULX;
                  switch(w){
                     ORUI_MAIN:
                        glk_set_window(gg_mainwin);
                     ORUI_STATUS:
                        glk_set_window(gg_statuswin);
                     default:
                        glk_set_window(w);
                  }
               #ifnot;
                  switch(w){
                     ORUI_MAIN:
                        @set_window 0;
                     ORUI_STATUS:
                        @set_window 1;
                     default:
                        @set_window(w);
                  }
               #endif;
			]
         , getChar[; return KeyCharPrimitive(); ]
         , eraseWindow[w;
            #ifdef TARGET_ZCODE;
               switch(w){
                  ORUI_MAIN:
					      @erase_window 0;
                  ORUI_STATUS:
                     @erase_window 1;
                  default:
                     @erase_window w;
               }
               !@set_cursor 1 1;
				#ifnot;
               if(w==0) w=ORUI_MAIN;
               switch(w){
                  ORUI_MAIN:
					      glk_window_clear(gg_mainwin);
                  ORUI_STATUS:
                     glk_window_clear(gg_statuswin);
                  default:
                     glk_window_clear(w);
               }
					!glk_window_move_cursor(gg_mainwin,0,0);
				#endif;
         ]
         ,  eraseMain[; self.eraseWindow(ORUI_MAIN); ]
         ,  eraseStatus[; self.eraseWindow(ORUI_STATUS); ]
         ,  eraseScreen[;
               #ifdef TARGET_ZCODE;
                  @erase_window -1;
               #ifnot;
               #endif;
               !self.position(0,0);
         ]
         ,  position[x y;
               !util.orUI.activateStatus();
               #ifdef TARGET_ZCODE;
                  y=y+1; x=x+1;
                  @set_cursor y x;
               #ifnot;
                  glk_window_move_cursor(gg_statuswin,x,y);
               #endif;
            ]
         ,  hideCursor[;
               self.activateStatus();
               self.position(self.getScreenWidth()-1, self.getStatusHeight()-1);
            ]
         ,  pauseForInput[;
               util.orUI.hideCursor(); !hide the blinking cursor
               util.orUI.getChar();
               util.orUI.activateMain(); !restore it to the main screen
            ]
   ;

#endif; !--BEFORE Parser
#iftrue (LIBRARY_STAGE == AFTER_PARSER);
   [__getStatusHeight; return gg_statuswin_cursize; ];
#endif; !--AFTER Parser
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE

