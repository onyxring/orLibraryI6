!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.09.02 orMenu
! A menu system with both full screen and top-only modes.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
!
! The following is an example of how a menu system might be implemented:
!
!	orMenu topMenu "Choose from the following";
!	orMenu -> "Do stuff with sticks.";
!	orMenu ->-> takesticks "Pick up sticks.";
!	orMenu ->-> burnsticks "Set sticks on fire.";
!	orMenu ->-> kicksticks "Kick sticks.";
!	orMenu -> "Look at stuff";
!	orMenu ->-> ash "Look closely at the fire ash.";
!	orMenu ->-> river "Look in the river.";
!	orMenu ->-> hole "Look into the hole.";
!	orMenu ->-> fish "Look at the fish.";
!	orMenu ->-> dog "Look around the tree.";
!
! Calling the menu programmatically can be accomplished with a call to the "show" property
! which also returns the user's selection (if any):
!
!	result=topMenu.show(orMenuTopOnly);
!
! The displayStyle property of the called menu object will determine the display form of
! the menu, either full screen (the default) suitable for "about" menus to read text,
! or at the top of the screen which does not clear the game transcript.  The following
! are valid values for this property:
!
!	orMenuTopOnly
!	orMenuFullScreen
!
! The result returned is the actual selected orMenu object, determined when a menu option
! with no children is chosen.   A zero result indicates no user selection (the user
! backed out of the menu).
!
! In full screen mode, childless menus can define the description property.  If defined,
! this will be displayed, inset on the screen and the menu will continue to run, allowing
! the player to back out and read other menus. In this mode, childless menus which do not
! define the description property will end the menu session and return the selected menu
! for the calling process to handle.
!
! In non-full screen mode, childless menus end the menu session and are returned,
! regardless of the presence of the the description property.
!
! The description property is ignored for menu objects with children.
!
!--------------------------------------------------------------------------------------
! Revision History
! 2002.01.01	Initial Creation
! ...			History from orLibrary v1 snipped away.
! 2024.09.02	Refactored for the orLibrary version 2.
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orMenu_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "orCenter";
#include "orUtilStr";
#include "orUtilUi";
#include "orUtilRef";
#include "_orInfExt";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orMenu_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orMenu_STAGE  < LIBRARY_STAGE);
   #undef      orMenu_STAGE  ;
   Constant    orMenu_STAGE  LIBRARY_STAGE;
   #ifdef      orMenu_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orMenu...";#endif;
!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);
	Global _orMenuFullScreenStatusHeight 2;
	constant orMenuFullScreen	0;
	!constant orMenuTopOnly		1; !todo - implement this!
	!constant ORMENU_BOTTOM	-1;
	property individual childMenuItems;
	property individual menuDisplay;
	property individual ext_handleMenuResult;
	property individual rawDescription;

#endif; !--Before Parser
!======================================================================================
! AFTER PARSER
#iftrue (LIBRARY_STAGE == AFTER_PARSER);
	#include "orUtilUi"; !make sure this previously declared dependency is included *before* this code

	[printOr val tStr fStr; if(val==true) print(string) tStr; else print (string) fStr;];
	object orMenuController
		private _startingStatusHeight 0 !remember the status height used by the game
		,	_rootMenu 0 !--The menu that show() was actually called against
		,	_currentHighlight 0 !--the current selection for the running object
		,	_isListMenu[menuInstance;
				if(menuInstance provides childMenuItems && util.orArray.getSize(menuInstance,childMenuItems)>0) rtrue;
				rfalse;
			]
		! This routine is called recursively.  It renders the current menu, then calls the input loop which may call this routine to render a different menu
		,	_render[currentMenu i;
				self._currentHighlight=0; !position at the top of the list
				!--this is a last-child item...
				if(self.init(currentMenu)==0) { !--if no children to display...
					if(self.displayStyle==orMenuFullScreen && (currentMenu provides description|| currentMenu provides rawDescription)){  !we only render the text in full screen mode
						util.orUI.setStatusHeight(util.orUI.getScreenHeight()); !force the status line to be as big as the screen 
						
						
						print util.orUI.getScreenHeight(), ":", util.orUI.getStatusHeight();
						self._renderFullScreenHeader(currentMenu);
						print util.orUI.getScreenHeight(), ":", util.orUI.getStatusHeight();
						print "^^"; !add a blank line after the header
						if(currentMenu provides rawDescription)
							printOrRun(currentMenu,rawDescription);
						else
							orInset(currentMenu.description);
						
						util.orUI.hideCursor();
						util.orUI.getChar(); !--pause with the description showing
					}
					else{
						self.result=currentMenu; !If there isn't anything to display, then this must be a selection, so record the selection and exit
					}
					return;
				}

				!--we have children to show in a menu...
				if(self.displayStyle==orMenuFullScreen){
					util.orUI.setStatusHeight(util.orUI.getScreenHeight()); !force the status line to be as big as the screen 
					self._renderFullScreenHeader(currentMenu);
				}
				else{
					util.orUI.setStatusHeight(self._startingStatusHeight + self.getNumberOfVisibleChildren(currentMenu));
					DrawStatusLine();
				}

				self._renderItems(currentMenu);
				self._inputLoop(currentMenu);				!--handle User Input
				self._dismiss();
				return;
			]
	,	_renderFullScreenHeader[currentMenu;
				!self._resizeHeader(currentMenu);

				util.orUI.activateStatus();
				style reverse;
				util.orUI.eraseStatus();
				
				util.orUI.position(0, 0); spaces(util.orUI.getScreenWidth()); 
				util.orUI.position(0, 1); spaces(util.orUI.getScreenWidth()); 

				style reverse;
				util.orUI.position(0, 0);
				orCenter(currentMenu,0,HIGHLIGHT_ALL);
				util.orUI.position(0, 1);

				style reverse;
				!--left-----
				printOr(children(currentMenu), " X : ", "Any: ");
				printOr(currentMenu==self._rootMenu, "Exit ", "Prev");
				!--right-----
				if(children(currentMenu)){
					util.orUI.position(util.orUI.getScreenWidth()-14, 1);
					print "Enter : Select";
				}
				style roman;
			]
		,	_resizeHeader[currentMenu 
					s;
				if(self.displayStyle==orMenuFullScreen)
				 	s = util.orUI.getScreenHeight(); !_orMenuFullScreenStatusHeight;
				else
				 	s = self._startingStatusHeight + self.getNumberOfVisibleChildren(currentMenu);

				util.orUI.setStatusHeight(s); !
			]
		,	_renderItems[currentMenu o t;
				util.orUI.activateStatus();
				!self._resizeHeader(currentMenu);
				!--Actually draw all the items...
				if(self._isListMenu(currentMenu)) {
					for(t=0:t<util.orArray.getSize(currentMenu,childMenuItems):t++){
						o=util.orArray.get(currentMenu,childMenuItems,t);
						if(util.orRef.isObject(o)==false || o hasnt concealed) self._drawItem(o,false, t);
					}
				}
				else{
					objectloop(o in currentMenu && o ofclass orMenu && valueOrRun(o, canDisplay)==true)
					self._drawItem(o, false, o.number);
				}

				!--now show what is currently selected...
				if(self._currentHighlight<self.getNumberOfVisibleChildren(currentMenu))
					self._highlightItemByIndex(currentMenu, self._currentHighlight);
				else
					self._highlightItemByIndex(currentMenu, 0);

			]
		,	_dismiss[;
				util.orUI.setStatusHeight(self._startingStatusHeight);
				util.orUI.eraseStatus();
				util.orUI.activateMain();
	
		]

		,	_highlightItemByIndex[currentMenu num
					obj;
				obj=self._getItem(currentMenu, self._currentHighlight);

				self._drawItem(obj, false, self._currentHighlight);
				self._currentHighlight=num;
				obj=self._getItem(currentMenu, num);

				self._drawItem(obj,true, num);
			]
			!---translate a number into the actual menu object
		,	_getItem[currentMenu num
					o;
				if(self._isListMenu(currentMenu))
					return util.orArray.get(currentMenu,childMenuItems,num);

				objectloop(o in currentMenu) if(o ofclass orMenu && o.number==num) return o;

			]
			!---actually write out the name of a given object
		,	_drawItem[obj isSelected vpos
					str;

				if(obj==0) return; 
			
				str=util.orStr.new(); !--allocate a string object

				!--adjust the vertical position of this item, based on the size of the header (based on display mode)
				if(self.displayStyle==orMenuFullScreen) 
					vpos=vpos+_orMenuFullScreenStatusHeight;
				else
					vpos=vpos + self._startingStatusHeight;

				!--format the text we are going to display
				str.capture();
				printOr(isSelected>0, self.prefixHighlight,self.prefixNormal); !--selected prefix

				if(util.orRef.isString(obj))
					print (string)obj;
				else {
					if(obj provides menuDisplay)
						printOrRun(obj, menuDisplay, true);
					else
						print (name)obj;
				}

				printOr(isSelected>0, self.postfixHighlight, self.postfixNorm); !--selected postfix
				str.release();
				
				!--now position vertically and clear the line
				util.orUI.position(0,vpos);
				spaces util.orUI.getScreenWidth();
				util.orUI.position(0,vpos);

				orCenter(str); !and center the text
				str.free();
			]
			!---handle user input
		,	_handleInput[ch currentMenu i count;
				count=self.getNumberOfVisibleChildren(currentMenu);
				if(count==0) rtrue;
				switch(ch){
					'p','P',129, -4:if(self._currentHighlight-1>=0) self._highlightItemByIndex(currentMenu, self._currentHighlight-1);
					'n','N',130, -5:if(self._currentHighlight+1<count) self._highlightItemByIndex(currentMenu, self._currentHighlight+1);
					' ',10,13, -6 :if(count>0){
										i=self._getItem(currentMenu, self._currentHighlight);
										self._render(i);  !--pass off control to selected item
										if(self.result~=0) {
											if(LibraryExtensions.runWhile(ext_handleMenuResult,false, self.result)==false) rtrue;
											!TODO: if reached here; handlemenuresult chose to return true, to interupt normal behavior and not exit the menu.  Might need to add code here to handle this.
										}

										!--when returns, re_display this one again
										self._currentHighlight=i.number; !make sure to return the selection to the same place in case the user is checking out each item one at a time
										self._renderFullScreenHeader(currentMenu);
										self._renderItems(currentMenu);
									}
					'`','~','q','Q',133,'x','X', 27:
						self.result=0;
						rtrue;
				}
				rfalse;
			]
		,	_inputLoop[currentMenu ch win; win=win;
				while(true){
					util.orUI.hideCursor();
					ch=util.orUI.getChar();
					if(self.getNumberOfVisibleChildren(currentMenu)==0) return; !--user has selected something that has no child menu items to navigate
					if(self._handleInput(ch, currentMenu)==true) return; !--user has backed out of the menu
				}
			]
		with displayStyle orMenuFullScreen
		,	prefixHighlight "> "
		,	postfixHighlight " <"
		,	prefixNormal "  "
		,	postfixNorm "  "
		,	result  0 !--The final selected result
		,	getNumberOfVisibleChildren[currentMenu
					count o;
				if(self._isListMenu(currentMenu)) {
					return util.orArray.getSize(currentMenu, childMenuItems);
				}
				objectloop(o in currentMenu && o ofclass orMenu && valueorrun(o, canDisplay)) count++;
				return count;
			]
		,	show[menuInstance showStyle;
				self._rootMenu=menuInstance; !--remember this entry point for the duration of this call
				self.displayStyle=showStyle; !--also remember the showStyle we are rendering
				self.result=0; !clear previous results, if any
				self._startingStatusHeight=util.orUI.getStatusHeight();
				self._render(menuInstance);	!--_display this menu option, returning when the user has made a selection

				util.orUI.setStatusHeight(self._startingStatusHeight); !--restore the status line size
				DrawStatusLine();

				! if(self.displayStyle==orMenuFullScreen) {
				! 	!util.orUI.eraseScreen();  !in addition to clearing the screen, this will also force the status line to restore
				! 	!new_line;
				! 	DrawStatusLine();
				! 	!LookSub(); !since we've just exited full screen mode, lets show the current location to refresh the players context
				! }
				return self.result;
			]
		,	init[currentMenu o count; !--get this menu ready to display
				count=0;
				if(currentMenu provides init) return currentMenu.init();

				if(self._isListMenu(currentMenu)) return util.orArray.getSize(currentMenu,childMenuItems);

				objectloop(o in currentMenu && o ofclass orMenu) {
					if(valueorrun(o, canDisplay)){
						o.number=count; !--assign each child menu a sequence number...
						count++;
					}
					else o.number=-1;
				}
				return count; !return the number of children to display
			]
	;

	class orMenu
		with number 0
		,	show[showStyle; return orMenuController.show(self, showStyle);]
		,	canDisplay[; !o hasChildren t obj;
				if(self has concealed) rfalse; !hidden
				if(orMenuController.getNumberOfVisibleChildren(self)>0) rtrue; !there are visible children
				if(children(self)==0) rtrue; !no children at all, hidden or otherwise, so this must be a selectable option

				rfalse; !here we've established that there ARE children, but none of them are visible, so hide this parent object as well

				!objectloop(o in self && valueorrun(o,canDisplay)==true) rtrue; !at least one child is visible
				!rfalse; !has children, but none ar visible

				!rtrue;

				!if(orMenuController.getNumberOfVisibleChildren(self)>0) rtrue;

				! if(_isListMenu._isListMenu(self)) {
				! 	for(t=0:t<util.orArray.getSize(self,childMenuItems):t++){
				! 		obj=util.orArray.get(self,childMenuItems);
				! 		if(util.orRef.isObject(obj)==false) rtrue; !non-objects can't be hidden
				! 		if(obj hasnt concealed) rtrue; !at least one item is visible
				! 	}
				! }
				! if(children(self)==0) rtrue; !no children, so this must be an action
				! objectloop(o in self && valueorrun(o,canDisplay)==true) rtrue; !at least one child is visible
				!rfalse; !has children, but none ar visible
			]
	;

#endif; !--After Parser
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE