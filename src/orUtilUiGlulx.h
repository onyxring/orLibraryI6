!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2026.02.01 orUtilUiGlulx
! Provides UI-centric utilities specific to Glulx.
! NOTE: This is an EXPERIMENTAL extension and subject to change.
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

   object imageAlign
   with  aboveLine imagealign_InlineUp
   ,     belowLine imagealign_InlineDown
   ,     middleOfLine imagealign_InlineCenter
   ,     leftMargin  imagealign_MarginLeft
   ,     rightmargin imagealign_MarginRight
   ,     centered 6;

   object winType
   with  textGrid wintype_TextGrid
   ,     textBuffer wintype_TextBuffer
   ,     graphics wintype_Graphics;

   object imageScale
   with  fixed winmethod_Fixed
   ,     proportional winmethod_Proportional
   ;

   object imageDimension
   with   original -1
   ,      scaled -2
   ;

   object styleType
   with  normal style_Normal
   ,     emphasized style_emphasized
   ,     fixed style_Preformatted
   ,     header style_Header
   ,     subheader style_Subheader
   ,     alert style_Alert
   ,     note  style_Note
   ,     blockQuote style_BlockQuote
   ,     input style_Input
   ,     user1 style_User1
   ,     user2 style_User2
   ;

   class styleDefinition(5)
   with  _styleType 0
   ,     indentation orMaxEnd
   ,     paragraphIndentation orMaxEnd
   ,     justify  orMaxEnd
   ,     sizeAdjustment orMaxEnd
   ,     fontWeight orMaxEnd
   ,     isItalics  orMaxEnd
   ,     isFixedWidth  orMaxEnd
   ,     foreColor   orMaxEnd
   ,     backColor orMaxEnd
   ,     isReverse   orMaxEnd
   ,     clear[winType;
            glk_stylehint_clear(winType, self._styleType, stylehint_Indentation);

            glk_stylehint_clear(winType, self._styleType, stylehint_ParaIndentation);
            glk_stylehint_clear(winType, self._styleType, stylehint_Justification);
            glk_stylehint_clear(winType, self._styleType, stylehint_Size);

            glk_stylehint_clear(winType, self._styleType, stylehint_Weight);
            glk_stylehint_clear(winType, self._styleType, stylehint_Oblique);
            glk_stylehint_clear(winType, self._styleType, stylehint_Proportional);

            glk_stylehint_clear(winType, self._styleType, stylehint_TextColor);
            glk_stylehint_clear(winType, self._styleType, stylehint_BackColor);
            glk_stylehint_clear(winType, self._styleType, stylehint_ReverseColor);
         ]
   ,     apply[winType;
            if(self.indentation~=orMaxEnd) glk_stylehint_set(winType, self._styleType, stylehint_Indentation, self.indentation);
            if(self.paragraphIndentation~=orMaxEnd) glk_stylehint_set(winType, self._styleType, stylehint_ParaIndentation, self.paragraphIndentation);
            if(self.justify~=orMaxEnd) glk_stylehint_set(winType, self._styleType, stylehint_Justification, self.justify);
            if(self.sizeAdjustment~=orMaxEnd) glk_stylehint_set(winType, self._styleType, stylehint_Size, self.sizeAdjustment);

            if(self.fontWeight ~=orMaxEnd) glk_stylehint_set(winType, self._styleType, stylehint_Weight, self.fontWeight);
            if(self.isItalics~=orMaxEnd) glk_stylehint_set(winType, self._styleType, stylehint_Oblique, self.isItalics);
            if(self.isFixedWidth~=orMaxEnd) glk_stylehint_set(winType, self._styleType, stylehint_Proportional, self.isFixedWidth);

            if(self.foreColor~=orMaxEnd) glk_stylehint_set(winType, self._styleType, stylehint_TextColor, self.foreColor);
            if(self.backColor~=orMaxEnd) glk_stylehint_set(winType, self._styleType, stylehint_BackColor, self.backColor);
            if(self.isReverse~=orMaxEnd) glk_stylehint_set(winType, self._styleType, stylehint_ReverseColor, self.isReverse);
         ]
   ,     validateAgainst[win
               fails;

            if(self.indentation~=orMaxEnd) if(glk_style_measure(win.id, self._styleType, stylehint_Indentation, gg_arguments)==0) rfalse;


            if(self.paragraphIndentation~=orMaxEnd) if(glk_style_measure(win.id, self._styleType, stylehint_ParaIndentation, gg_arguments)==0) rfalse;
            if(self.justify~=orMaxEnd) if(glk_style_measure(win.id, self._styleType, stylehint_Justification, gg_arguments)==0) rfalse;
            if(self.sizeAdjustment~=orMaxEnd) if(glk_style_measure(win.id, self._styleType, stylehint_Size, gg_arguments)==0) rfalse;

            if(self.fontWeight ~=orMaxEnd) if(glk_style_measure(win.id, self._styleType, stylehint_Weight, gg_arguments)==0) rfalse;
            if(self.isItalics~=orMaxEnd) if(glk_style_measure(win.id, self._styleType, stylehint_Oblique, gg_arguments)==0) rfalse;
            if(self.isFixedWidth~=orMaxEnd) if(glk_style_measure(win.id, self._styleType, stylehint_Proportional, gg_arguments)==0) rfalse;

            if(self.foreColor~=orMaxEnd) if(glk_style_measure(win.id, self._styleType, stylehint_TextColor, gg_arguments)==0) rfalse;
            if(self.backColor~=orMaxEnd) if(glk_style_measure(win.id, self._styleType, stylehint_BackColor, gg_arguments)==0) rfalse;
            if(self.isReverse~=orMaxEnd) if(glk_style_measure(win.id, self._styleType, stylehint_ReverseColor, gg_arguments)==0) rfalse;

            rtrue;
         ]
   ;

   object justification
   with  left  stylehint_just_LeftFlush
   ,     full stylehint_just_LeftRight
   ,     centered stylehint_just_Centered
   ,     right stylehint_just_RightFlush
   ;
   object weight
   with  bold     1
   ,     normal   0
   ,     lighter    -1
   ;
   object _tmpSizeStruct with width, height;
   constant GLULX_WINDOWS_MAX=20;
