!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
! 2024.09.02 orNPCSkillDialogue
! A base class and controlling daemon for NPCs.
!--------------------------------------------------------------------------------------
! Created by Jin Fisher
!-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -
! License: Public Domain
!--------------------------------------------------------------------------------------
!
!--------------------------------------------------------------------------------------
! Revision History
! 2002.02.08	Initial Creation
! ...			History from orLibrary v1 snipped away.
! 2024.09.02	Refactored for the orLibrary version 2.
!======================================================================================
! Extension Framework management
#ifndef        orExtensionFramework_STAGE;
default        orNPCSkillDialogue_STAGE  0;
!--------------------------------------------------------------------------------------
! INCLUDE DEPENDENCIES
#include "orNpc";
#include "orDialogue";
#include "_orTopicPool";
!--------------------------------------------------------------------------------------
#ifnot;
#ifndef        orNPCSkillDialogue_STAGE; message fatalerror orXFErrorInclude; #endif;
#iftrue(       orNPCSkillDialogue_STAGE  < LIBRARY_STAGE);
   #undef      orNPCSkillDialogue_STAGE  ;
   Constant    orNPCSkillDialogue_STAGE  LIBRARY_STAGE;
   #ifdef      orNPCSkillDialogue_STAGE  ; #endif;
   #ifndef orExtensionFrameworkBrief; message "   orNPCSkillDialogue...";#endif;
!======================================================================================
! BEFORE PARSER
#iftrue (LIBRARY_STAGE == BEFORE_PARSER);

#endif; !--Before Parser
!======================================================================================
! AFTER PARSER
#iftrue (LIBRARY_STAGE == AFTER_PARSER);

   class orNPCSkillDialogue
      with dialoguePool 0 0 0 0 0
      , learnTopic[topic;
         if(util.orArray.find(topic,knownBy,self)==-1)
            util.orArray.append(topic,knownBy,self);
      ]
      , takeNoticeOfTopic[topic;
         if((self provides dialoguePool)==false) return;
         if(util.orArray.find(self, dialoguePool, topic)==-1) util.orArray.insert(self,dialoguePool,0,topic);
      ]
   ;
#endif; !--After Parser
!======================================================================================
! AFTER VERBLIB
#iftrue (LIBRARY_STAGE == AFTER_VERBLIB);
   orNpcSkill _orNPCSkillDialogue
      with doesApplyToCharacter[npc; if(npc ofclass orNPCSkillDialogue) rtrue; rfalse;]
		,  canPerform[npc;
            !TODO: make this return false, when none of the elements in the dialogPool work with the character being spoken to
            return (util.orArray.getLength(npc, dialoguePool)>0);
         ]
      ,  perform[npc i r;

            r=util.orArray.get(npc, dialoguePool,random(util.orArray.getLength(npc, dialoguePool))-1);
            util.orArray.removeValue(npc, dialoguePool,r);
            new_line;

            if(r.isKnownBy(npc))
               <tellTopic player r, npc>;
            else
               <askTopic player r, npc>;

         ]
	;

#endif; !--After VERBLIB
!======================================================================================
#endif; !--_STAGE  < LIBRARY_STAGE
#endif; !--ndef _STAGE
