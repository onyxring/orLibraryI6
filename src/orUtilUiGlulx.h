!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2026.02.01 orUtilUiGlulx
! Provides UI-centric utilities specific to Glulx.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! Revision History
! 2026.02.01	Initial Creation
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
#ifndef        orUtilUiGlulx_STAGE;
default        orUtilUiGlulx_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "orUtilUi";
!--------------------------------------------------------------------------------------
#endif;
#ifnot;
#ifndef        orUtilUiGlulx_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orUtilUiGlulx_STAGE  < LIBRARY_STAGE);
   #undef      orUtilUiGlulx_STAGE  ;
   Constant    orUtilUiGlulx_STAGE  LIBRARY_STAGE;
   #ifdef      orUtilUiGlulx_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orUtilUiGlulx...";#endif;
   #ifndef  TARGET_GLULX; message fatalerror "orUtilUiGlulx: This extension is specific to the Glulx platform and will not compile for Z-Code."; #endif;
!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);

   constant uiAboveLine imagealign_InlineUp;
   constant uiBelowLine imagealign_InlineDown;
   constant uiMiddleOfLine imagealign_InlineCenter;
   constant uiLeft imagealign_MarginLeft;
   constant uiRight imagealign_MarginRight;
   constant uiCentered 6; !--calculated, floating in the middle with no text on either side
   
   constant uiGrid wintype_TextGrid;
   constant uiText wintype_TextBuffer;
   constant uiGraphics wintype_Graphics;
   
   constant uiFixed winmethod_Fixed;
   constant uiScaled winmethod_Proportional;
   
   constant uiOriginalSize -1;
   
   object _tmpSizeStruct with width, height;
   constant GLULX_WINDOWS_MAX=10;
   
#endif; !--Before Parser
!======================================================================================
! AFTER PARSER
#iftrue (LIBRARY_STAGE == AFTER_PARSER);
   [ InitGlkWindow; rfalse;]; 
   class _glulxWindow(GLULX_WINDOWS_MAX)
      with  id 0
      ,  arrangement 0
      ,  create[;
            self.supports=_glulxWindowSupport.create(self);
         ]
      ,  splitLeft[type width scale 
               retval;
            if(scale==0) scale=uiFixed;
            retval=_glulxWindow.create();
            retval.arrangement=winmethod_Left | scale | winmethod_NoBorder;
            retval.id=util.orUi.glulx._splitWindow(self.id, retval.arrangement, width, type);
            return retval;
         ]
      ,  splitRight[type width scale 
               retval;
            if(scale==0) scale=uiFixed;
            retval=_glulxWindow.create();
            retval.arrangement=winmethod_Right | scale | winmethod_NoBorder;
            retval.id=util.orUi.glulx._splitWindow(self.id, retval.arrangement, width, type);
            return retval;
         ]
      ,  splitUp[type height scale 
               retval;
            if(scale==0) scale=uiFixed;
            retval=_glulxWindow.create();
            retval.arrangement=winmethod_Above | scale | winmethod_NoBorder;
            retval.id=util.orUi.glulx._splitWindow(self.id, retval.arrangement, height, type);
            return retval;
         ]
      ,  splitDown[type height scale 
               retval;
            if(scale==0) scale=uiFixed;
            retval=_glulxWindow.create();
            retval.arrangement=winmethod_Below | scale | winmethod_NoBorder;
            retval.id=util.orUi.glulx._splitWindow(self.id, retval.arrangement, height, type);
            return retval;
         ]
      ,  drawImage[imgId alignment w h; 
            util.orUi.glulx._drawImage(self.id, imgId, alignment, w, h);
         ]
      ,  getSize[retval;
            glk_window_get_size(self.id, gg_arguments, gg_arguments+WORDSIZE);
            retval=_tmpSizeStruct;
            retval.width=gg_arguments-->0;
            retval.height=gg_arguments-->1;      
            return retval; 
         ]
      ,  close[;
            glk_window_close(self.id, 0); 
            _glulxWindowSupport.destroy(self.supports);
            _glulxWindow.destroy(self);
         ]
      ,  supports 0
      ,  resize[dim 
               p v;
             !void glk_window_get_arrangement(winid_t win, glui32 *methodptr, glui32 *sizeptr, winid_t *keywinptr);
             p=glk_window_get_parent(self.id);
             glk_window_set_arrangement(p, self.arrangement, dim, self.id);
          ]
   ;
   
   class _glulxWindowSupport(GLULX_WINDOWS_MAX)
      with  create[win; self.attachedWindow=win;]
      ,     attachedWindow 0
      ,     mouseInput[;return glk_gestalt(gestalt_MouseInput, self.attachedWindow.id);]
      ,     drawImage[;return glk_gestalt(gestalt_DrawImage, self.attachedWindow.id);]
      ,     hyperlinkInput[;return glk_gestalt(gestalt_HyperlinkInput, self.attachedWindow.id);]
   ;
 
object   _orUIGlulx LibraryExtensions
      private 
      with _screenTextWidthSizer 0 !which may or may not be different than the statusbar width, depending on if the status bar has been split
      ,   _screenPixelWidthSizer 0
      ,   _screenPixelHeightSizer 0
      ,   _drawImage[win img alignment w h 
      a b c
            imgInfo i; 
            if(alignment==0) alignment=uiCentered;
            
            imgInfo=self.getImageSize(img);
            if(imgInfo==0) "[ERROR:orUI unable to determine image dimentions.]";
            
            if(h==0 or uiScaled && w==0 or uiScaled) w=h=uiOriginalSize; !--both dimentions are undefined, so no way to keep these proportional
            if(w~=0 or uiScaled && h==0 or uiScaled) h=(imgInfo.height * w) /imgInfo.width;
            if(h~=0 or uiScaled && w==0 or uiScaled) w=(imgInfo.width * h) / imgInfo.height;
            
            if(h==uiOriginalSize) h=imgInfo.height;
            if(w==uiOriginalSize) w=imgInfo.width;

            if(alignment==uiCentered){ !--in this mode, we believe we are print to a text window, so lets center it by printing fixed-width spaces...
               
               !i=(self.getScreenTextWidth()/2)-(w/self.getMonoFontPixelWidth());
               
               !i=((self.getScreenPixelWidth()*100 - (w*100))/2)/self.getMonoFontPixelWidth(100); !we multiply these calcs by 100 to reduce the rounding errors in calculatins, since Inform doesn't support decimal values

               i=(self.getScreenTextWidth() - self.convertPixelWidthToTextWidth(w))/2;
               style fixed;
               new_line;
               spaces(i);
               style roman;
               alignment==uiMiddleOfLine;
            }
            glk_image_draw_scaled(win,img,alignment,0,w,h);               
         ]
      ,  _splitWindow[win method size type
               retval;
            retval=glk_window_open(win, method, size, type, 0);
            return retval;
         ]
      ,  getImageSize[img 
               retval;
            retval=glk_image_get_info(img, gg_arguments, gg_arguments+WORDSIZE);
            if(retval==true) {
               retval=_tmpSizeStruct;
               retval.width=gg_arguments-->0;
               retval.height=gg_arguments-->1;
            }
            return retval; 
         ]
      ,	convertPixelWidthToTextWidth[w; return ((w*100)+50)/(self.getMonoFontPixelWidth(100)); ]
      ,	getScreenPixelHeight[; return self._screenPixelHeightSizer.getSize().height; ]
      ,	getScreenPixelWidth[; return self._screenPixelWidthSizer.getSize().width; ]
      ,	getScreenTextWidth[; return self._screenTextWidthSizer.getSize().width; ]
      ,  getStatusTextWidth[; return winStatus.getSize().width; ]
      ,	getMonoFontPixelWidth[res; 
            if(res==0) res=1; !res is used to do the math at a higher order of magnitude to avoid truncating decimal places
            return ((self._screenPixelWidthSizer.getSize().width*res)/ (self.getScreenTextWidth())); 
         ]
      ,	getMonoFontPixelHeight[n; return self._screenPixelHeightSizer.getSize().height/util.orUi.getScreenHeight(); ]
      ,  supports _glulxSupport
      
      ,  ext_InitGlkWindow[rck; !--called multiple times by the library...
            if(rck==GG_STATUSWIN_ROCK){ !--the library has just initialized the main window which is the entire screen, but has not yet initialized the status bar
               _orUi.glulx=self; !install this into the orUi object, where it belongs
            
               winMain=_glulxWindow.create(); !--associate the winMin variable, defined in orUtilUi, as an actual, uable object, rather than an enumerated value.  The effect is the same for switch statements, but allows us to use the variable in other ways
                  winMain.id=gg_mainwin;
            
               !--now we create zero width windows to allow us to get sizes which aren't normally available to us...
               self._screenPixelWidthSizer=winMain.splitUp(uiGraphics,0); !--the width of the screen in pixels
               self._screenPixelHeightSizer=winMain.splitLeft(uiGraphics,0); !--the height of the screen in pixels
               self._screenTextWidthSizer=winMain.splitUp(uiText,0); !--the width of the screen in fixed width font (for z-code, this is the same as the width of the status bar, but we can split windows which means this isn't always the case)
               
            }
            if(rck==1){ !--the last call by the parser
               winStatus=_glulxWindow.create(); !--expand the winStatus variable just like we did winMin in the last pass
                  winStatus.id=gg_statuswin;
            }
           rfalse;
      ]
   ;
     object _glulxSupport 
      with version[;return glk_gestalt(gestalt_Version, 0);]
      ,     timer [;return glk_gestalt(gestalt_Timer, 0); ]
      ,     graphics [;return glk_gestalt(gestalt_Graphics, 0);]
      ,     graphicsTransparency [;return glk_gestalt(gestalt_GraphicsTransparency, 0); ]
      ,     sound [;return glk_gestalt(gestalt_Sound, 0); ]
      ,     soundMusic [;return glk_gestalt(gestalt_SoundMusic, 0);]
      ,     soundVolume [;return glk_gestalt(gestalt_SoundVolume, 0);]
      ,     soundNotify [;return glk_gestalt(gestalt_SoundNotify, 0);]
      ,     hyperlinks [;return glk_gestalt(gestalt_Hyperlinks, 0);] 
      ,     charOutput[c; return glk_gestalt(gestalt_CharOutput, c);]
      ,     charFromKeyStroke[c; return glk_gestalt(gestalt_CharInput, c);]
      ,     charFromLineInput[c; return glk_gestalt(gestalt_LineInput, c);]
      ,     drawImageForWindowType[wt; return glk_gestalt(gestalt_DrawImage, wt);]
   ;
    
#endif; !--After Parser
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);

#endif; !--After VERBLIB
!======================================================================================
! AFTER Grammar
#iftrue (LIBRARY_STAGE == AFTER_GRAMMAR);

#endif; !--After Grammar
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE


