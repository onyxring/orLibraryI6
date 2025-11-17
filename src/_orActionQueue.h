!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2004.06.08 orActionQueue
! A base class to represent actions and detect whether or not they are available.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
! This extension, classified as a "utility", provides tooling for other extensions to
! use and provides no specific features by itself.
!
!--------------------------------------------------------------------------------------
! Revision History
! 2004.02.15	Initial Creation
! ...			History from orLibrary v1 snipped away.
! 2024.08.10	Refactored for the orLibrary version 2.
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orActionQueue_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orDeque";
#include "orUtilRef";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orActionQueue_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orActionQueue_STAGE  < LIBRARY_STAGE);
   #undef      orActionQueue_STAGE  ;
   Constant    orActionQueue_STAGE  LIBRARY_STAGE;
   #ifdef      orActionQueue_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orActionQueue..."; #endif;
!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);
	property individual ext_canAttemptAction;
	property individual ext_isCompletedAction;
#endif; !before parser
!======================================================================================
! AFTER PARSER
#iftrue (LIBRARY_STAGE == AFTER_PARSER);
	default ACTIONQUEUE_BUFFER 10;	 !--TODO: should be named something else?
	global orActionFlags 0;

	class orAction(ACTIONQUEUE_BUFFER)
		private
			_action 0
		,	_noun 0
		,	_second 0
		,	_cmd 0
		,	_flags 0
		with _performer[; return actor;]
		,  create[a n s displayText;
				self.initialize(a,n,s,displayText);
			]
		,	destroy[;
#ifdef orString_STAGE;
				if(self._cmd ofclass orString) { !--just an extra safeguard in case we previously failed to free the orString
					self._cmd.free();
					self._cmd=0;
				}
#endif;
			]
		,	initialize[a n s displayText; ! performer displayText;
#ifdef orString_STAGE;
				if(self._cmd ofclass orString) { !--just an extra safeguard in case we previously failed to free the orString
					self._cmd.free();
					self._cmd=0;
				}
				if(util.orRef.isString(a) || a ofclass orString) {
					if(a ofclass orString) a=util.orStr.new(a); !--create a safe copy of the orString object (the calling routine will free the passed in parameter)
#ifnot;
				if(util.orRef.isString(a)) {
#endif;
					self._cmd=a;
					self._displayTextOverride=n;
					self._action=0;
					self._noun=0;
					self._second=0;
				}
				else{
					self._action=a;
					self._noun=n;
					self._second=s;
					self._displayTextOverride=displayText;
					self._cmd=0;
				}
				self._flags=0;
				self._performer=self.orAction::_performer; !--reset to base routine
				self.canAttempt=self.orAction::canAttempt; !--reset to default in case overridden in a previous instance...
			]
		,	keepSilent false
		,	getAction[;return self._action;]
		,	getNoun[;return self._noun;]
		,	getSecond[;return self._second;]
		,	getCustomFlags[;return self._flags;]
		,	getCustomFlag[bit;return util.orNum.getBit(self._flags,bit);]
		,	setCustomFlagOn[bit;self._flags=util.orNum.setBitOn(self._flags,bit);]
		,	setCustomFlagOff[bit;self._flags=util.orNum.setBitOff(self._flags,bit);]
			! createTextCommand[cmd;
			! 	self._cmd=cmd;
			! 	self._displayTextOverride=0;
			! 	self._performer=self.orAction::_performer; !--reset to base routine
			! 	self.canAttempt=self.orAction::canAttempt; !--reset to default in case overridden in a previous instance...
			! ],
		,	isMeta false
		,	_displayTextOverride 0
		,	print[;
				if(self._displayTextOverride~=0) {
					self._displayTextOverride(self);
				}
				else
					self.formatCommand(); !PrintOrRun(self,prop,true);

			]
		,	formatCommand[
					n s a;
				if(self._cmd~=0) {
#ifdef orString_STAGE;
					if(self._cmd ofclass orString){
						self._cmd.print();
						return;
					}
#endif;
					PrintOrRun(self,_cmd,true);
					return;
				}

				a=ValueOrRun(self,_action);
				print (ActionName)a;
				n=valueorrun(self,_noun);
				s=valueorrun(self,_second);
				if(n~=0) print " ",(name)n;
				if(s~=0) {
					self.printPreposition();
					print " ",(name)s;
				}
			]
		,	printPreposition[;
				if(valueorrun(self,_second)==0) return; !--no second, no preposition
				if(self._action==##PutOn) print " on";
				else if(self._action==##Insert) print " into";
				else if(self._action==##Give or ##Show) print " to";
				else if(self._action==##ThrowAt) print " at";
				else if(self._action==##Wave) print " at";
				else if(self._action==##Unlock or ##Lock or ##Open or ##Close) print " with";
			]
		,	isDone[a n s actn;
				if(self._cmd~=0) return -1;
				if(LibraryExtensions.RunUntil(ext_IsCompletedAction,true,self)~=true) {
					a=valueorrun(self,_performer);
					n=valueorrun(self,_noun);
					s=valueorrun(self,_second);
					actn=ValueOrRun(self,_action);
					!--handle no-noun verbs here
					switch(actn){
						!--add default handler here.
					}
					switch(actn){
						##Take: if(n in a) rtrue;
						##Drop: if(~~(n in a)) rtrue;
						##Open: if(n has open) rtrue;
						##Close: if(n hasnt open) rtrue;
						##Wear: if(n in a && n has worn) rtrue;
						##Disrobe: if(~~(n in a) || n hasnt worn) rtrue;
						##Eat: if(parent(n)==0) rtrue;
						##Enter: if(a in n) rtrue;
						##SwitchOn: if(n has switchable && n has on) rtrue;
						##SwitchOff:if(n has switchable && n hasnt on) rtrue;
					}
					switch(actn){
						##PutOn: if(n in a && s has supporter) rtrue;
						##Give: if(n in a && s has animate) rtrue;
						##Insert: if(n in a && s has container) rtrue;
						##Unlock: if(n hasnt locked) rtrue;
						##Lock:if(n has locked) rtrue;
					}
				}
				rfalse;

			]
		,	canAttempt[a n s actn matchscope touch_n touch_s;
				if(self._cmd~=0) rtrue; !we assume all text commands are possible, since we can't really tell that they aren't
				matchscope=true; !--default to in scope
				touch_n=true;  !--and touchable
				touch_s=true;  !--and touchable
				if(LibraryExtensions.RunUntil(ext_canAttemptAction,false,self)~=false) {
					a=valueorrun(self,_performer);
					n=valueorrun(self,_noun);
					s=valueorrun(self,_second);
					actn=ValueOrRun(self,_action);
					!--handle no-noun verbs here
					switch(actn){
						!--add default handler here.
					}
					!--ensure noun in scope from here on...
					if(n==0 || testscope(a,n)==false) {
						touch_n=matchscope=false; !--not in scope
					}
					else touch_n=(objectisuntouchable(n,true)==false);

					switch(actn){
						##Examine: if(~~(matchscope)) rfalse;
						##Take: if(~~(touch_n && n hasnt scenery or static && parent(n) hasnt animate && parent(n)~=player)) rfalse;
						##Drop: if(~~(n in a)) rfalse;
						##Open: if(~~(touch_n && n has openable && n hasnt open && n hasnt locked)) rfalse;
						##Close: if(~~(touch_n && n has openable && n has open)) rfalse;
						##Wear: if(~~(n in a && n has clothing && n hasnt worn)) rfalse;
						##Disrobe: if(~~(n in a && n has worn)) rfalse;
						##Eat: if(~~(touch_n && n has edible)) rfalse;
						##Enter: if(~~(touch_n && n has enterable or door && (n hasnt openable || n has open))) rfalse;
						##SwitchOn: if(~~(touch_n && n has switchable && n hasnt on)) rfalse;
						##SwitchOff:if(~~(touch_n && n has switchable && n has on)) rfalse;
					}
					!--ensure second in scope from here on (and noun too)...
					if(s==0 || testscope(a,s)==false) {
						touch_s=matchscope=false; !--not in scope
					}
					else touch_s=(objectisuntouchable(s,true)==false);

					switch(actn){
						##PutOn: if(~~(touch_s && n in a && s has supporter)) rfalse;
						##Give: if(~~(touch_s && n in a && s has animate)) rfalse;
						##Insert: if(~~(touch_s && n in a && s has container)) rfalse;
						##Unlock: if(~~(touch_n && n has locked && s in a)) rfalse;
						##Lock:if(~~(touch_n && n hasnt locked && n has lockable && s in a)) rfalse;
					}
				}
				rtrue; !--default to possible if we've nothing to say it isn't
			]
		,	execute[saveactor;
				saveactor=actor;
				actor=ValueOrRun(self,_performer);
				action=valueorrun(self,_action);	!--action
				noun=valueorrun(self,_noun);		!--noun of the action
				second=valueorrun(self,_second);	!--second of the action
				orActionFlags=valueorrun(self,_flags);
				ActionPrimitive();
				orActionFlags=0;
				actor=saveactor;
			]
	;

	class orActionQueue
		class orDeque
		private
		with _queuedActions 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
		,	arrayPropName _queuedActions
		,	preemptAction[v n s displayText meta silent
					actn;
				actn=self.preQueue(v,n,s,displayText, meta);
				actn.keepSilent=silent;
				return actn;
			]
		,	preemptCommand[cmd meta silent
					actn;
				actn=self.preQueue(cmd,0,0,0, meta);
				actn.keepSilent=silent;
				return actn;
			]
		,	pushAction[v n s displayText meta silent
					actn;
				actn=self.enqueue(v,n,s,displayText, meta);
				actn.keepSilent=silent;
				return actn;
			]
		,	pushCommand[cmd meta silent
					actn;
				actn=self.enqueue(cmd,0,0,0, meta);
				actn.keepSilent=silent;
				return actn;
			]
		,	preQueue[v n s seltxt meta
					actn;
				actn = orAction.create(v, n, s);
				if(seltxt~=0) actn._displayTextOverride=seltxt;

				actn.isMeta=meta;
				self.orDeque::push(actn);
				return actn;
			]
		,	enqueue[v n s seltxt meta
					actn;
				actn = orAction.create(v, n, s);
				if(seltxt~=0) actn._displayTextOverride=seltxt;

				actn.isMeta=meta;
				self.orDeque::enqueue(actn);
				return actn;
			]
		,	dequeue[; return self.orDeque::dequeue(); ]
		,	clear[t h;
				for(t=0:t<self.getLength():t++) {
					h=self.pop(t);
					if(h~=0) orAction.destroy(h);
				}
				self.orDeque::clear();
			]
		,	getLength[; return self.orDeque::getLength(); ]
		,	interrupt false
	;
#endif; !--After Parser
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
	[ actionName a
			str;
		switch(a){
			##disrobe: print "take off";
			##throwAt: print "throw";
			##putOn: print "put";
			default:
			#ifdef TARGET_GLULX;
				str = #identifiers_table-->6;
			#ifnot;
				str = #identifiers_table;
				str = str + 2*(str-->0) + 2*48;
			#endif;
				print (string) str-->a;
		}
	];
#endif; !--AFTER_VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE