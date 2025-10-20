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
default        orDialogueMenu_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
!#include "_orInfExt";
#include "orUtilArray";
#include "_orTopicPool";
#include "orMenu";
#include "orPlayerCommandQueue";
#include "orUtilBuf";
!--------------------------------------------------------------------------------------
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

#endif;
!======================================================================================
! AFTER PARSER
#iftrue (LIBRARY_STAGE == AFTER_PARSER);

#endif; !--After Parser
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
   #ifndef orTempBuffer ;array orTempBuffer buffer 2400; #endif; !orTempBuffer is used thoughout the library

   class orDialogueMenu
      class orMenu
      with childMenuItems 0 0 0 0 0 0 0 0 0 0 0 0
      ,  init[
               t o;
            self.clear();
            for(t=0:t<util.orArray.getLength(playerDialoguePool):t++){
               self.add(util.orArray.get(playerDialoguePool,t));
            }
            return util.orArray.getLength(self,childMenuItems);
         ]
      , clear[; util.orArray.clear(self,childMenuItems);]
      , add[o; util.orArray.insert(self,childMenuItems,o);]
   ;

   [printAskTellAction act
         talkingTo top;

      talkingTo=act.getNoun();
      top=act.getSecond();

      if(top.isKnownBy(player))
         print "tell ";
      else
         print "ask ";

      if(talkingTo>0) print (name)talkingTo, " ";

      print "about ", (name)top;
   ];
   orInfExt with ext_handleMenuResult[selected talkingTo;

         util.orui.activateMain();

   		if(selected ofclass orTopic){

            talkingTo = resolveActorTalkingTo(player);

            if(talkingTo>0){
               if(selected.isKnownBy(player)) {
                  !<tellTopic talkingTo selected>;
                  playerCommands.pushAction(##tellTopic, talkingTo, selected, printAskTellAction); !, buf);
               }else{
                  !<askTopic talkingTo selected>;
                  playerCommands.pushAction(##askTopic, talkingTo, selected, printAskTellAction);!, buf);
               }
            }else{ !the below wont do anything except print error messages, since we know we wont be able to resolve who we are talking to
               if(selected.isKnownBy(player))
                  <vagueTellTopic selected>;
               else
                  <vagueAskTopic selected>;
            }

         }
	];
#endif; !--After VERBLIB
!======================================================================================
! AFTER Grammar
#iftrue (LIBRARY_STAGE == AFTER_GRAMMAR);

#endif; !--After Grammar
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
