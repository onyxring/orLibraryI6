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
#ifndef        orUtilUI_STAGE ;
default        orUtilUI_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orUtil";
!--------------------------------------------------------------------------------------
#endif;
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
   global winMain	   0;  !--note: orUtilUiGlulx extension turns these into objects. 
	global winStatus	1;

   object   _orUI
     with getStatusWidth[w;    
      #ifdef orUtilUiGlulx_STAGE;
            return util.orUi.glulx.getStatusTextWidth();
      #ifnot;
            return util.orUi.getScreenWidth();
      #endif;         
         ]
   ,   getScreenWidth[w; 
   #ifdef orUtilUiGlulx_STAGE;
         return util.orUi.glulx.getScreenTextWidth();
   #endif;
         if(w==0) w=winStatus;

         #ifdef TARGET_ZCODE;
            return ScreenWidth();
         #ifnot;
            if(w==winMain) glk_window_get_size(gg_mainwin, gg_arguments, 0);
            else if(w==winStatus) glk_window_get_size(gg_statuswin, gg_arguments, 0);
            else glk_window_get_size(w, gg_arguments, 0);
            
            return gg_arguments-->0;
         #endif;
      ]
	   ,  getScreenHeight[; 
            #ifdef TARGET_ZCODE; 
               return ScreenHeight(); 
            #ifnot;
               return ScreenHeight() + __getStatusHeight(); 
            #endif;
         ]
      
      ,	getStatusHeight[; return __getStatusHeight();]
      ,	setStatusHeight[h;
            StatusLineHeight(h);
            #ifdef TARGET_ZCODE; @split_window h; #endif;
         ]
      ,	   activateMain[; self.activateWindow(winMain); ]
      ,	   activateStatus[; self.activateWindow(winStatus); ]
      ,	   activateWindow[w;!--set the current window
               #ifdef TARGET_GLULX;
                  if(w==winMain) glk_set_window(gg_mainwin);
                  else if(w==winStatus) glk_set_window(gg_statuswin);
                  else glk_set_window(w);
               #ifnot;
                  if(w==winMain) @set_window 0;
                  else if(w==winStatus) @set_window 1;
                  else @set_window(w);
               #endif;
			]
         , getChar[; return KeyCharPrimitive(); ]
         , eraseWindow[w;
            #ifdef TARGET_ZCODE;
                  if(w==winMain)    @erase_window 0;
                  else if(w==winStatus)    @erase_window 1;
                  else    @erase_window w;
				#ifnot;
               if(w==0) w=winMain;
               if(w==winMain) glk_window_clear(gg_mainwin);
               else if(w==winStatus) glk_window_clear(gg_statuswin);
               else glk_window_clear(w);
				#endif;
         ]
         ,  eraseMain[; self.eraseWindow(winMain); ]
         ,  eraseStatus[; self.eraseWindow(winStatus); ]
         ,  eraseScreen[;
               #ifdef TARGET_ZCODE;
                  @erase_window -1;
               #ifnot; !TODO: where's the Glulx code?
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
         , glulx 0
         , pauseForInput[;
               util.orUI.hideCursor(); !hide the blinking cursor
               util.orUI.getChar();
               util.orUI.activateMain(); !restore it to the main screen
            ]
      
      ;

#endif; !--BEFORE Parser
#iftrue (LIBRARY_STAGE == AFTER_PARSER);
   [__getStatusHeight; return gg_statuswin_cursize; ];!so status height can be used by routines declared before parser
#endif; !--AFTER Parser
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE

