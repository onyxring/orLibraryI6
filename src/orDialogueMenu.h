!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.09.02 orDialogueMenu
! Extending the orMenu to support items which execute commands when selected.
!--------------------------------------------------------------------------------------
! Created by Jim Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
!
!--------------------------------------------------------------------------------------
! Revision History
! 2024.09.02	Initial Creation
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
#ifndef        orDialogueMenu_STAGE;
default        orDialogueMenu_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "_orInfExt";
#include "orUtilArray";
#include "_orTopicPool";
#include "orMenu";
#include "orPlayerCommandQueue";
#include "orUtilBuf";
!--------------------------------------------------------------------------------------
#endif;
#ifnot;
#ifndef        orDialogueMenu_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orDialogueMenu_STAGE  < LIBRARY_STAGE);
   #undef      orDialogueMenu_STAGE  ;
   Constant    orDialogueMenu_STAGE  LIBRARY_STAGE;
   #ifdef      orDialogueMenu_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orDialogueMenu..."; #endif;
!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);
   property individual commandText;
#endif;
!======================================================================================
! AFTER PARSER
#iftrue (LIBRARY_STAGE == AFTER_PARSER);
   global autoMenuSpeak = true;
 

#endif; !--After Parser
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
  object amsDialog LibraryExtensions
      with ext_beforePrompt[;
         if(playerCommands.getLength()>0) rfalse; !--the player has queued actions; don't interupt these to display a dialogue menu
         
         if(autoMenuSpeak==true && orDialogueMenu.initForDisplay()>0) {
            orDialogueMenu.show(orMenuTopOnly);
         }
         rfalse;
      ]
   ;
   
   [sayTopic act top talkingTo;
      print "say ",(name)top, " to ",(name)talkingTo;
   ];
   object orDialogueMenu
      class orMenu
      with childMenuItems 0 0 0 0 0 0 0 0 0 0 0 0
      ,  initForDisplay[
               t talkingTo top;
            self.clear();
            talkingTo=vagueError(ResolveActorTalkingTo());

            for(t=0:t<util.orArray.getLength(playerDialoguePool):t++){
               top=util.orArray.get(playerDialoguePool,t);
               if(top.isAppropriateFor(player, talkingTo)) self.add(top);
            }
            return util.orArray.getLength(self,childMenuItems);
         ]
      , clear[; util.orArray.clear(self,childMenuItems);]
      , add[o; util.orArray.append(self,childMenuItems,o); ]
   ;

   [printCommandText act
         talkingTo top;

      talkingTo=act.getNoun();
      top=act.getSecond();

      if(top provides commandText) {
         util.orRef.resolvePrint(top, commandText, act, top, talkingTo);
      }
      else{      
         if(top.isKnownBy(player))
            print "tell ";
         else
            print "ask ";

         if(talkingTo>0) print (name)talkingTo, " ";

         print "about ", (name)top;
      }
      new_line;
   ];
   [queuePlayerTopic selected talkingTo;
      talkingTo = resolveActorTalkingTo(); 

      if(talkingTo>0){
         if(selected.isKnownBy(player)) {
            !<tellTopic talkingTo selected>;
            playerCommands.pushAction(##tellTopic, talkingTo, selected, printCommandText); 
         }else{
            !<askTopic talkingTo selected>;
            playerCommands.pushAction(##askTopic, talkingTo, selected, printCommandText); 
         }
      }else{ !the below wont do anything except print error messages, since we've established that we can't resolve who we are talking to
         if(selected.isKnownBy(player))
            <vagueTellTopic selected>;
         else
            <vagueAskTopic selected>;
      }

   ];
   orInfExt with ext_handleMenuResult[selected talkingTo;
      util.orui.activateMain();
   	if(selected ofclass orTopic) queuePlayerTopic(selected);
	];
#endif; !--After VERBLIB
!======================================================================================
! AFTER Grammar
#iftrue (LIBRARY_STAGE == AFTER_GRAMMAR);
verb meta 'ms' 'menuspeak' * -> menuSpeak;				!orLib: note that we launch the dialog menu as a meta verb, which takes no game time.  This is because selections are not
														!	immediately executed, but are queued up to happen at the next prompt.  This way, if the player exits from the menu without
														!	making a selection, no game time passes.

[menuSpeakSub; orDialogueMenu.show(orMenuTopOnly); ];	!orLib: show the dialog menu if it contains anything to show

verb meta 'ams' 'autoMenuSpeak' 
	* -> toggleAutoMenuSpeak
	* 'on'/'true' -> autoMenuSpeakOn
	* 'off'/'false' -> autoMenuSpeakOff
;
[toggleAutoMenuSpeakSub; 
	if(autoMenuSpeak==true) 
		autoMenuSpeakOffSub();
	else
		autoMenuSpeakOnSub();
];
[autoMenuSpeakOnSub; 
	if(autoMenuSpeak==true) 
		print "[Automatic dialog menus are already on.]^";
	else{
		autoMenuSpeak=true;	
		print "[Automatic dialog menus are now on.]^";
	}
];
[autoMenuSpeakOffSub; 
	if(autoMenuSpeak==false) 
		print "[Automatic dialog menus are already off.]^";
	else{
		autoMenuSpeak=false;	
		print "[Automatic dialog menus are now off.]^";
	}
];
#endif; !--After Grammar
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