#endif; !--Before Parser
!======================================================================================
! AFTER Parser
#iftrue (LIBRARY_STAGE == AFTER_PARSER);
   class _glulxWindow(GLULX_WINDOWS_MAX)
      with  id 0
      ,  parentWin 0
      ,  arrangement 0
      ,  _winType 0
      ,  create[;
            self.supports=_glulxWindowSupport.create(self);
         ]
      ,  splitLeft[type width scale;
            return self._splitWindow(type, width, scale, winmethod_Left);
         ]
      ,  splitRight[type width scale;
            return self._splitWindow(type, width, scale, winmethod_Right);
         ]
      ,  splitUp[type height scale;
            return self._splitWindow(type, height, scale, winmethod_Above);
         ]
      ,  splitDown[type height scale;
            return self._splitWindow(type, height, scale, winmethod_Below);
         ]
      ,  _splitWindow[type heightOrWidth scale arngmnt
               retval;
            if(scale==0) scale=imageScale.fixed;
            retval=_glulxWindow.create();
            retval.arrangement=arngmnt | scale | winmethod_NoBorder;
            self._applyQueuedStyles(type);
            retval.id=util.orUi.glulx._splitWindow(self.id, retval.arrangement, heightOrWidth, type);
            retval._winType=type;
            retval.parentWin=self;
            return retval;
         ]
      ,  _numStyles 0
      ,  queuedStyleDefinitions 0 0 0 0 0
      ,  queueStyle[styl
               retval;
            retval=styleDefinition.create();
            retval._styleType=styl;
            util.orArray.set(self, queuedStyleDefinitions, self._numStyles, retval);
            self._numStyles=self._numStyles+1;
            return retval;
         ]
      ,  getQueuedStyle[styl
               retval t;
            for(t=0:t<self._numStyles:t++){
               retval=util.orArray.get(self,queuedStyleDefinitions, t);
               if(retval._styleType==styl) return retval;
            }
            return 0;
         ]
      ,  _applyQueuedStyles[type
               t;
            for(t=0:t<self._numStyles:t++){
               util.orArray.get(self,queuedStyleDefinitions, t).apply(type);
            }
          ]
      ,  clearQueuedStyles[type
               t s;
            for(t=0:t<self._numStyles:t++) {
               s=util.orArray.get(self,queuedStyleDefinitions, t);
               s.clear(type);
               styleDefinition.destroy(s);
            }
            util.orArray.clear(self,queuedStyleDefinitions);
            self._numStyles=0;
         ]
      ,  validateQueuedStylesAgainst[win
               t s failed;

               for(t=0:t<self._numStyles:t++) {
                  s=util.orArray.get(self,queuedStyleDefinitions, t);
                  failed=failed+s.validateAgainst(win);
               }
               return failed;
         ]

      ! ,  moveUp[height scale
      !          par;
      !       if(scale==0) scale=imageScale.fixed;
      !       self.arrangement=winmethod_Above | scale | winmethod_NoBorder;
      !       par = glk_window_get_parent(self.id);
      !       glk_window_set_arrangement(par, self.arrangement, height, self.id);
      !    ]
      ! ,  moveDown[height scale
      !          par;
      !       if(scale==0) scale=imageScale.fixed;
      !       self.arrangement=winmethod_Below | scale | winmethod_NoBorder;
      !       par = glk_window_get_parent(self.id);
      !       glk_window_set_arrangement(par, self.arrangement, height, self.id);
      !    ]
      ! ,  moveLeft[width scale
      !          par;
      !       if(scale==0) scale=imageScale.fixed;
      !       self.arrangement=winmethod_Left | scale | winmethod_NoBorder;
      !       par = glk_window_get_parent(self.id);
      !       glk_window_set_arrangement(par, self.arrangement, height, self.id);
      !    ]
      ! ,  moveRight[width scale
      !          par;
      !       if(scale==0) scale=imageScale.fixed;
      !       self.arrangement=winmethod_Right | scale | winmethod_NoBorder;
      !       par = glk_window_get_parent(self.id);
      !       glk_window_set_arrangement(par, self.arrangement, height, self.id);
      !    ]
      ,  drawImage[imgId xOrAlignment yOrW wOrH hOrIgnore;
            util.orUi.glulx._drawImage(self, imgId, xOrAlignment, yOrW, wOrH, hOrIgnore);
         ]
      ,  getSize[retval;
            glk_window_get_size(self.id, gg_arguments, gg_arguments+WORDSIZE);
            retval=_tmpSizeStruct;
            retval.width=gg_arguments-->0;
            retval.height=gg_arguments-->1;
            return retval;
         ]
      ,  close[ c;
            if(self.id==0) return;
            objectloop(c ofclass _glulxWindow) if(c.parentWin==self) c.close();
            glk_window_close(self.id, 0);
            if(self.supports ofclass _glulxWindowSupport) _glulxWindowSupport.destroy(self.supports);
            _glulxWindow.destroy(self);
         ]
      ,  supports 0
      ,  resize[dim key
               p v;
            if(key==0) key=self.id;

             p=glk_window_get_parent(self.id);
             glk_window_get_arrangement(p, gg_arguments, gg_arguments+WORDSIZE, gg_arguments+(WORDSIZE*2));
             glk_window_set_arrangement(p, gg_arguments-->0, dim, key);
          ]
      ,  setBackgroundColor[col;
            if(self._winType==winType.graphics){
               glk_window_set_background_color(self.id, col);
               glk_window_clear(self.id);
            }
            else{
               print "Error: setBackgroundColor() is only valid for Graphics windows.";
            }
         ]
   ;

   class _glulxWindowSupport(GLULX_WINDOWS_MAX)
      with  create[win; self.attachedWindow=win;]
      ,     attachedWindow 0
      ,     mouseInput[;return glk_gestalt(gestalt_MouseInput, self.attachedWindow.id);]
      ,     drawImage[;return glk_gestalt(gestalt_DrawImage, self.attachedWindow.id);]
      ,     hyperlinkInput[;return glk_gestalt(gestalt_HyperlinkInput, self.attachedWindow.id);]
   ;

   _glulxWindowSupport _winScreenSupportGlulxWindow;
   _glulxWindow winScreen with supports _winScreenSupportGlulxWindow;

object   _orUIGlulx LibraryExtensions
      private
      with _screenPixelWidthSizer 0
      ,   _screenPixelHeightSizer 0
      ,   _drawImage[win img xOrAlignment yOrW wOrH hOrScale;
               if(win._winType==winType.textBuffer) self._drawImageInTextBuffer(win.id, img, xOrAlignment, yOrW, wOrH);
               if(win._winType==winType.graphics) self._drawImageInGraphicsWindow(win.id, img, xOrAlignment, yOrW, wOrH, hOrScale);
            ]
      ,   _drawImageInGraphicsWindow[win img x y w h
                  imgInfo;
               imgInfo=self.getImageSize(img);
               if(imgInfo==0) "[ERROR:orUI unable to determine image dimentions.]";

               if(h==0 or imageScale.proportional && w==0 or imageScale.proportional) w=h=imageDimension.original; !--both dimentions are undefined, so no way to keep these proportional
               if(w~=0 or imageScale.proportional && h==0 or imageScale.proportional) h=(imgInfo.height * w) /imgInfo.width;
               if(h~=0 or imageScale.proportional && w==0 or imageScale.proportional) w=(imgInfo.width * h) / imgInfo.height;

               if(h==imageDimension.original) h=imgInfo.height;
               if(w==imageDimension.original) w=imgInfo.width;

               glk_image_draw_scaled(win,img,x,y,w,h);
            ]
      ,   _drawImageInTextBuffer[win img alignment w h
            imgInfo i;
            if(alignment==0) alignment=imageAlign.centered;

            imgInfo=self.getImageSize(img);
            if(imgInfo==0) "[ERROR:orUI unable to determine image dimentions.]";

            if(h==0 or imageScale.proportional && w==0 or imageScale.proportional) w=h=imageDimension.original; !--both dimentions are undefined, so no way to keep these proportional
            if(w~=0 or imageScale.proportional && h==0 or imageScale.proportional) h=(imgInfo.height * w) /imgInfo.width;
            if(h~=0 or imageScale.proportional && w==0 or imageScale.proportional) w=(imgInfo.width * h) / imgInfo.height;

            if(h==imageDimension.original) h=imgInfo.height;
            if(w==imageDimension.original) w=imgInfo.width;

            if(alignment==imageAlign.centered){ !--in this mode, we believe we are print to a text window, so lets center it by printing fixed-width spaces...

               !i=(self.getScreenTextWidth()/2)-(w/self.getMonoFontPixelWidth());

               !i=((self.getScreenPixelWidth()*100 - (w*100))/2)/self.getMonoFontPixelWidth(100); !we multiply these calcs by 100 to reduce the rounding errors in calculatins, since Inform doesn't support decimal values

               i=(winMain.getSize().width - self.convertPixelWidthToTextWidth(w))/2;
               !i=(self.getScreenTextWidth() - self.convertPixelWidthToTextWidth(w))/2;
               style fixed;
               new_line;
               spaces(i);
               style roman;
               alignment=imageAlign.middleOfLine;
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
      ,  getStatusTextWidth[; return winStatus.getSize().width; ]
      ,	getMonoFontPixelWidth[res;
            if(res==0) res=1; !res is used to do the math at a higher order of magnitude to avoid truncating decimal places
            !return ((self._screenPixelWidthSizer.getSize().width*res)/ (self.getScreenTextWidth()));
            return ((self._screenPixelWidthSizer.getSize().width*res)/ util.orUi.getScreenWidth());

         ]
      ,	getMonoFontPixelHeight[n; return self._screenPixelHeightSizer.getSize().height/util.orUi.getScreenHeight(); ]
      ,  supports _glulxSupport
      ,  makeWinFromId[winid type
               p;
            winStatus=_glulxWindow.create(); !--expand the winStatus variable just like we did winMin in the last pass
            !p = glk_window_get_parent(d);

            glk_window_get_arrangement(p, gg_arguments, gg_arguments+WORDSIZE, gg_arguments+(WORDSIZE*2));
               winStatus.id=winid;
               winStatus._winType=type;
               winStatus.arrangement=gg_arguments-->0;
         ]
      ,  priority 20
      ,  ext_InitGlkWindow[rck; !--called multiple times by the library...
            if(rck==0){
               _winScreenSupportGlulxWindow.attachedWindow=winScreen;
               _orUi.glulx=self; !install this into the orUi object, where it belongs
            }
            if(rck==GG_MAINWIN_ROCK){ !--the library is about to initialize the main window which is the entire screen, but has not done so yet.
               winScreen._applyQueuedStyles();
            }
            if(rck==GG_STATUSWIN_ROCK){ !--the library has just initialized the main window which is the entire screen, but has not yet initialized the status bar
   
               winScreen._applyQueuedStyles();
               winMain=_glulxWindow.create(); !--associate the winMin variable, defined in orUtilUi, as an actual, uable object, rather than an enumerated value.  The effect is the same for switch statements, but allows us to use the variable in other ways
                  winMain.id=gg_mainwin;
                  winMain._winType=winType.textBuffer;
                  winMain.arrangement=winmethod_Below|winmethod_Proportional;

               !--now we create zero width windows to allow us to get sizes which aren't normally available to us...
               self._screenPixelWidthSizer=winMain.splitUp(winType.graphics,0); !--the width of the screen in pixels
               self._screenPixelHeightSizer=winMain.splitLeft(winType.graphics,0); !--the height of the screen in pixels
            }
            if(rck==1){ !--the last call by the parser
               winStatus=_glulxWindow.create(); !--expand the winStatus variable just like we did winMin in the last pass
                  winStatus.id=gg_statuswin;
                  winStatus._winType=winType.textGrid;
                  winStatus.arrangement=winmethod_Above|winmethod_Fixed;
               
               
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


